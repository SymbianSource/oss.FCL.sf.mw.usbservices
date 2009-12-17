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


#include <usbotgdefs.h>

#include "cusbidpinobserver.h"

#include "definitions.h"
#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbIdPinObserver::CUsbIdPinObserver() :
    CActive(EPriorityStandard)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIdPinObserver::ConstructL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbIdPinObserver::ConstructL" ) );

    User::LeaveIfError(iIdPin.Attach(KUidUsbManCategory,
            KUsbOtgIdPinPresentProperty));

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbIdPinObserver* CUsbIdPinObserver::NewL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbIdPinObserver::NewL" ) );

    CUsbIdPinObserver* self = new (ELeave) CUsbIdPinObserver();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbIdPinObserver::~CUsbIdPinObserver()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbIdPinObserver::~CUsbIdPinObserver" ) );

    Cancel();

    iIdPin.Close();

    iObservers.Close();

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbIdPinObserver::TState CUsbIdPinObserver::IdPin()
/* this getter is not const, because for some reason RProperty::Get is not const */
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbIdPinObserver::IdPin" ) );

    TInt val(0);

    TInt err = iIdPin.Get(val);

    if (KErrNone != err)
        {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbIdPinObserver::IdPin CanNotGetIdPinProperty" ) );
        Panic(ECanNotGetIdPinProperty);
        }

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbIdPinObserver::IdPin = %d" ), val ));

    // not found in docs clear definition of this property. Verification is needed   
    return (0 == val ? EIdPinOff : EIdPinOn);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//   
void CUsbIdPinObserver::SubscribeL(MUsbIdPinObserver* aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbIdPinObserver::SubscribeL" ) );

    User::LeaveIfError(iObservers.Append(aObserver));

    if (KFirst == iObservers.Count()) // first item
        {
        iIdPin.Subscribe(iStatus);
        SetActive();

        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIdPinObserver::UnsubscribeL(MUsbIdPinObserver* aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbIdPinObserver::UnsubscribeL" ) );
    if (0 == iObservers.Count()) // no items
        {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbIdPinObserver::UnsubscribeL No observers" ) );
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
            FLOG( _L( "[USBOTGWATCHER]\tCUsbIdPinObserver::UnsubscribeL CanNotGetIdPinObserver" ) );
        Panic(ECanNotFindIdPinObserver);
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
void CUsbIdPinObserver::RunL()
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbIdPinObserver::RunL iStatus = %d" ), iStatus.Int()));

        // if error occured, tell to Observers
        if(KErrNone != iStatus.Int()) 
            {
            for (TInt i(0); i < iObservers.Count(); ++i)
                 {
                 iObservers[i]->IdPinErrorL(iStatus.Int());
                 }
            return;
            }

    // re-issue request first
    iIdPin.Subscribe(iStatus);
    SetActive();

    // then process property change
    TState state(IdPin());

    // report change   
    switch (state)
        {
        case EIdPinOn:
            {
                FLOG(_L( "[USBOTGWATCHER]\tCUsbIdPinObserver::RunL IdPin ON"));

            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->IdPinOnL();
                }
            break;
            }
            ;

        case EIdPinOff:
            {
                FLOG(_L( "[USBOTGWATCHER]\tCUsbIdPinObserver::RunL IdPin OFF"));

            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->IdPinOffL();
                }
            break;
            }
            ;

        default:
            {
                FLOG(_L( "[USBOTGWATCHER]\tCUsbIdPinObserver::RunL WrongIdPinState"));
            Panic(EWrongIdPinState);
            }
        }

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIdPinObserver::DoCancel()
    {
    iIdPin.Cancel();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbIdPinObserver::RunError(TInt aError)
    {

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbIdPinObserver::RunError aError = %d" ), aError));

    // try to continue	
    return KErrNone;

    }
