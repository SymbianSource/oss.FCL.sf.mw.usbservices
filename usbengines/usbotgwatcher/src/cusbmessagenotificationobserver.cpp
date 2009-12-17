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


#include <usbman.h>
#include <d32usbdi_errors.h>
#include <d32otgdi_errors.h>

#include "cusbmessagenotificationobserver.h"

#include "definitions.h"
#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbMessageNotificationObserver::CUsbMessageNotificationObserver(RUsb* aUsb) :
    CActive(EPriorityStandard), iUsb(aUsb)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbMessageNotificationObserver::ConstructL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::ConstructL" ) );

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbMessageNotificationObserver* CUsbMessageNotificationObserver::NewL(
        RUsb* aUsb)
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::NewL" ) );

    CUsbMessageNotificationObserver* self =
            new (ELeave) CUsbMessageNotificationObserver(aUsb);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbMessageNotificationObserver::~CUsbMessageNotificationObserver()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::~CUsbMessageNotificationObserver" ) );

    Cancel();

    iObservers.Close();

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbMessageNotificationObserver::SubscribeL(
        MUsbMessageNotificationObserver* aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::SubscribeL" ) );

    User::LeaveIfError(iObservers.Append(aObserver));

    if (KFirst == iObservers.Count()) // first item
        {
        iUsb->MessageNotification(iStatus, iMessage);
        SetActive();
        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbMessageNotificationObserver::UnsubscribeL(
        MUsbMessageNotificationObserver* aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::UnsubscribeL" ) );

        if (0 == iObservers.Count()) // no items
            {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::UnsubscribeL No observers" ) );
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
            FLOG( _L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::UnsubscribeL CanNotFindMessageNotificationObserver" ) );
        Panic(ECanNotFindMessageNotificationObserver);
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
void CUsbMessageNotificationObserver::RunL()
    {
    // if error occured, tell to Observers
    if(KErrNone != iStatus.Int()) 
        {
        for (TInt i(0); i < iObservers.Count(); ++i)
             {
             iObservers[i]->MessageNotificationErrorL(iStatus.Int());
             }
        return;
        }

    TInt message(iMessage);

    // re-issue request first
    iUsb->MessageNotification(iStatus, iMessage);
    SetActive();

        // Log the event
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::RunL iMessage = %d" ), message));

    // then process property change
    switch (message)
        {
        case KErrUsbBadHubPosition:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::RunL HubBadPosition" ) );

            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->BadHubPositionL();
                }
            break;
            }
        case KErrUsbOtgVbusError:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::RunL VBusError" ) );

            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->VBusErrorL();
                }
            break;
            }
        case KUsbMessageSrpReceived:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::RunL SRP received" ) );

            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->SrpReceivedL();
                }
            break;
            }
        case KUsbMessageRequestSession:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::RunL Session Requested" ) );

            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->SessionRequestedL();
                }
            break;
            }
            // notify states with other messages  
        default:
            {
            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->MessageNotificationReceivedL(message);
                }
            break;
            }

        }

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbMessageNotificationObserver::DoCancel()
    {
    iUsb->MessageNotificationCancel();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbMessageNotificationObserver::RunError(TInt aError)
    {

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbMessageNotificationObserver::RunError aError = %d" ), aError));

    // try to recover and continue	
    return KErrNone;

    }

