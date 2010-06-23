/*
 * Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Blink/show Usb indicator notifier implementation
 *
 */

#include <usbuinotif.h>
#include <hb/hbcore/hbindicatorsymbian.h>

#include "cusbindicatornotifier.h"
#include "cusbstate.h"
#include "definitions.h"

#include "debug.h"
#include "panic.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUsbIndicatorNotifier* CUsbIndicatorNotifier::NewL(
        CUsbNotifManager& aNotifManager, CUsbOtgWatcher& aOtgWatcher)
    {
    LOG_FUNC

    CUsbIndicatorNotifier* self = new (ELeave) CUsbIndicatorNotifier(
            aNotifManager, aOtgWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CUsbIndicatorNotifier::~CUsbIndicatorNotifier()
    {
    LOG_FUNC

    Close();

    if (iOtgWatcher.VBusObserver())
        {
        TRAP_IGNORE(iOtgWatcher.VBusObserver()->UnsubscribeL(*this));
        }

    // Unsubscribe from otg watcher states change notifications
    TRAP_IGNORE(iOtgWatcher.UnsubscribeL(*this));

    delete iUsbConnectingIndicator; 
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CUsbIndicatorNotifier::CUsbIndicatorNotifier(CUsbNotifManager& aNotifManager,
        CUsbOtgWatcher& aOtgWatcher) :
    CUsbNotifier(aNotifManager, KUsbUiNotifOtgIndicator, NULL), iOtgWatcher(
            aOtgWatcher)
    {
    LOG_FUNC
    
    }

// ---------------------------------------------------------------------------
// Second-phase constructor
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::ConstructL()
    {
    LOG_FUNC

    iUsbConnectingIndicator = CHbIndicatorSymbian::NewL();
    
    // Subscribe for VBus change notifications
    iOtgWatcher.VBusObserver()->SubscribeL(*this);

    // Subscribe for otg watcher states change notifications
    iOtgWatcher.SubscribeL(*this);

    // check here for condition to set usb indicator
    SetIndicator();
    }

// ---------------------------------------------------------------------------
// Show/hide static icon of the indicator. 
// If the indicator is blinking, stop blinking it and show/hide the static 
// form of the indicator.
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::ShowStatic(TBool aVisible)
    {
    LOG_FUNC

    LOG1("aVisible = %d" , aVisible);

    SetIndicatorState(aVisible
                                ? EIndicatorStateOn
                                   : EIndicatorStateOff);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::Blink()
    {
    LOG_FUNC

    SetIndicatorState( EIndicatorConnecting );
    }

// ---------------------------------------------------------------------------
// From base class CUsbNotifier
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::ShowL()
    {
    LOG_FUNC

    ShowStatic(ETrue);
    }

// ---------------------------------------------------------------------------
// From CUsbNotifier
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::Close()
    {
    LOG_FUNC

    ShowStatic(EFalse);
    }

// ---------------------------------------------------------------------------
// Set USB indicator On or Off
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::SetIndicatorState(const TInt aState)
    {
    LOG1( "USB indicator State = %d" , aState);
    
    TBool success = ETrue;
    
    if ((aState == EIndicatorConnecting) && !iConnectingIndicatorOn)
        {
        success = iUsbConnectingIndicator->Activate(KUsbConnectingIndicator);   
        LOG1( "calling CHbIndicatorSymbian::Activate(), returned %d", success);   
        if (success)
            {
            iConnectingIndicatorOn = ETrue;
            }  
                   
        }
   
    if ((aState != EIndicatorConnecting) && iConnectingIndicatorOn)
        {
        success = iUsbConnectingIndicator->Deactivate(KUsbConnectingIndicator); 
        if (success)
            {
            iConnectingIndicatorOn = EFalse;
            }  
        LOG1( "calling CHbIndicatorSymbian::Deactivate(), returned %d", success);                  
        }   
        
    // if success became false loading the indicator failed, log the error    
    if (!success)
        {
        TInt error = iUsbConnectingIndicator->Error();
        LOG1( "indicator error is %d", error);       
        }         
    }
       
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::OtgWatcherStateChangedL(TUsbStateIds aState)
    {
    SetIndicator();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::VBusDownL()
    {
    SetIndicator();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::VBusUpL()
    {
    SetIndicator();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//  
void CUsbIndicatorNotifier::VBusObserverErrorL(TInt aError)
    {
    // do nothing
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//  
void CUsbIndicatorNotifier::SetIndicator()
    {
    if (!(iOtgWatcher.IsDeviceA()) || iOtgWatcher.CurrentHostState()->Id() == EUsbStateHostAPeripheral)
        {
        // if B or peripheral, than other party (usbwatcher) takes care of usb indicator
        // in combined usbwatcher (if role swap allowed) one class has to manage usb indicator 
        return;
        }

    // if VBus Up and we are host -> show indicator
    if ((iOtgWatcher.VBusObserver()->VBus() == CUsbVBusObserver::EVBusUp)
            && (iOtgWatcher.CurrentHostState()->Id() == EUsbStateHostAHost))
        {
        ShowStatic(ETrue);
        }
    // if VBus up and we are not host -> Blink indicator
    else if ((iOtgWatcher.VBusObserver()->VBus() == CUsbVBusObserver::EVBusUp)
            && (iOtgWatcher.CurrentHostState()->Id() != EUsbStateHostAHost))
        {
        Blink();
        }
    else
    // Otherwise do not show indicator
        {
        ShowStatic(EFalse);
        }
    }

// End of file
