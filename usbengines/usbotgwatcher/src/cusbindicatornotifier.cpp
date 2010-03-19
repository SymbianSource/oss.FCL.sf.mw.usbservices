/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <AknSmallIndicator.h>
#include <avkon.hrh>
#include <usbuinotif.h>

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
    delete iIconBlinkingTimer;

    if (iOtgWatcher.VBusObserver())
        {
        TRAP_IGNORE(iOtgWatcher.VBusObserver()->UnsubscribeL(*this));
        }

    // Unsubscribe from otg watcher states change notifications
    TRAP_IGNORE(iOtgWatcher.UnsubscribeL(*this));
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

    //To be changed to EAknIndicatorStateAnimate and remove iIconBlinkingTimer
    //when AVKON implements animation form of usb indicator.
    iIndicatorState = EAknIndicatorStateOn;
    }

// ---------------------------------------------------------------------------
// Second-phase constructor
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::ConstructL()
    {
    LOG_FUNC

    // Subscribe for VBus change notifications
    iOtgWatcher.VBusObserver()->SubscribeL(*this);

    // Subscribe for otg watcher states change notifications
    iOtgWatcher.SubscribeL(*this);

    iIconBlinkingTimer = CUsbTimer::NewL(*this, EIconBlinkingTimer);

    // check here for condition to set usb indicator
    SetIndicatorL();

    }

// ---------------------------------------------------------------------------
// Show/hide static icon of the indicator. 
// If the indicator is blinking, stop blinking it and show/hide the static 
// form of the indicator.
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::ShowStaticL(TBool aVisible)
    {
    LOG_FUNC

    LOG1("aVisible = %d" , aVisible);

    iIconBlinkingTimer->Cancel();

    SetIndicatorStateL(aVisible
                                ? EAknIndicatorStateOn
                                   : EAknIndicatorStateOff);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::BlinkL()
    {
    LOG_FUNC

    // Will be canceled if active in After()
    iIconBlinkingTimer->After(0);
    }

// ---------------------------------------------------------------------------
// From base class CUsbNotifier
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::ShowL()
    {
    LOG_FUNC

    ShowStaticL(ETrue);
    }

// ---------------------------------------------------------------------------
// From CUsbNotifier
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::Close()
    {
    LOG_FUNC

    iIconBlinkingTimer->Cancel();
    TRAP_IGNORE( ShowStaticL(EFalse) );
    }

// ---------------------------------------------------------------------------
// From MUsbTimerObserver
// ---------------------------------------------------------------------------
// 
void CUsbIndicatorNotifier::TimerElapsedL(TUsbTimerId aTimerId)
    {
    switch (aTimerId)
        {
        case EIconBlinkingTimer:
            {
            SetIndicatorStateL(iIndicatorState
                                               ? EAknIndicatorStateOn
                                                  : EAknIndicatorStateOff);

            iIndicatorState
                    = (iIndicatorState == EAknIndicatorStateOn
                                                               ? EAknIndicatorStateOff
                                                                  : EAknIndicatorStateOn);

            // Will be canceled if active in After()
            iIconBlinkingTimer->After(KUsbIndicatorBlinkingInterval);
            break;
            }
        default:
            {
            LOG1("Unknown timer id = %d", aTimerId)
            Panic(EWrongTimerId);
            }
        }
    }

// ---------------------------------------------------------------------------
// Set USB indicator On or Off
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::SetIndicatorStateL(const TInt aState)
    {

    LOG1( "USB indicator State = %d" , aState);

    CAknSmallIndicator* indicator = CAknSmallIndicator::NewLC(TUid::Uid(
            EAknIndicatorUSBConnection));
    indicator->SetIndicatorStateL(aState);
    CleanupStack::PopAndDestroy(indicator); //indicator    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::OtgWatcherStateChangedL(TUsbStateIds aState)
    {
    SetIndicatorL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::VBusDownL()
    {
    SetIndicatorL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::VBusUpL()
    {
    SetIndicatorL();
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
void CUsbIndicatorNotifier::SetIndicatorL()
    {
    if (!(iOtgWatcher.IsDeviceA()))
        {
        // if B, than other party (usbwatcher) takes care of usb indicator
        // in combined usbwatcher (if role swap allowed) one class has to manage usb indicator 
        return;
        }

    // if VBus Up and we are host -> show indicator
    if ((iOtgWatcher.VBusObserver()->VBus() == CUsbVBusObserver::EVBusUp)
            && (iOtgWatcher.CurrentHostState()->Id() == EUsbStateHostAHost))
        {
        ShowStaticL(ETrue);
        }
    // if VBus up and we are not host -> Blink indicator
    else if ((iOtgWatcher.VBusObserver()->VBus() == CUsbVBusObserver::EVBusUp)
            && (iOtgWatcher.CurrentHostState()->Id() != EUsbStateHostAHost))
        {
        BlinkL();
        }
    else
    // Otherwise do not show indicator
        {
        ShowStaticL(EFalse);
        }
    }

// End of file
