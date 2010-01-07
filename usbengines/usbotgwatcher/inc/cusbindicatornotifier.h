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
#include "cusbvbusobserver.h"
#include "cusbotgwatcher.h"


/**
 * Class implements functionality of showing/blinking usb indicator
 * Class does not provide method to get response from the user
 */
NONSHARABLE_CLASS( CUsbIndicatorNotifier ): public CUsbNotifier, MUsbTimerObserver, MUsbOtgWatcherStateObserver, MUsbVBusObserver
    {
public:
    /**
     * Two-phased constructor.
     * @param aNotifManager parent
     * @param aOtgWatcher provides VBus observer and usb otg watcher state notifications
     * @return Pointer to the new instance of CUsbIndicatorNotifier
     */
    static CUsbIndicatorNotifier* NewL(CUsbNotifManager* aNotifManager, CUsbOtgWatcher* aOtgWatcher);

    /**
     * Destructor.
     */
    virtual ~CUsbIndicatorNotifier();

    // From MUsbOtgWatcherStateObserver
    /**
     * Otg Watcher state changed call back
     * @param aState new state
     */
    virtual void OtgWatcherStateChangedL(TUsbStateIds aState);
    
    // From MUsbVBusObserver
    /**
     * VBus down event received
     */
    virtual void VBusDownL();
    /**
     * VBus up event received
     */
    virtual void VBusUpL();
    /**
     * error handler
     * @param aError error code
     */   
    virtual void VBusObserverErrorL(TInt aError);
    
    
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
     * @param aNotifManger parent
     * @param aOtgWatcher Otg watcher
     */
    CUsbIndicatorNotifier(CUsbNotifManager* aNotifManager, CUsbOtgWatcher* aOtgWatcher);

    /**
     * 2nd phase constructor.
     */
    void ConstructL();

    /**
     * Set USB indicator On or Off
     * @param aState Indicator states 
     */
    void SetIndicatorStateL(const TInt aState);
    
    /**
     * Show/hide static icon of the indicator. 
     * If the indicator is blinking, stop blinking it and show/hide the static 
     * form of the indicator.
     * @param aVisible ETrue - Show the indicator, EFalse - Hide the indicator 
     */
    void ShowStaticL(TBool aVisible);
    
    /**
     * Blinks indicator
     */
    void BlinkL();
    
    /**
     * Sets indicator accordingly
     */
    void SetIndicatorL();
    
private:
    // data

    /**
     * OtgWatcher
     * not own
     */
    CUsbOtgWatcher* iOtgWatcher;
    
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
