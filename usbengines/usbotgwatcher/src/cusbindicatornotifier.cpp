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
CUsbIndicatorNotifier* CUsbIndicatorNotifier::NewL(CUsbNotifManager* aNotifManager, CUsbOtgWatcher* aOtgWatcher)
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::NewL" ));

    CUsbIndicatorNotifier* self = new (ELeave) CUsbIndicatorNotifier(aNotifManager, aOtgWatcher);
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
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::~CUsbIndicatorNotifier" ) );
        
    Close();
    delete iIconBlinkingTimer;
    
    // Unsubscribe from VBus change notifications
    iOtgWatcher->VBusObserver()->UnsubscribeL(this);
    
    // Unsubscribe from otg watcher states change notifications
    iOtgWatcher->UnsubscribeL(this);
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CUsbIndicatorNotifier::CUsbIndicatorNotifier(CUsbNotifManager* aNotifManager, CUsbOtgWatcher* aOtgWatcher) :
    CUsbNotifier(aNotifManager, KUsbUiNotifOtgIndicator, NULL), iOtgWatcher(aOtgWatcher)
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::CUsbIndicatorNotifier" ) );

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
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::ConstructL" ) );
    
    // Subscribe for VBus change notifications
    iOtgWatcher->VBusObserver()->SubscribeL(this);
    
    // Subscribe for otg watcher states change notifications
    iOtgWatcher->SubscribeL(this);

    iIconBlinkingTimer = CUsbTimer::NewL(this, EIconBlinkingTimer);
    
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
    FTRACE( FPrint (_L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::ShowStaticL, aVisible=%d" ), aVisible));

    iIconBlinkingTimer->Cancel();

    SetIndicatorStateL(aVisible ? EAknIndicatorStateOn : EAknIndicatorStateOff);    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::BlinkL()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::BlinkL" ));

    // Will be canceled if active in After()
    iIconBlinkingTimer->After(0);
    }

// ---------------------------------------------------------------------------
// From base class CUsbNotifier
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::ShowL()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::ShowL" ) );
    
    ShowStaticL(ETrue);
    }

// ---------------------------------------------------------------------------
// From CUsbNotifier
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::Close()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::Close" ) );
    
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
            SetIndicatorStateL(iIndicatorState ? EAknIndicatorStateOn : EAknIndicatorStateOff);    

            iIndicatorState = (iIndicatorState == EAknIndicatorStateOn
                                    ? EAknIndicatorStateOff : EAknIndicatorStateOn);

            // Will be canceled if active in After()
            iIconBlinkingTimer->After(KUsbIndicatorBlinkingInterval);
            break;
            }
        default:
            {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::TimerElapsedL - Unknown timer" ) );
            
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
    FTRACE( FPrint (_L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::SetIndicatorStateL - aState=%d" ), aState));

    CAknSmallIndicator* indicator = CAknSmallIndicator::NewLC(TUid::Uid(EAknIndicatorUSBConnection));
    indicator->SetIndicatorStateL( aState );
    CleanupStack::PopAndDestroy( indicator ); //indicator    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::OtgWatcherStateChangedL(TUsbStateIds aState)
    {
    FTRACE( FPrint (_L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::OtgWatcherStateChangedL - aState=%d" ), aState));
    SetIndicatorL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::VBusDownL()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::VBusDownL" ) );
    SetIndicatorL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::VBusUpL()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::VBusUpL" ) );
    SetIndicatorL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//  
void CUsbIndicatorNotifier::VBusObserverErrorL(TInt aError)
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::VBusObserverErrorL" ) );
    // do not care
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//  
void CUsbIndicatorNotifier::SetIndicatorL()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::SetIndicatorL" ) );
    // if VBus Up and we are host -> show indicator
    if ((iOtgWatcher->VBusObserver()->VBus() == CUsbVBusObserver::EVBusUp) &&
            (iOtgWatcher->CurrentHostState()->Id() == EUsbStateHostAHost))
        {
        ShowStaticL(ETrue);
        }
    // if VBus up and we are not host -> Blink indicator
    else if((iOtgWatcher->VBusObserver()->VBus() == CUsbVBusObserver::EVBusUp) &&
            (iOtgWatcher->CurrentHostState()->Id() != EUsbStateHostAHost))
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
