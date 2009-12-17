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


#ifndef C_USBINDICATORNOTIFIER_H
#define C_USBINDICATORNOTIFIER_H

#include <e32base.h>
#include <AknNotifyStd.h>       // SAknSmallIndicatorParams
#include <AknNotifySignature.h> // SAknNotifierPackage
#include <avkon.hrh>            // EAknIndicatorUSBConnection
#include "cusbtimer.h"
#include "cusbnotifier.h"


/**
 * Class implements functionality of showing/blinking usb indicator
 * Class does not provide method to get response from the user
 */
NONSHARABLE_CLASS( CUsbIndicatorNotifier ): public CUsbNotifier, MUsbTimerObserver
    {
public:
    /**
     * Two-phased constructor.
     * @param aNotifManager Owner of the class, will destroy the object when needed
     * @return Pointer to the new instance of CUsbIndicatorNotifier
     */
    static CUsbIndicatorNotifier* NewL(CUsbNotifManager* aNotifManager);

    /**
     * Destructor.
     */
    virtual ~CUsbIndicatorNotifier();

    /**
     * Show/hide static icon of the indicator. 
     * If the indicator is blinking, stop blinking it and show/hide the static 
     * form of the indicator.
     * @param aVisible ETrue - Show the indicator, EFalse - Hide the indicator 
     */
    void ShowIndicatorL(TBool aVisible);

    // From base class CUsbNotifier
    /**
     * Start to show notifier
     */
    virtual void ShowL();

    /**
     * Stop showing notifier
     */
    virtual void Close();

    // From MUsbTimerObserver
    /**
     * Timer elapsed
     * @param Timer id
     */
    void TimerElapsedL(TUsbTimerId aTimerId);

private:

    /**
     * Default constructor.
     * @param aNotifManager Owner
     */
    CUsbIndicatorNotifier(CUsbNotifManager* aNotifManager);

    /**
     * 2nd phase constructor.
     */
    void ConstructL();

    /**
     * Set USB indicator On or Off
     * @param aState Indicator states 
     */
    void SetIndicatorStateL(const TInt aState);
    
private:
    // data

    /**
     * Switch timer for showing/hiding the usb indicator interleavingly for 
     * animating the indicator. 
     * Own
     */
    CUsbTimer* iIconBlinkingTimer;

    /**
     * Current indicator state
     */
    TInt iIndicatorState;

    };

#endif // C_USBINDICATORNOTIFIER_H
