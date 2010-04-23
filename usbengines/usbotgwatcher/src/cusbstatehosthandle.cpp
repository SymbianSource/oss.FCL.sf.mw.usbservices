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


#include <usbuinotif.h>

#include "cusbstatehosthandle.h"
#ifndef STIF
#include "cusbnotifmanager.h"
#else
#include "mockcusbnotifmanager.h"
#endif

#include "errors.h"
#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostHandle::CUsbStateHostHandle(CUsbOtgWatcher* aWatcher) :
    CUsbState(aWatcher)
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostHandle* CUsbStateHostHandle::NewL(CUsbOtgWatcher* aWatcher)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::NewL" ) );

    CUsbStateHostHandle* self = new (ELeave) CUsbStateHostHandle(aWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::ConstructL" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostHandle::~CUsbStateHostHandle()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::~CUsbStateHostHandle" ) );

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TUsbStateIds CUsbStateHostHandle::Id()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::Id" ) );

    return EUsbStateHostHandle;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::JustAdvancedToThisStateL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::JustAdvancedToThisStateL" ) );
    iWatcher->PrintStateToLog();

    DoHandleL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::JustBeforeLeavingThisStateL()
    {
    iWatcher->NotifManager()->CloseAllNotifiers();
    }
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::SetWhat(TInt aWhat)
    {
    iWhat = aWhat;
    }

// this means only one wait notifier at a time can be shown by this state
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::WaitNotifierCompletedL(TInt /*aFeedback*/)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL" ) );
    switch (iWhat)
        {
        case EUsbWatcherErrDriversNotFound:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL DriversNotFound" ) );
            ChangeHostStateL(EUsbStateHostAInitiate);
            break;
            }
        case EUsbWatcherHubsNotSupported:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL EUsbWatcherHubsNotSupported" ) );
            ChangeHostStateL(EUsbStateHostAInitiate);
            break;
            }
        case EUsbWatcherErrDeviceRequiresTooMuchPower:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL EUsbWatcherErrDeviceRequiresTooMuchPower" ) );
            ChangeHostStateL(EUsbStateHostAInitiate);
            break;
            }
        case EUsbWatcherErrDeviceRequiresTooMuchPowerOnEnumeration:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL EUsbWatcherErrDeviceRequiresTooMuchPowerOnEnumeration" ) );
            ChangeHostStateL(EUsbStateHostAInitiate);
            break;
            }
        case EUsbWatcherErrUnsupportedDevice:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL EUsbWatcherErrUnsupportedDevice" ) );
            ChangeHostStateL(EUsbStateHostAInitiate);
            break;
            }
        case EUsbWatcherConnectedToOTG:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL EUsbWatcherConnectedToOTG" ) );
            ChangeHostStateL(EUsbStateHostAInitiate);
            break;
            }
        case EUsbWatcherErrDandlingCable:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL EUsbWatcherErrDandlingCable" ) );
            ChangeHostStateL(EUsbStateHostAInitiate);
            break;
            }
        case EUsbWatcherNoActivity:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL EUsbWatcherNoActivity" ) );
            ChangeHostStateL(EUsbStateHostAInitiate);
            break;
            }
        case EUsbWatcherErrorInConnection:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL EUsbWatcherErrorInConnection" ) );
            ChangeHostStateL(EUsbStateHostAInitiate);
            break;
            }
        case EUsbWatcherCanNotStartUsbServices:
            {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL EUsbWatcherCanNotStartUsbServices" ) );
            
            break;                     
            }
        // errors from observers mapped to one
        case EUsbWatcherIdPinError:
        case EUsbWatcherVBusObserverError:
        case EUsbWatcherHostEventNotificationError:
        case EUsbWatcherOtgStateError:
        case EUsbWatcherMessageNotificationError:
            {
            // try to restore
            FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL Error in observer" ) );
            ChangeHostStateL(EUsbStateHostAInitiate);
            break;
            }
        default:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::WaitNotifierCompletedL Unexpected situation to be handled" ) );
            Panic(EUnexpectedSituationToHandle);
            break;
            }

        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::DoHandleL()
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DoHandleL iWhat = %d" ), iWhat));
        
    // Drop VBus first    
    // Ignore any errors when calling BusDrop(). Those indicate that VBus already dropped 
    iWatcher->Usb().BusDrop();
    
    switch (iWhat)
        {
                
        case EUsbWatcherHubsNotSupported:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DoHandleL EUsbWatcherHubsNotSupported" ) );
            iWatcher->NotifManager()->ShowNotifierL(KUsbUiNotifOtgError,
                    EUsbOtgHubUnsupported, this);
            break;
            }
        case EUsbWatcherErrDeviceRequiresTooMuchPower:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DoHandleL EUsbWatcherErrDeviceRequiresTooMuchPower" ) );
            iWatcher->NotifManager()->ShowNotifierL(KUsbUiNotifOtgError,
                    EUsbOtgTooMuchPower, this);
            break;
            }
        case EUsbWatcherErrUnsupportedDevice:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DoHandleL EUsbWatcherErrUnsupportedDevice" ) );
            iWatcher->NotifManager()->ShowNotifierL(KUsbUiNotifOtgError,
                    EUsbOtgUnsupportedDevice, this);
            break;
            }
        case EUsbWatcherConnectedToOTG:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DoHandleL EUsbWatcherConnectedToOTG" ) );
            iWatcher->NotifManager()->ShowNotifierL(KUsbUiNotifOtgError,
                    EUsbOtgUnsupportedDevice, this);
                   
            break;
            }
        case EUsbWatcherErrDandlingCable:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DoHandleL EUsbWatcherErrDandlingCable" ) );
            iWatcher->NotifManager()->ShowNotifierL(KUsbUiNotifOtgError,
                    EUsbOtgErrorAttachTimedOut, this);
            break;
            }
        case EUsbWatcherNoActivity:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DoHandleL EUsbWatcherNoActivity" ) );
            iWatcher->NotifManager()->ShowNotifierL(KUsbUiNotifOtgError,
                    EUsbOtgUnsupportedDevice, this);

            break;
            }
        case EUsbWatcherErrorInConnection:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DoHandleL EUsbWatcherErrorInConnection" ) );
            iWatcher->NotifManager()->ShowNotifierL(KUsbUiNotifOtgError,
                    EUsbOtgErrorInConnection, this);
            break;
            }
        case EUsbWatcherCanNotStartUsbServices:
            {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DoHandleL EUsbWatcherCanNotStartUsbServices" ) );
            iWatcher->NotifManager()->ShowNotifierL(KUsbUiNotifOtgError,
                EUsbOtgErrorInConnection, this);
            break;           
            }
        // errors from observers mapped to one
        case EUsbWatcherIdPinError:
        case EUsbWatcherVBusObserverError:
        case EUsbWatcherHostEventNotificationError:
        case EUsbWatcherOtgStateError:
        case EUsbWatcherMessageNotificationError:
            {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DoHandleL Error from observer" ) );
        iWatcher->NotifManager()->ShowNotifierL(KUsbUiNotifOtgError,
                EUsbOtgErrorInConnection, this);
            break;
            }
        default:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DoHandleL Unexpected situation to be handled" ) );
            Panic(EUnexpectedSituationToHandle);
            break;
            }
        }
    }

