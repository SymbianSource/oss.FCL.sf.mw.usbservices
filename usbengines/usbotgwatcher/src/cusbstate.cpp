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
CUsbState::CUsbState(CUsbOtgWatcher& aWatcher) :
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
void CUsbState::ConstructL()
    {

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::JustAdvancedToThisStateL()
    {
    iWatcher.PrintStateToLog();
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

    iWatcher.ChangeStateL(aNewStateId);

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::ChangeHostStateL(TUsbStateIds aNewStateId)
    {

    iWatcher.ChangeHostStateL(aNewStateId);

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::HandleL(TInt aWhat, TUsbStateIds aWhereToHandle)
    {
    LOG_FUNC

    LOG2( "aWhat = %d aWhere = %d" , aWhat, aWhereToHandle);

    iWatcher.HandleHostProblemL(aWhat, aWhereToHandle);
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
    LOG_FUNC
    Panic(EIdPinOffNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::IdPinOnL()
    {
    LOG_FUNC
    Panic(EIdPinOnNotExpected);
    }

// From VBus observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::VBusDownL()
    {
    LOG_FUNC
    Panic(EVBusDownNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::VBusUpL()
    {
    LOG_FUNC
    Panic(EVBusUpNotExpected);
    }

// From OTG state observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::AIdleL()
    {
    LOG_FUNC
    Panic(EAIdleNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::AHostL()
    {
    LOG_FUNC
    Panic(EAHostNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::APeripheralL()
    {
    LOG_FUNC
    Panic(EAPeripheralNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::AVBusErrorL()
    {
    LOG_FUNC
    Panic(EAVBusErrorNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::BIdleL()
    {
    LOG_FUNC
    Panic(EBIdleNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::BPeripheralL()
    {
    LOG_FUNC
    Panic(EBPeripheralNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::BHostL()
    {
    LOG_FUNC
    Panic(EBHostNotExpected);
    }

// From bus activity observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::BusIdleL()
    {
    LOG_FUNC
    Panic(EBusIdleNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::BusActiveL()
    {
    LOG_FUNC
    Panic(EBusActiveNotExpected);
    }

// From Host Event notification observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::DeviceAttachedL(TDeviceEventInformation)
    {
    LOG_FUNC
    Panic(EDeviceAttachedNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::DeviceDetachedL(TDeviceEventInformation)
    {
    LOG_FUNC
    Panic(EDeviceDetachedNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::DriverLoadSuccessL(TDeviceEventInformation)
    {
    LOG_FUNC
    Panic(EDriverLoadSuccessNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::DriverLoadPartialSuccessL(TDeviceEventInformation)
    {
    LOG_FUNC
    Panic(EDriverLoadPartialSuccessNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::DriverLoadFailureL(TDeviceEventInformation)
    {
    LOG_FUNC
    Panic(EDriverLoadFailureNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::BadHubPositionL()
    {
    LOG_FUNC
    Panic(EBadHubPositionNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::VBusErrorL()
    {
    LOG_FUNC
    Panic(EVBusErrorNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::MessageNotificationReceivedL(TInt)
    {
    LOG_FUNC
    Panic(EMessageNotificationNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::SrpReceivedL()
    {
    LOG_FUNC
    Panic(ESrpNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbState::SessionRequestedL()
    {
    LOG_FUNC
    Panic(ESessionRequestNotExpected);
    }
