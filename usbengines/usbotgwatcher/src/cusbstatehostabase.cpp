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


#include <e32base.h> 
#include <usbhosterrors.h>
#include <d32otgdi_errors.h>
#include <d32usbdi_errors.h>

#include "cusbstatehostabase.h"
#include "cusbstatehosthandle.h"

#include "errors.h"

#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostABase::CUsbStateHostABase(CUsbOtgWatcher* aWatcher) :
    CUsbState(aWatcher)
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostABase::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::ConstructL" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostABase::~CUsbStateHostABase()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::~CUsbStateHostABase" ) );

    }

// From VBus observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostABase::VBusDownL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::VBusDownL" ) );
    ChangeHostStateL(EUsbStateHostAIdle);
    }

// From OTG state observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostABase::AIdleL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::AIdleL" ) );
    // do nothing
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostABase::AVBusErrorL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::AVBusErrorL" ) );
    // No need to handle BusClearError error code, due to Bus will be dropped anyway
    iWatcher->Usb().BusClearError();
    HandleL(EUsbWatcherErrDeviceRequiresTooMuchPower, EUsbStateHostHandle);
    }

// From bus activity observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostABase::BusIdleL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::BusIdleL" ) );
    //do nothing
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostABase::BusActiveL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::BusActiveL" ) );
    // do nothing
    }

// From message notification observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostABase::MessageNotificationReceivedL(TInt aMessage)
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostABase::MessageNotificationReceivedL = %d" ), aMessage));

    switch (aMessage)
        {

        // OTGDI
        case KErrUsbOtgEventQueueOverflow: //         = -6670;
        case KErrUsbOtgStateQueueOverflow://         = -6671;
        case KErrUsbOtgMessageQueueOverflow: //       = -6672;

        case KErrUsbOtgBadState://                   = -6675;

        case KErrUsbOtgStackNotStarted://            = -6680;
        //case KErrUsbOtgVbusAlreadyRaised://          = -6681;
        //case KErrUsbOtgSrpForbidden://               = -6682;

        case KErrUsbOtgHnpNotResponding://           = -6683;
        //case KErrUsbOtgHnpBusDrop://                 = -6684;

        case KErrUsbOtgBusControlProblem://          = -6685;

        case KErrUsbOtgVbusPowerUpError://           = -6686;

        case KErrUsbOtgHnpEnableProblem://           = -6687;

        case KErrUsbOtgVbusError://                  = -6690;
        case KErrUsbOtgSrpTimeout://                 = -6691;
        //case KErrUsbOtgSrpActive://                  = -6692;
        //case KErrUsbOtgSrpNotPermitted://            = -6693;
        //case KErrUsbOtgHnpNotPermitted://            = -6694;
        //case KErrUsbOtgHnpNotEnabled://              = -6695;
        //case KErrUsbOtgHnpNotSuspended://            = -6696;
        //case KErrUsbOtgVbusPowerUpNotPermitted://    = -6697;
        //case KErrUsbOtgVbusPowerDownNotPermitted://  = -6698;
        //case KErrUsbOtgVbusClearErrorNotPermitted:// = -6699;

            // hosterrors.h
        case KErrUsbConfigurationHasNoInterfaces:
        case KErrUsbInterfaceCountMismatch:
        case KErrUsbDuplicateInterfaceNumbers:
        case KErrUsbBadDescriptor:
        case KErrUsbDeviceDetachedDuringDriverLoading:
        case KErrUsbAttachmentFailureGeneralError:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::MessageNotificationReceivedL ErrorInConnection" ) );
            HandleL(EUsbWatcherErrorInConnection, EUsbStateHostHandle);
            break;
            }

            // OTGDI
        case KErrUsbOtgPeriphNotSupported: //        = -6688

            // USBDI
        case KErrUsbRequestsPending:
        case KErrUsbBadAddress:
        case KErrUsbNoAddress:
        case KErrUsbSetAddrFailed:
        case KErrUsbNoPower:
        case KErrUsbTooDeep:
        case KErrUsbIOError:
        case KErrUsbNotConfigured:
        case KErrUsbTimeout:
        case KErrUsbStalled:
        case KErrUsbTestFailure:
        case KErrUsbBadState:
        case KErrUsbDeviceSuspended:
        case KErrUsbBadDescriptorTopology:
        case KErrUsbDeviceRejected:
        case KErrUsbDeviceFailed:
        case KErrUsbBadDevice:
        case KErrUsbBadHub:
        case KErrUsbEventOverflow:

            // hosterrors.h
        case KErrUsbUnsupportedDevice:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::MessageNotificationReceivedL UnsupportedDevice" ) );
            HandleL(EUsbWatcherErrUnsupportedDevice, EUsbStateHostHandle);
            break;
            }
        default:
            {
                FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostABase::MessageNotificationReceivedL Unhandled message = %d" ), aMessage));
            break;
            }

        }

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostABase::BadHubPositionL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::BadHubPositionL" ) );
    HandleL(EUsbWatcherHubsNotSupported, EUsbStateHostHandle);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostABase::VBusErrorL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::VBusErrorL" ) );
    // No error code handling after BusClearError call, due to bus will be dropped anyway
    iWatcher->Usb().BusClearError();
    HandleL(EUsbWatcherErrDeviceRequiresTooMuchPower, EUsbStateHostHandle);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostABase::SessionRequestedL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::SessionRequestedL" ) );

    if (iWatcher->VBusObserver()->VBus() == CUsbVBusObserver::EVBusUp)
        {
            // session already ongoing; BusRequest() in this case returns KErrUsbOtgBadState...
            FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::SessionRequestedL() VBus is already UP; ignore Session request." ) );
        return;
        }

    TInt err = iWatcher->Usb().BusRequest();
    if (KErrNone != err && KErrUsbOtgVbusAlreadyRaised != err) // sometimes this also comes...
        {
            FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostABase::SessionRequestedL BusRequestError err = %d" ), err));
        iWatcher->HandleHostProblemL(EUsbWatcherErrorInConnection, EUsbStateHostHandle);
        return;
        }

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostABase::SessionRequestedL Session started OK (or VBUS already UP) err = %d" ), err));
    }
