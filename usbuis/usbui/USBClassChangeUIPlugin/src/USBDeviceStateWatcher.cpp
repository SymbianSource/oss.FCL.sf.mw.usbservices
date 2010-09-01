/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Device state watcher class.
 *
*/


// INCLUDE FILES
#include "USBDeviceStateWatcher.h"

// CONSTANTS
const TUint KUsbAllStates = 0xFFFFFFFF;

// --------------------------------------------------------------------------
// Two-phased constructor. Uses existing usb manager session.
// --------------------------------------------------------------------------
CUSBDeviceStateWatcher*
CUSBDeviceStateWatcher::NewL(MUSBDeviceStateObserver& aObserver, RUsb& aUsbMan)
    {
    CUSBDeviceStateWatcher* self = new(ELeave)CUSBDeviceStateWatcher(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL(aUsbMan);
    CleanupStack::Pop(self);
    return self;
    }

// --------------------------------------------------------------------------
// Two-phased constructor. Creates its own usb manager session.
// --------------------------------------------------------------------------
CUSBDeviceStateWatcher*
CUSBDeviceStateWatcher::NewL(MUSBDeviceStateObserver& aObserver)
    {
    CUSBDeviceStateWatcher* self = new(ELeave)CUSBDeviceStateWatcher(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// --------------------------------------------------------------------------
// C++ constructor
// --------------------------------------------------------------------------
CUSBDeviceStateWatcher::CUSBDeviceStateWatcher(MUSBDeviceStateObserver& aObserver) 
    : CActive(EPriorityStandard), iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }

// --------------------------------------------------------------------------
// C++ destructor
// --------------------------------------------------------------------------
CUSBDeviceStateWatcher::~CUSBDeviceStateWatcher()
    {
    Cancel();
    }

// --------------------------------------------------------------------------
// Symbian 2nd phase constructor. Uses existing usb manager session. 
// --------------------------------------------------------------------------
void CUSBDeviceStateWatcher::ConstructL(RUsb& aUsbMan)
    {
    iUsbMan.SetHandleNC(aUsbMan.Handle()); // NC == NoClose
    CommonConstructL();
    }

// --------------------------------------------------------------------------
// Symbian 2nd phase constructor. Creates its own usb manager session. 
// --------------------------------------------------------------------------
void CUSBDeviceStateWatcher::ConstructL()
    {
    User::LeaveIfError(iUsbMan.Connect());
    CommonConstructL();
    }

// --------------------------------------------------------------------------
// Code shared by all ConstructL methods.
// Usb manager session has already been set up.
// --------------------------------------------------------------------------
void CUSBDeviceStateWatcher::CommonConstructL()
    {
    User::LeaveIfError(iUsbMan.GetDeviceState(iCurrentState));
    iPreviousState = iCurrentState;
    iUsbMan.DeviceStateNotification(KUsbAllStates, iCurrentState, iStatus);
    SetActive();
    }

// --------------------------------------------------------------------------
// The device state has changed.
// --------------------------------------------------------------------------
void CUSBDeviceStateWatcher::RunL()
    {
    if (iStatus == KErrNone)
        {
            if (iCurrentState != iPreviousState)
                {
                    iObserver.DeviceStateChanged(iPreviousState, iCurrentState);
                    iPreviousState = iCurrentState;
                }
            iUsbMan.DeviceStateNotification(KUsbAllStates, iCurrentState, iStatus);
            SetActive();
        }
    }
 
// ----------------------------------------------------------------------------
// Standard active object error function.
// ----------------------------------------------------------------------------
TInt CUSBDeviceStateWatcher::RunError(TInt /*aError*/)
    {
    // Currently no leaving functions called in RunL, thus nothing should cause
    // this to be called -> return.
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Standard active object cancellation function.
// ----------------------------------------------------------------------------
void CUSBDeviceStateWatcher::DoCancel()
    {
    iUsbMan.DeviceStateNotificationCancel();    
    }
