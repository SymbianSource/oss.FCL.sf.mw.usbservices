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
#include <e32base.h>
#include <e32property.h>
#include <UsbWatcherInternalPSKeys.h>

#include "usbotghoststatewatcher.h"
#include "USBClassChangeUIPluginDebug.h"

// --------------------------------------------------------------------------
// Two-phased constructor. Uses existing usb manager session.
// --------------------------------------------------------------------------
CUSBOtgHostStateWatcher* 
CUSBOtgHostStateWatcher::NewL(MUSBOtgHostStateObserver& aObserver, RUsb& aUsbMan)
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgHostStateWatcher:NewL"));
    
    CUSBOtgHostStateWatcher* self = new(ELeave)CUSBOtgHostStateWatcher(aObserver, aUsbMan);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// --------------------------------------------------------------------------
// C++ destructor
// --------------------------------------------------------------------------
CUSBOtgHostStateWatcher::~CUSBOtgHostStateWatcher()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgHostStateWatcher:~CUSBOtgHostStateWatcher"));

    delete iOtgIdPinWatcher;
    delete iHostEventWatcher;
    iPeripheral.Close();
    }

// --------------------------------------------------------------------------
// Get Id pin state
// --------------------------------------------------------------------------
TInt CUSBOtgHostStateWatcher::IsIdPinOn(TBool& aIsIdPinOn)
    {
    return iOtgIdPinWatcher->IsIdPinOn(aIsIdPinOn);
    }

// --------------------------------------------------------------------------
// Check wheather a pheripheral device is connected or not. 
// --------------------------------------------------------------------------
TInt CUSBOtgHostStateWatcher::IsPeripheralConnected(TBool &aIsConnected)
    {
    aIsConnected = EFalse;

    TInt val(0);
    TInt err = iPeripheral.Get(val);

    if (KErrNone != err)
        {
        FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgHostStateWatcher::IsPeripheralConnected - err=%d"), err));
        return err;
        }
    
    FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgHostStateWatcher::IsPeripheralConnected - val=%d"), val));
    aIsConnected = (TBool)val;
    
    return KErrNone;
    }

// --------------------------------------------------------------------------
// C++ constructor
// --------------------------------------------------------------------------
CUSBOtgHostStateWatcher::CUSBOtgHostStateWatcher(
        MUSBOtgHostStateObserver& aObserver, RUsb& aUsbMan) 
    : iUsbMan(aUsbMan), iObserver(aObserver)
    {
    }

// --------------------------------------------------------------------------
// Symbian 2nd phase constructor. 
// --------------------------------------------------------------------------
void CUSBOtgHostStateWatcher::ConstructL()
    {
    iOtgIdPinWatcher = CUSBOtgIdPinStateWatcher::NewL(*this);
    iHostEventWatcher = CUSBHostEventNotifWatcher::NewL(*this);
    TInt ret = iPeripheral.Attach(KPSUidUsbWatcher, KUsbWatcherIsPeripheralConnected);
    if (ret != KErrNone)
        {
        FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgHostStateWatcher::ConstructL - iPeripheral.Attach err=%d"), ret));
        User::Leave(ret);
        }    
    }


// --------------------------------------------------------------------------
// Two-phased constructor.
// --------------------------------------------------------------------------
CUSBOtgHostStateWatcher::CUSBOtgIdPinStateWatcher* 
CUSBOtgHostStateWatcher::CUSBOtgIdPinStateWatcher::NewL(CUSBOtgHostStateWatcher& aOtgHostStateWatcher)
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::NewL"));

    CUSBOtgIdPinStateWatcher* self = new(ELeave)CUSBOtgIdPinStateWatcher(aOtgHostStateWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// --------------------------------------------------------------------------
// C++ destructor
// --------------------------------------------------------------------------
CUSBOtgHostStateWatcher::CUSBOtgIdPinStateWatcher::~CUSBOtgIdPinStateWatcher()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::~CUSBOtgIdPinStateWatcher"));

    Cancel();    
    iIdPin.Close();
    }

// --------------------------------------------------------------------------
// Get Id pin state
// --------------------------------------------------------------------------
TInt CUSBOtgHostStateWatcher::CUSBOtgIdPinStateWatcher::IsIdPinOn(TBool& aIsIdPinOn)
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::IsIdPinOn" ));

    TInt val(0);
    TInt err = iIdPin.Get(val);

    if (KErrNone != err)
        {
        FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::IsIdPinOn - err=%d"), err));
        return err;
        }

    FTRACE(FPrint(_L( "[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::IsIdPinOn=%d" ), val ));

    // not found in docs clear definition of this property. Verification is needed   
    aIsIdPinOn = (0 == val ? EFalse : ETrue);
    
    return KErrNone;
    }

// --------------------------------------------------------------------------
// The Id pin state has changed.
// --------------------------------------------------------------------------
void CUSBOtgHostStateWatcher::CUSBOtgIdPinStateWatcher::RunL()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::RunL"));

    if (iStatus.Int() != KErrNone)
        {
        FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::RunL - iStatus=%d"), iStatus.Int()));
        User::Leave(iStatus.Int());
        }    

    // re-issue request first
    iIdPin.Subscribe(iStatus);
    SetActive();

    TBool isIdPinOn;
    TInt ret = IsIdPinOn(isIdPinOn);
    if (ret != KErrNone)
        {
        FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::RunL - IsIdPinOn=%d"), ret));
        User::Leave(ret);
        }    

    if (isIdPinOn)
        {
        FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::RunL IdPin ON"));

        iOtgHostStateWatcher.iObserver.OtgHostIdPinStateChanged(ETrue);
        }
    else
        {
        FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::RunL IdPin OFF"));

        iOtgHostStateWatcher.iObserver.OtgHostIdPinStateChanged(EFalse);
        }
    }
 
