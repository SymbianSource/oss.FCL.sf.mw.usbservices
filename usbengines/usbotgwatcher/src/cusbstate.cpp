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


#include "cusbstate.h"

#include "panic.h"
#include "debug.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbState::CUsbState(CUsbOtgWatcher* aWatcher) :
    iWatcher(aWatcher)
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbState::~CUsbState()
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbState* CUsbState::NewL(CUsbOtgWatcher* aWatcher)
    {
    CUsbState* self = new (ELeave) CUsbState(aWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::ConstructL()
    {

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TUsbStateIds CUsbState::Id()
    {
    return EUsbStateUndefined;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::JustAdvancedToThisStateL()
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::JustBeforeLeavingThisStateL()
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::ChangeStateL(TUsbStateIds aNewStateId)
    {

    iWatcher->ChangeStateL(aNewStateId);

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::ChangeHostStateL(TUsbStateIds aNewStateId)
    {

    iWatcher->ChangeHostStateL(aNewStateId);

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::SetPersonalityL()
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::CancelSetPersonalityL()
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::SetPreviousPersonalityL()
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::CancelSetPreviousPersonalityL()
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::SetPreviousPreviousPersonalityOnDisconnectL()
    {
    }

/////////////////////////////////////////////////////////////////////////////////////

// From IdPin observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::IdPinOffL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::IdPinOffL" ) );
    Panic(EIdPinOffNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::IdPinOnL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::IdPinOnL" ) );
    Panic(EIdPinOnNotExpected);
    }

// From VBus observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::VBusDownL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::VBusDownL" ) );
    Panic(EVBusDownNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::VBusUpL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::VBusUpL" ) );
    }

// From OTG state observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::AIdleL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::AIdleL" ) );
    Panic(EAIdleNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::AHostL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::AHostL" ) );
    Panic(EAHostNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::APeripheralL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::APeripheralL" ) );
    Panic(EAPeripheralNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::AVBusErrorL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::AVBusErrorL" ) );
    Panic(EAVBusErrorNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::BIdleL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::BIdleL" ) );
    Panic(EBIdleNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::BPeripheralL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::BPeripheralL" ) );
    Panic(EBPeripheralNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::BHostL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::BHostL" ) );
    Panic(EBHostNotExpected);
    }

// From bus activity observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::BusIdleL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::BusIdleL" ) );
    Panic(EBusIdleNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::BusActiveL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::BusActiveL" ) );
    Panic(EBusActiveNotExpected);
    }

// From Host Event notification observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::DeviceAttachedL(TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::DeviceAttachedL" ) );
    Panic(EDeviceAttachedNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::DeviceDetachedL(TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::DeviceDetachedL" ) );
    Panic(EDeviceDetachedNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::DriverLoadSuccessL(TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::DriverLoadSuccessL" ) );
    Panic(EDriverLoadSuccessNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::DriverLoadPartialSuccessL(TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::DriverLoadPartialSuccessL" ) );
    Panic(EDriverLoadPartialSuccessNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::DriverLoadFailureL(TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::DriverLoadFailureL" ) );
    Panic(EDriverLoadFailureNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::BadHubPositionL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::BadHubPositionL" ) );
    Panic(EBadHubPositionNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::VBusErrorL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::VBusErrorL" ) );
    Panic(EVBusErrorNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::MessageNotificationReceivedL(TInt)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::MessageNotificationReceivedL" ) );
    Panic(EMessageNotificationNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::SrpReceivedL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::SrpReceivedL" ) );
    Panic(ESrpNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::SessionRequestedL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbState::SessionRequestedL" ) );
    Panic(ESessionRequestNotExpected);
    }
