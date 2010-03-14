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

#include "cusbhosteventnotificationobserver.h"

#include "definitions.h"
#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbHostEventNotificationObserver::CUsbHostEventNotificationObserver(
        RUsb* aUsb) :
    CActive(EPriorityStandard), iUsb(aUsb)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbHostEventNotificationObserver::ConstructL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::ConstructL" ) );

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbHostEventNotificationObserver* CUsbHostEventNotificationObserver::NewL(
        RUsb* aUsb)
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::NewL" ) );

    CUsbHostEventNotificationObserver* self =
            new (ELeave) CUsbHostEventNotificationObserver(aUsb);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbHostEventNotificationObserver::~CUsbHostEventNotificationObserver()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::~CUsbHostEventNotificationObserver" ) );

    Cancel();

    iObservers.Close();

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbHostEventNotificationObserver::SubscribeL(
        MUsbHostEventNotificationObserver& aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::SubscribeL" ) );

        // check if the same observer already exist in a list
        if(KErrNotFound != iObservers.Find(&aObserver))
            {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::SubscribeL Observer already exists." ) );
            Panic(EObserverAlreadyExists);
            return;
            }
        
    iObservers.AppendL(&aObserver);

    if (KFirst == iObservers.Count()) // first item
        {
        iUsb->HostEventNotification(iStatus, iEventInfo);
        SetActive();

        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbHostEventNotificationObserver::UnsubscribeL(
        MUsbHostEventNotificationObserver& aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::UnsubscribeL" ) );
        TInt i(iObservers.Find(&aObserver));
        if(KErrNotFound == i)
                {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::UnsubscribeL Observer not found." ) );
                Panic(ECanNotFindHostEventNotificationObserver);
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
//
// ---------------------------------------------------------------------------
//
void CUsbHostEventNotificationObserver::RunL()
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL iStatus = %d" ), iStatus.Int()));

        // if error occured, tell to Observers
        if(KErrNone != iStatus.Int()) 
            {
            for (TInt i(0); i < iObservers.Count(); ++i)
                 {
                 iObservers[i]->HostEventNotificationErrorL(iStatus.Int());
                 }
            return;
            }

    TDeviceEventInformation dei(iEventInfo);

    // re-issue request first
    iUsb->HostEventNotification(iStatus, iEventInfo);
    SetActive();

        // Log the event
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL iEventInfo.iDeviceId         = %d" ), dei.iDeviceId));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL iEventInfo.iEventType        = %d" ), dei.iEventType));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL iEventInfo.iError            = %d" ), dei.iError));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL iEventInfo.iDriverLoadStatus = %d" ), dei.iDriverLoadStatus));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL iEventInfo.iVid              = %d" ), dei.iVid));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL iEventInfo.iPid              = %d" ), dei.iPid));

    // then process property change
    switch (dei.iEventType)
        {
        case EDeviceAttachment:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL DeviceAttachment" ) );

            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->DeviceAttachedL(dei);
                }
            break;
            }

        case EDeviceDetachment:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL DeviceDetachment" ) );

            for (TInt i(0); i < iObservers.Count(); ++i)
                {
                iObservers[i]->DeviceDetachedL(dei);
                }
            break;
            }

        case EDriverLoad:
            {
            switch (dei.iDriverLoadStatus)
                {
                case EDriverLoadSuccess:
                    {
                        FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL DriverLoadSuccess" ) );

                    for (TInt i(0); i < iObservers.Count(); ++i)
                        {
                        iObservers[i]->DriverLoadSuccessL(dei);
                        }

                    break;
                    }
                case EDriverLoadPartialSuccess:
                    {
                        FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL DriverLoadPartialSuccess" ) );

                    for (TInt i(0); i < iObservers.Count(); ++i)
                        {
                        iObservers[i]->DriverLoadPartialSuccessL(dei);
                        }
                    break;

                    }
                case EDriverLoadFailure:
                    {
                        FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL DriverLoadFailure" ) );

                    for (TInt i(0); i < iObservers.Count(); ++i)
                        {
                        iObservers[i]->DriverLoadFailureL(dei);
                        }
                    break;
                    }
                default:
                    {
                        FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL DriverLoadFailure WrongDriverLoadStatus" ) );
                    Panic(EWrongDriverLoadStatus);
                    }
                }
            break;

            }
        default:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunL WrongHostEventNotification" ) );
            Panic(EWrongHostEventNotification);
            }

        }

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbHostEventNotificationObserver::DoCancel()
    {
    iUsb->HostEventNotificationCancel();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbHostEventNotificationObserver::RunError(TInt aError)
    {

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbHostEventNotificationObserver::RunError aError = %d" ), aError));

    // try to recover and continue	
    return KErrNone;

    }

