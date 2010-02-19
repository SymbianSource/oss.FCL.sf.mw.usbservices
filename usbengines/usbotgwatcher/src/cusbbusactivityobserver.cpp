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

#include "cusbbusactivityobserver.h"

#include "definitions.h"
#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbBusActivityObserver::CUsbBusActivityObserver() :
    CActive(EPriorityStandard)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbBusActivityObserver::ConstructL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::ConstructL" ) );

    User::LeaveIfError(iBusActivity.Attach(KUidUsbManCategory,
            KUsbOtgConnectionIdleProperty));

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbBusActivityObserver* CUsbBusActivityObserver::NewL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::NewL" ) );

    CUsbBusActivityObserver* self = new (ELeave) CUsbBusActivityObserver();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbBusActivityObserver::~CUsbBusActivityObserver()

    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::~CUsbBusActivityObserver" ) );

    Cancel();

    iBusActivity.Close();

    iObservers.Close();

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbBusActivityObserver::TBusActivity CUsbBusActivityObserver::BusActivity()
    {

    TInt val(0);

    TInt err = iBusActivity.Get(val);

    if (KErrNone != err)
        {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::BusActivity CanNotGetBusActivityProperty" ) );
        Panic(ECanNotGetBusActivityProperty);
        }

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::State = %d" ), val ));

    return (0 == val ? EBusActive : EBusIdle);

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbBusActivityObserver::SubscribeL(MUsbBusActivityObserver& aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::SubscribeL" ) );
    // check if the same observer already exist in a list
    if(KErrNotFound != iObservers.Find(&aObserver))
        {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbIdPinObserver::SubscribeL Observer already exists." ) );
        Panic(EObserverAlreadyExists);
        return;
        }
    iObservers.AppendL(&aObserver);

    if (KFirst == iObservers.Count()) // first item
        {
        iBusActivity.Subscribe(iStatus);
        SetActive();

        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbBusActivityObserver::UnsubscribeL(MUsbBusActivityObserver& aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::UnsubscribeL" ) );

    TInt i(iObservers.Find(&aObserver));
    if(KErrNotFound == i)
        {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::UnsubscribeL Observer not found." ) );
        Panic(ECanNotFindIdPinObserver);
        return;
        }
    
    iObservers.Remove(i);
    
    if (0 == iObservers.Count()) // no observers anymore
        {
        // cancel pending request
        Cancel();
        }
    }

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//
void CUsbBusActivityObserver::RunL()
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::RunL iStatus = %d" ), iStatus.Int()));

    // if error occured, inform observers
    if(KErrNone != iStatus.Int())
        {
        for (TInt i(0); i < iObservers.Count(); ++i)
                    {
                    iObservers[i]->BusActivityErrorL(iStatus.Int());
                    }
        }

    // re-issue request first
    iBusActivity.Subscribe(iStatus);
    SetActive();

    // then process property change
    TBusActivity state(BusActivity());

    switch (state)
        {
        case EBusIdle:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::RunL BusIdle" ) );

            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->BusIdleL();
                }
            break;
            }

        case EBusActive:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::RunL BusActive" ) );

            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->BusActiveL();
                }
            break;
            }

        default:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::RunL BusActive WrongBusState" ) );
            Panic(EWrongBusState);
            }
        }

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbBusActivityObserver::DoCancel()
    {
    iBusActivity.Cancel();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbBusActivityObserver::RunError(TInt aError)
    {

    FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbBusActivityObserver::RunError aError = %d" ), aError));
                  
    // try to continue	
    return KErrNone;

    }

