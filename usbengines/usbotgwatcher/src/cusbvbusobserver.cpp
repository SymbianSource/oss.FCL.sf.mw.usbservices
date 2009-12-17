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

#include "cusbvbusobserver.h"

#include "definitions.h"
#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbVBusObserver::CUsbVBusObserver() :
    CActive(EPriorityStandard)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbVBusObserver::ConstructL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbVBusObserver::ConstructL" ) );

    User::LeaveIfError(iVBus.Attach(KUidUsbManCategory,
            KUsbOtgVBusPoweredProperty));

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbVBusObserver* CUsbVBusObserver::NewL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbVBusObserver::NewL" ) );

    CUsbVBusObserver* self = new (ELeave) CUsbVBusObserver();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbVBusObserver::~CUsbVBusObserver()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbVBusObserver::~CUsbVBusObserver" ) );

    Cancel();

    iVBus.Close();

    iObservers.Close();

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbVBusObserver::TState CUsbVBusObserver::VBus() /* not const, because for some reason RProperty::Get is not const! */
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbVBusObserver::VBus" ) );

    TInt val(0);

    TInt err = iVBus.Get(val);

    if (KErrNone != err)
        {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbVBusObserver::VBus CanNotGetVBusProperty" ) );
        Panic(ECanNotGetVBusProperty);
        }

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbVBusObserver::VBus = %d" ), val ));

    // not found in docs clear definition of this property. Verification is needed   
    return (0 == val ? EVBusDown : EVBusUp);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbVBusObserver::SubscribeL(MUsbVBusObserver* aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbVBusObserver::SubscribeL" ) );

    User::LeaveIfError(iObservers.Append(aObserver));

    if (KFirst == iObservers.Count()) // first item
        {
        iVBus.Subscribe(iStatus);
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbVBusObserver::UnsubscribeL(MUsbVBusObserver* aObserver)
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbVBusObserver::UnsubscribeL" ) );
    
    if (0 == iObservers.Count()) // no items
        {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbVBusObserver::UnsubscribeL No observers" ) );
        return;
        }
    
    TInt i(0);
    while (i < iObservers.Count() && aObserver != iObservers[i])
        {
        ++i;
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbVBusObserver::UnsubscribeL i = %d" ), i ));
        }

    if (aObserver == iObservers[i]) // found
        {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbVBusObserver::UnsubscribeL Removing item" ) );
        iObservers.Remove(i);
        }
    else
        {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbVBusObserver::UnsubscribeL CanNotFindVBusObserver" ) );
        Panic(ECanNotFindVBusObserver);
        }
    
    if (0 == iObservers.Count()) // no observers anymore
        {
        // cancel pending request, if any
        Cancel();
        return;
        }
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CUsbVBusObserver::RunL()
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbVBusObserver::RunL iStatus = %d" ), iStatus.Int()));

        // if error occured, tell to Observers
        if(KErrNone != iStatus.Int()) 
            {
            for (TInt i(0); i < iObservers.Count(); ++i)
                 {
                 iObservers[i]->VBusObserverErrorL(iStatus.Int());
                 }
            return;
            }

    // re-issue request first
    iVBus.Subscribe(iStatus);
    SetActive();

    // then process property change
    TState state(VBus());

    // report change   
    switch (state)
        {
        case EVBusUp:
            {
                FLOG(_L( "[USBOTGWATCHER]\tCUsbVBusObserver::RunL VBus UP"));

            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->VBusUpL();
                }
            break;
            }
            ;

        case EVBusDown:
            {
                FLOG(_L( "[USBOTGWATCHER]\tCUsbVBusObserver::RunL VBus DOWN"));

            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->VBusDownL();
                }
            break;
            }
            ;

        default:
            {
            Panic(EWrongVBusState);
            }
        }

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbVBusObserver::DoCancel()
    {
    iVBus.Cancel();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbVBusObserver::RunError(TInt aError)
    {

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbVBusObserver::RunError aError = %d" ), aError));

    // try to continue	
    return KErrNone;
    }