// ----------------------------------------------------------------------------
// Standard active object error function.
// ----------------------------------------------------------------------------
TInt CUSBOtgHostStateWatcher::CUSBOtgIdPinStateWatcher::RunError(TInt aError)
    {
    FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::RunL"), aError));

    // Currently no leaving functions called in RunL, thus nothing should cause
    // this to be called -> return.
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Standard active object cancellation function.
// ----------------------------------------------------------------------------
void CUSBOtgHostStateWatcher::CUSBOtgIdPinStateWatcher::DoCancel()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::DoCancel"));
    
    iIdPin.Cancel();
    }

// ----------------------------------------------------------------------------
// C++ constructor
// ----------------------------------------------------------------------------
CUSBOtgHostStateWatcher::CUSBOtgIdPinStateWatcher::CUSBOtgIdPinStateWatcher(
        CUSBOtgHostStateWatcher& aOtgHostStateWatcher)
    : CActive(EPriorityStandard), iOtgHostStateWatcher(aOtgHostStateWatcher)
    {
    CActiveScheduler::Add(this);
    }

// --------------------------------------------------------------------------
// Symbian 2nd phase constructor. 
// --------------------------------------------------------------------------
void CUSBOtgHostStateWatcher::CUSBOtgIdPinStateWatcher::ConstructL()
    {
    TInt ret = iIdPin.Attach(KUidUsbManCategory, KUsbOtgIdPinPresentProperty);
    if (ret != KErrNone)
        {
        FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBOtgIdPinStateWatcher::ConstructL - iIdPin.Attach error:%d"), ret));
        User::Leave(ret);
        }
    
    // Issue request first
    iIdPin.Subscribe(iStatus);
    SetActive();
    }

// --------------------------------------------------------------------------
// Two-phased constructor.
// --------------------------------------------------------------------------
CUSBOtgHostStateWatcher::CUSBHostEventNotifWatcher* 
CUSBOtgHostStateWatcher::CUSBHostEventNotifWatcher::NewL(CUSBOtgHostStateWatcher& aOtgHostStateWatcher)
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBHostEventNotifWatcher::NewL"));

    CUSBHostEventNotifWatcher* self = new(ELeave)CUSBHostEventNotifWatcher(aOtgHostStateWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// --------------------------------------------------------------------------
// C++ destructor
// --------------------------------------------------------------------------
CUSBOtgHostStateWatcher::CUSBHostEventNotifWatcher::~CUSBHostEventNotifWatcher()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBHostEventNotifWatcher::~CUSBHostEventNotifWatcher"));

    Cancel();    
    }

// --------------------------------------------------------------------------
// The Id pin state has changed.
// --------------------------------------------------------------------------
void CUSBOtgHostStateWatcher::CUSBHostEventNotifWatcher::RunL()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBHostEventNotifWatcher::RunL"));

    if (iStatus.Int() != KErrNone)
        {
        FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBHostEventNotifWatcher::RunL - iStatus=%d"), iStatus.Int()));
        User::Leave(iStatus.Int());
        }    

    // Save current event
    TDeviceEventInformation dei = iEventInfo;

    // Re-issue request first
    iOtgHostStateWatcher.iUsbMan.HostEventNotification(iStatus, iEventInfo);
    SetActive();
   
    iOtgHostStateWatcher.iObserver.HostEventNotify(dei);    
    }
 
// ----------------------------------------------------------------------------
// Standard active object error function.
// ----------------------------------------------------------------------------
TInt CUSBOtgHostStateWatcher::CUSBHostEventNotifWatcher::RunError(TInt aError)
    {
    FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBHostEventNotifWatcher::RunL"), aError));

    // Currently no leaving functions called in RunL, thus nothing should cause
    // this to be called -> return.
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Standard active object cancellation function.
// ----------------------------------------------------------------------------
void CUSBOtgHostStateWatcher::CUSBHostEventNotifWatcher::DoCancel()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBHostEventNotifWatcher::DoCancel"));
    
    iOtgHostStateWatcher.iUsbMan.HostEventNotificationCancel();
    }

// ----------------------------------------------------------------------------
// C++ constructor
// ----------------------------------------------------------------------------
CUSBOtgHostStateWatcher::CUSBHostEventNotifWatcher::CUSBHostEventNotifWatcher(
        CUSBOtgHostStateWatcher& aOtgHostStateWatcher)
    : CActive(EPriorityStandard), iOtgHostStateWatcher(aOtgHostStateWatcher)
    {
    CActiveScheduler::Add(this);
    }

// --------------------------------------------------------------------------
// Symbian 2nd phase constructor. 
// --------------------------------------------------------------------------
void CUSBOtgHostStateWatcher::CUSBHostEventNotifWatcher::ConstructL()
    {
    iOtgHostStateWatcher.iUsbMan.HostEventNotification(iStatus, iEventInfo);
    SetActive();
    }
