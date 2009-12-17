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

#include "cusbservicecontrol.h"
#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbServiceControl::CUsbServiceControl(MUsbServiceControlObserver* aObserver,
        RUsb& aUsb) :
    CActive(CActive::EPriorityStandard), iObserver(aObserver), iUsb(aUsb),
            iPersonalityId(0)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbServiceControl::~CUsbServiceControl()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::~CUsbServiceControl" ) );
    Cancel();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbServiceControl::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::ConstructL" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbServiceControl* CUsbServiceControl::NewL(
        MUsbServiceControlObserver* aObserver, RUsb& aUsb)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::NewL" ) );

    CUsbServiceControl* self = new (ELeave) CUsbServiceControl(aObserver,
            aUsb);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self); // pop self
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbServiceControl::Start(TInt aPersonalityId)
    {
        FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbServiceControl::Start aPersonalityId = %d" ), aPersonalityId));

    TUsbServiceState serviceState;
    TInt err = iUsb.GetServiceState(serviceState);

    if (KErrNone != err)
        {
        return err;
        }

    switch (serviceState)
        {
        case EUsbServiceIdle:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start UsbServiceState == EUsbServiceIdle" ) );

            iPersonalityId = aPersonalityId; // when request completed, this will indicate that we started what we wanted
            if (IsActive())
                {
                    FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start Request is outstanding, cancelling first." ) );
                Cancel();
                }
            iUsb.TryStart(aPersonalityId, iStatus);
            SetActive();
            break;
            }

        case EUsbServiceStarted:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start UsbServiceState == EUsbServiceStarted" ) );

            TInt currentPersonality(0);
            err = iUsb.GetCurrentPersonalityId(currentPersonality);
            if (KErrNone != err)
                {
                    FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbServiceControl::Start Error getting current personality err = %d" ), err));
                return err;
                }

            if (aPersonalityId == currentPersonality) // already started
                {
                    FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start Personality already sterted" ) );
                iObserver->UsbServiceControlReqCompletedL(KErrNone);
                return KErrNone;
                }

            // we need to stop current personality and start service with new personailty id
            iPersonalityId = aPersonalityId; // this will indicate that we want to start this personality after

            Stop();

            // start new personality in RunL() when state is Idle

            break;
            }
        case EUsbServiceStarting:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start UsbServiceState == EUsbServiceStarting" ) );
            // do exactly the same as in case of EUsbServiceStopping;
            // break statement is not required here
            }
        case EUsbServiceStopping:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start UsbServiceState == EUsbServiceStopping" ) );

            // subscribe for usb service state change, and start new usb service once current one started/stopped
            iPersonalityId = aPersonalityId; // this will indicate that we want to start this personality

            // this check is needed due to usb service might be stopping by other client  
            if (IsActive())
                {
                    FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start Request outstanding. Waiting for completion." ) );
                return KErrNone; // when the outstanding request get completed, we start usb services with iPersonalityId
                }

            // otherwise, monitor usb service state
            iUsb.ServiceStateNotification(iServiceState, iStatus);
            SetActive();

            break;
            }
        case EUsbServiceFatalError:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start UsbServiceState == EUsbServiceFatalError" ) );
            return KErrGeneral;
            }
        default:
            {
            Panic(EUnknownUsbServiceState);
            }
        }

    return KErrNone;

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbServiceControl::Stop()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Stop" ) );

    TUsbServiceState serviceState;
    TInt err = iUsb.GetServiceState(serviceState);

    if (KErrNone != err)
        {
        return err;
        }

    switch (serviceState)
        {
        case EUsbServiceIdle:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Stop UsbServiceState == EUsbServiceIdle" ) );

            return KErrNone;
            }

        case EUsbServiceStarted:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Stop UsbServiceState == EUsbServiceStarted" ) );

            if (IsActive())
                {
                    FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Stop Request is outstanding, cancelling it." ) );
                Cancel();
                }
            iUsb.TryStop(iStatus);
            SetActive();

            break;
            }
        case EUsbServiceStopping:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Stop UsbServiceState == EUsbServiceStopping" ) );
            // do exactly the same as in case of EUsbServiceStarting;
            // break statement is not required here
            }
        case EUsbServiceStarting:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Stop UsbServiceState == EUsbServiceStarting" ) );

            // subscribe for usb service state change, and stop usb service once current one started
            iPersonalityId = 0; // this will indicate that we do not want to start this personality

            // this check is needed due to usb service might be starting by other client  
            if (IsActive())
                {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Stop Request is outstanding, waiting for completion. " ) );
                return KErrNone; // when this request get completed, we request to stop usb services
                }

            // otherwise, monitor usb service state
            iUsb.ServiceStateNotification(iServiceState, iStatus);
            SetActive();

            break;
            }
        case EUsbServiceFatalError:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start UsbServiceState == EUsbServiceFatalError" ) );
            return KErrGeneral;
            }
        default:
            {
            Panic(EUnknownUsbServiceState);
            }
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbServiceControl::RunL()
    {
    FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbServiceControl::RunL iStatus %d" ), iStatus.Int()));
    if (KErrNone != iStatus.Int())
        {
        //User::Leave(iStatus.Int());
        iObserver->UsbServiceControlReqCompletedL(iStatus.Int());
        return;
        }

    TUsbServiceState serviceState;
    TInt err = iUsb.GetServiceState(serviceState);

    if (KErrNone != err)
        {
            FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbServiceControl::RunL error while getting service state %d" ), err));
        //User::Leave(err);
        iObserver->UsbServiceControlReqCompletedL(err);
        return;            
        }

    switch (serviceState)
        {
        case EUsbServiceIdle: // usb service stopped
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::RunL UsbServiceState == EUsbServiceIdle" ) );

            if (iPersonalityId != 0) // during service stopping, requested to start it
                {
                    FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbServiceControl::RunL Requested to start personality %d. Starting it." ), iPersonalityId));
                TInt personalityId = iPersonalityId;
                iPersonalityId = 0; // reset
                err = Start(personalityId);
                if(KErrNone != err)
                    {
                    iObserver->UsbServiceControlReqCompletedL(err);
                    }
                return;
                }

            // otherwise, we've done, notify
            iObserver->UsbServiceControlReqCompletedL(KErrNone);
            break;
            }

        case EUsbServiceStarted:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start UsbServiceState == EUsbServiceStarted" ) );

            TInt currentPersonality(0);
            err = iUsb.GetCurrentPersonalityId(currentPersonality);
            if (KErrNone != err)
                {
                FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbServiceControl::RunL error while getting PersonalityId err = %d" ), err));
                //User::Leave(err);
                iObserver->UsbServiceControlReqCompletedL(err);
                return;                    
                }

            if (iPersonalityId == currentPersonality) // already done
                {
                    FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start Personality already started." ) );
                iPersonalityId = 0;
                iObserver->UsbServiceControlReqCompletedL(KErrNone);
                return;
                }

            if (iPersonalityId == 0) // during service start requested to stop it
                {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start Requested to stop personality. Stopping." ) );
                err = Stop();
                if(KErrNone != err)
                    {
                    FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbServiceControl::RunL error while stopping personality err = %d" ), err));
                    iObserver->UsbServiceControlReqCompletedL(err);
                    }
                return;
                }

            // otherwise, during service start, requested to start it with another personality
            FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbServiceControl::RunL Requested to start personality %d. Starting it." ), iPersonalityId));
            TInt personalityId = iPersonalityId;
            iPersonalityId = 0; // reset
            //User::LeaveIfError(Start(personalityId));
            err = Start(personalityId);
            if(KErrNone != err)
                {
                FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbServiceControl::RunL error while starting personality err = %d" ), err));
                iObserver->UsbServiceControlReqCompletedL(err);
                }
            break;
            }

        case EUsbServiceStarting:
            {
                // do exactly same as in EUsbServiceStopping
                // break statement is not required here
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start State == EUsbServiceStarting. Continue." ) );
            }
        case EUsbServiceStopping:
            {
                // we are not interested in these states, just continue monitoring
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start State == EUsbServiceStopping. Continue." ) );
            iUsb.ServiceStateNotification(iServiceState, iStatus);
            SetActive();
            break;
            }
        case EUsbServiceFatalError:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::Start UsbServiceState == EUsbServiceFatalError" ) );
            //User::Leave(KErrGeneral);
            iObserver->UsbServiceControlReqCompletedL(KErrGeneral);    
            break;
            }
        default:
            {
            Panic(EUnknownUsbServiceState);
            }
        }

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbServiceControl::RunError(TInt aError)
    {
        FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbServiceControl::RunError aError %d" ), aError ));
        //TRAP_IGNORE(iObserver->UsbServiceControlReqCompletedL(aError));

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbServiceControl::DoCancel()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::DoCancel Cancelling interest to Usb TryStart."))
    iUsb.CancelInterest(RUsb::ETryStart);
        FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::DoCancel Cancelling interest to Usb TrySop."))
    iUsb.CancelInterest(RUsb::ETryStop);
        FLOG( _L( "[USBOTGWATCHER]\tCUsbServiceControl::DoCancel Cancelling interest to usb states notifications."))
    iUsb.ServiceStateNotificationCancel();
    }
