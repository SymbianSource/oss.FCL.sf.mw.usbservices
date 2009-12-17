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
#include <usbpersonalityids.h>

#include "cusbstatehostainitiatebase.h"
#ifndef STIF
#include "cusbnotifmanager.h"
#include "cusbtimer.h"
#else
#include "mockcusbnotifmanager.h"
#include "mockcusbtimer.h"
#endif

#include "definitions.h"
#include "errors.h"

#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostAInitiateBase::CUsbStateHostAInitiateBase(
        CUsbOtgWatcher* aWatcher) :
    CUsbStateHostABase(aWatcher)
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiateBase::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::ConstructL" ) );

    CUsbStateHostABase::ConstructL();

    iAttachmentTimer = CUsbTimer::NewL(this, EDeviceAttachmentTimer);

    //iUsbPersonalitySwitch = CUsbPersonalitySwitch::NewL(this);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostAInitiateBase::~CUsbStateHostAInitiateBase()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::~CUsbStateHostAInitiateBase" ) );

    delete iAttachmentTimer;
    //delete iUsbPersonalitySwitch;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiateBase::JustBeforeLeavingThisStateL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::JustBeforeLeavingThisStateL" ) );
    iAttachmentTimer->Cancel();
    }

// From TimerObserver
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiateBase::TimerElapsedL(TUsbTimerId aTimerId)
    {
    switch (aTimerId)
        {
        case EDeviceAttachmentTimer:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::TimerElapsedL - AttachmentTimer" ) );
            HandleL(EUsbWatcherErrDandlingCable);
            break;
            }
        default:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::TimerElapsedL - Unknown timer" ) );
            Panic(EWrongTimerId);
            }
        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiateBase::VBusUpL()
    {
    // start timer, waiting for device attachment
    iAttachmentTimer->After(KTimeToWaitForDeviceAttachment);
    }

// From VBus observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiateBase::VBusDownL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::VBusDownL" ) );
    iAttachmentTimer->Cancel();

    CUsbStateHostABase::VBusDownL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiateBase::AHostL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::AHostL" ) );
    // do nothing
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiateBase::APeripheralL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::AHostL" ) );
    ChangeHostStateL(EUsbStateHostAPeripheral);
    }

// From Host Event notification observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiateBase::DeviceAttachedL(TDeviceEventInformation aTdi)
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::DeviceAttachedL aTdi.iDeviceId = %d" ), aTdi.iDeviceId));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::DeviceAttachedL aTdi.iEventType = %d" ), aTdi.iEventType));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::DeviceAttachedL aTdi.iError = %d" ), aTdi.iError));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::DeviceAttachedL aTdi.iDriverLoadStatus = %d" ), aTdi.iDriverLoadStatus));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::DeviceAttachedL aTdi.iVid = %d" ), aTdi.iVid));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::DeviceAttachedL aTdi.iPid = %d" ), aTdi.iPid));

    iAttachmentTimer->Cancel();
    
    // check if an OTG device connected
    TOtgDescriptor otgDescriptor;
    
    // ignore all the errors, assume we connected to not otg
    TInt err  = iWatcher->Usb().GetOtgDescriptor(aTdi.iDeviceId, otgDescriptor);
             
    TBool hnpSupported(otgDescriptor.iAttributes & EUsbOtgHNPSupported);
    TBool srpSupported(otgDescriptor.iAttributes & EUsbOtgSRPSupported);
    
    FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::DeviceAttachedL GetOtgDescriptor err = %d, HNP supported = %d, SRP supported = %d" ), err, hnpSupported, srpSupported));
             
    // OTG device supports both hnp and srp
    if(hnpSupported && srpSupported)
        {
        HandleL(EUsbWatcherConnectedToOTG);
        return;
        }
            
    if (KErrNone != aTdi.iError)
        {
        switch (aTdi.iError)
            // error in attachement
            {
            case KErrBadPower:
                {
                    FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::DeviceAttachedL TooMuchPower" ) );
                    HandleL(EUsbWatcherErrDeviceRequiresTooMuchPowerOnEnumeration);
                    break;
                }
            default:
                {
                    FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::DeviceAttachedL AttachmentError" ) );
                    HandleL(EUsbWatcherErrUnsupportedDevice);
                    break;
                }

            }
        
        return;
        }

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiateBase::DriverLoadSuccessL(TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::DriverLoadSuccessL" ) );
    ChangeHostStateL(EUsbStateHostAHost);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiateBase::DriverLoadPartialSuccessL(
        TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::DriverLoadPartialSuccessL" ) );
    iWatcher->NotifManager()->ShowNotifierL(KUsbUiNotifOtgWarning,
            EUsbOtgPartiallySupportedDevice, NULL);
    ChangeHostStateL(EUsbStateHostAHost);

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiateBase::DriverLoadFailureL(
        TDeviceEventInformation aDei)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::DriverLoadFailureL" ) );

    // try initiate to switch to mass storage
  /*  if (KErrNone != iUsbPersonalitySwitch->SwitchPersonalityL(aDei.iDeviceId,
            KUsbPersonalityIdMS ))
        {*/
        HandleL(EUsbWatcherErrDriversNotFound);
      //  }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiateBase::SrpReceivedL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::SrpReceivedL" ) );
    if (CUsbVBusObserver::EVBusUp != iWatcher->VBusObserver()->VBus())
        {
        TInt err = iWatcher->Usb().BusRespondSrp();
        if (KErrNone != err)
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::SrpReceivedL BusRespondSrp error" ) );
            iWatcher->HandleHostProblemL(EUsbWatcherErrorInConnection);
            }
        }
    }

void CUsbStateHostAInitiateBase::UsbPersonalitySwitchStateChangedL(
        MUsbPersonalitySwitchObserver::TState aState, TInt aData)
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::UsbPersonalitySwitchStateChangedL aState = %d aData = %d" ), TInt(aState), aData));

    switch (aState)
        {
        case ERequestingAllPersonalities:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::UsbPersonalitySwitchStateChangedL ERequestingAllPersonalities" ) );
            break;
            }
        case ERequestingSetPersonality:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::UsbPersonalitySwitchStateChangedL ERequestingSetPersonality" ) );
            break;
            }
        case EGetAllPersonalitiesCompleted:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::UsbPersonalitySwitchStateChangedL EGetAllPersonalitiesCompleted" ) );
            if (KErrNotFound == aData)
                {
                // no supported or needed personality/ies in the peripheral
                HandleL(EUsbWatcherErrDriversNotFound);
                }
            break;
            }
        case ESetPersonalityCompleted:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::UsbPersonalitySwitchStateChangedL ESetPersonalityCompleted" ) );

            break;
            }
        case ERequestCancelled:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::UsbPersonalitySwitchStateChangedL ERequestCancelled" ) );
            break;
            }
        case ERequestFailed:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiateBase::UsbPersonalitySwitchStateChangedL ERequestFailed" ) );
            HandleL(EUsbWatcherErrDriversNotFound);
            break;
            }
        default:
            {
            Panic(EUnexpectedUsbSwitchPersonalityState);
            }
        }
    }