/////////////////////////////////////////////////////////////////////////////////////
// From VBus observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::VBusUpL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::VBusUpL" ) );
        // as result of BusRespondSrp() VBus might rise up.
        // role swap is not supported => drop vbus
        // when role swap is supported, leave this function empty
        
        iWatcher->Usb().BusDrop();
    }

// ---------------------------------------------------------------------------
// this is expected in the state, for example after calling BusDrop
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::VBusDownL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::VBusDownL" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::VBusErrorL()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::VBusErrorL" ) );

    iWatcher->Usb().BusClearError();
    }

// From OTG state observer
// ---------------------------------------------------------------------------
// this AIdle means that VBus gets down. From usbwatcher POV no need to change state here
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::AIdleL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::AIdleL" ) );

    }

// ---------------------------------------------------------------------------
// this means VBus gets up (for example as result RespondSrp()). Do not change state to Host, due to for usbwatcher it would mean 
// that device attached and driverls found.
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::AHostL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::AHostL" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::APeripheralL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::APeripheralL" ) );
        
        ChangeHostStateL(EUsbStateHostAPeripheral);

    }

// ---------------------------------------------------------------------------
// ignore any problems on VBus, just clear it
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::AVBusErrorL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::AVBusErrorL" ) );
        iWatcher->Usb().BusClearError();
    }

// From bus activity observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::BusIdleL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::BusIdleL" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::BusActiveL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::BusActiveL" ) );
    }

// From Host Event notification observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::DeviceAttachedL(
        TDeviceEventInformation aDevEventInfo)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DeviceAttachedL" ) );

    } 

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::DeviceDetachedL(TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::DeviceDetachedL" ) );

    } 

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::DriverLoadSuccessL(TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::DriverLoadSuccessL" ) );
    
    }
    
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::DriverLoadPartialSuccessL(TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::DriverLoadPartialSuccessL" ) );
    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::DriverLoadFailureL(TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::DriverLoadFailureL" ) );
    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::MessageNotificationReceivedL(TInt)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::MessageNotificationReceivedL" ) );

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::SrpReceivedL()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::SrpReceivedL" ) );
        
    TInt err = iWatcher->Usb().BusRespondSrp();
    FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostHandle::SrpReceivedL BusRespondSrp()=%d" ), err));
    
    if (KErrNone != err)
            {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::SrpReceivedL BusRespondSrp error" ) );
            iWhat = EUsbWatcherErrorInConnection;
            DoHandleL();
            return;
            }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostHandle::SessionRequestedL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostHandle::SessionRequestedL" ) );

    }
