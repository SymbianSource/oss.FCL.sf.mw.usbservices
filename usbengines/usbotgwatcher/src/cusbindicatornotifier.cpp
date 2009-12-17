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
#include "cusbnotifier.h"
#include "definitions.h"

#include "debug.h"
#include "panic.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUsbIndicatorNotifier* CUsbIndicatorNotifier::NewL(CUsbNotifManager* aNotifManager)
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::NewL" ));

    CUsbIndicatorNotifier* self = new (ELeave) CUsbIndicatorNotifier(aNotifManager);
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
    }

// ---------------------------------------------------------------------------
// Show/hide static icon of the indicator. 
// If the indicator is blinking, stop blinking it and show/hide the static 
// form of the indicator.
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::ShowIndicatorL(TBool aVisible)
    {
    FTRACE( FPrint (_L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::ShowIndicator, aVisible=%d" ), aVisible));

    iIconBlinkingTimer->Cancel();

    SetIndicatorStateL(aVisible ? EAknIndicatorStateOn : EAknIndicatorStateOff);    
    }

// ---------------------------------------------------------------------------
// From CUsbNotifier
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::ShowL()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::ShowL" ));

    // Will be canceled if active in After()
    iIconBlinkingTimer->After(0);
    }

// ---------------------------------------------------------------------------
// From CUsbNotifier
// ---------------------------------------------------------------------------
//
void CUsbIndicatorNotifier::Close()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbIndicatorNotifier::Close" ) );
    
    iIconBlinkingTimer->Cancel();
    TRAP_IGNORE( ShowIndicatorL(EFalse) );
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
// C++ constructor
// ---------------------------------------------------------------------------
//
CUsbIndicatorNotifier::CUsbIndicatorNotifier(CUsbNotifManager* aNotifManager) :
    CUsbNotifier(aNotifManager, KUsbUiNotifOtgIndicator, NULL)
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

    iIconBlinkingTimer = CUsbTimer::NewL(this, EIconBlinkingTimer);
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

// End of file
