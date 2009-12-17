/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation
 *
*/


#include "cusbotgstateobserver.h"

#include "definitions.h"
#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbOtgStateObserver::CUsbOtgStateObserver() :
    CActive(EPriorityStandard)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgStateObserver::ConstructL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgStateObserver::ConstructL" ) );

    User::LeaveIfError(iOtgState.Attach(KUidUsbManCategory,
            KUsbOtgStateProperty));

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbOtgStateObserver* CUsbOtgStateObserver::NewL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgStateObserver::NewL" ) );

    CUsbOtgStateObserver* self = new (ELeave) CUsbOtgStateObserver();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbOtgStateObserver::~CUsbOtgStateObserver()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgStateObserver::~CUsbOtgStateObserver" ) );

    Cancel();

    iOtgState.Close();

    iObservers.ResetAndDestroy();

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TUsbOtgState CUsbOtgStateObserver::OtgState()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgStateObserver::OtgState" ) );

    TInt val(0);

    TInt err = iOtgState.Get(val);

    if (KErrNone != err)
        {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgStateObserver::StOtgState CanNotGetOtgStateProperty" ) );
        Panic(ECanNotGetOtgStateProperty);
        }

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgStateObserver::OtgState = %d" ), val ));

    return (TUsbOtgState) val;

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgStateObserver::SubscribeL(MUsbOtgStateObserver* aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOTGStateObserver::SubscribeL" ) );

    User::LeaveIfError(iObservers.Append(aObserver));

    if (KFirst == iObservers.Count()) // first item
        {
        iOtgState.Subscribe(iStatus);
        SetActive();

        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgStateObserver::UnsubscribeL(MUsbOtgStateObserver* aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOTGStateObserver::UnsubscribeL" ) );

        if (0 == iObservers.Count()) // no items
                {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgStateObserver::UnsubscribeL No observers" ) );
                return;
                }
        
    TInt i(0);
    while (i < iObservers.Count() && aObserver != iObservers[i])
        ++i;

    if (aObserver == iObservers[i]) // found
        {
        iObservers.Remove(i);
        }
    else
        {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbOTGStateObserver::UnsubscribeL CanNotFindOtgStateObserver" ) );
        Panic(ECanNotFindOtgStateObserver);
        }

    if (0 == iObservers.Count()) // no items
        {
        // cancel pending request
        Cancel();
        }
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CUsbOtgStateObserver::RunL()
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgStateObserver::RunL iStatus = %d" ), iStatus.Int()));

    // if error occured, tell to Observers
    if(KErrNone != iStatus.Int()) 
        {
        for (TInt i(0); i < iObservers.Count(); ++i)
             {
             iObservers[i]->OtgStateErrorL(iStatus.Int());
             }
        return;
        }
    
    // re-issue request first
    iOtgState.Subscribe(iStatus);
    SetActive();

    // then process property change
    TUsbOtgState state(OtgState());

    switch (state)
        {
        case EUsbOtgStateReset:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOTGStateObserver::RunL OTGState == RESET" ) );
            break;
            }
        case EUsbOtgStateAIdle:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOTGStateObserver::RunL OTGState == AIdle" ) );
            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->AIdleL();
                }
            break;
            }
        case EUsbOtgStateAHost:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOTGStateObserver::RunL OTGState == AHost" ) );
            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->AHostL();
                }
            break;
            }
        case EUsbOtgStateAPeripheral:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOTGStateObserver::RunL OTGState == APeripheral" ) );
            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->APeripheralL();
                }
            break;
            }
        case EUsbOtgStateAVbusError:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOTGStateObserver::RunL OTGState == AVBusError" ) );
            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->AVBusErrorL();
                }
            break;
            }
        case EUsbOtgStateBIdle:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOTGStateObserver::RunL OTGState == BIdle" ) );
            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->BIdleL();
                }
            break;
            }
        case EUsbOtgStateBPeripheral:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOTGStateObserver::RunL OTGState == BPeripheral" ) );
            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->BPeripheralL();
                }
            break;
            }
        case EUsbOtgStateBHost:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOTGStateObserver::RunL OTGState == BHost" ) );
            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->BHostL();
                }
            break;
            }
        default:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOTGStateObserver::RunL WrongOtgState" ) );
            Panic(EWrongOtgState);
            }
        }

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgStateObserver::DoCancel()
    {
    iOtgState.Cancel();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbOtgStateObserver::RunError(TInt aError)
    {

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgStateObserver::RunError aError = %d" ), aError));

    // try to continue	
    return KErrNone;

    }

