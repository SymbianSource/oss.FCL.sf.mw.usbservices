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
* Description:  Manages notifiers
 *
*/


#ifndef C_USBNOTIFMANAGER_H
#define C_USBNOTIFMANAGER_H

#include <e32base.h>

// forward declaration
class CUsbNotifier;
class CUsbIndicatorNotifier;

// pause warning is visible for (ms)
const TUint KPauseForWarnings = 1000000; // 1 sec

/**
 * Observer has to implement this interface to get feedback on user action
 * Notification received when Whait notified completed (cancelled ot key pressed) 
 */
NONSHARABLE_CLASS( MWaitNotifierObserver)
    {
public:

    /**
     * feedback to observer
     * @param aFeedback how notifier comleted 
     */
    virtual void WaitNotifierCompletedL(TInt aFeedback) = 0;
    };

/**
 * this class contains information for wait notifier
 * once notifier gets response from user, notif manager will send feedback data to caller
 */
NONSHARABLE_CLASS( CWaitNotifierInfo ) : public CBase
    {
public:

    /**
     * 2-phase construction
     * @param aWaitNotifier owner
     * aObserver will receive notification when completed 
     */
    static CWaitNotifierInfo* NewL(CUsbNotifier* aWaitNotifier,
            MWaitNotifierObserver* aObserver);

    /**
     * Destructor
     */
    virtual ~CWaitNotifierInfo();

    /**
     * gets wait notifier
     */
    CUsbNotifier* WaitNotifier() const;

    /**
     * gets observer
     */
    MWaitNotifierObserver* Observer() const;

private:

    /**
     * Default constructor
     * @param aWaitNotifier Notifier
     * @param aObserver Observer
     */
    CWaitNotifierInfo(CUsbNotifier* aWaitNotifier,
            MWaitNotifierObserver* aObserver);

    /**
     * 2nd phase construction
     */
    void ConstructL();

private:

    /**
     * notifier
     * owns
     */
    CUsbNotifier* iWaitNotifier;

    /**
     * observer
     * not owns
     */
    MWaitNotifierObserver* iObserver;
    };

/**
 *  Function of the class is own and share RNotifier, own list of CWaitNotifier(s),
 *	show notifiers, which are synchronous, and cancel it with a delay 
 */
class CUsbNotifManager : public CBase
    {
    friend class CUsbNotifManager;

public:

    /**
     * 2phase construction
     */
    static CUsbNotifManager* NewL();

    /**
     * Destructor
     */
    virtual ~CUsbNotifManager();

    /**
     * Shows notifier
     * @param aCat Category
     * @param aNotifId notifier id
     * @param aObserver will receive notifications
     */
    void ShowNotifierL(TUid aCat, TUint aNotifId,
            MWaitNotifierObserver* aObserver = NULL);

    /**
     * Changes USB indicator to blink or not
     * @param aBlinking ETrue if want to make it blinking, EFalse otherwise
     */
    void BlinkIndicatorL(TBool aBlinking);

    /**
     * Shows or hides usb indicator
     * @param aVisible ETRue is show, EFalse id hide 
     */
    void ShowIndicatorL(TBool aVisible);

    /**
     * Closes and deletes all the notifiers
     */
    void CloseAllNotifiers();
    
    /** 
     * Calls back Notifier show is over
     * @param aWaitNotifier wait notifier
     * @param aResult result (was any errors or not)
     * @param aFeedback user press (cancel or any other) 
     */
    void NotifierShowCompletedL(CUsbNotifier* aWaitNotifier, TInt aResult,
            TInt aFeedback);

private:

    /**
     * Default construction
     */
    CUsbNotifManager();

    /**
     * Second phase construction
     */
    void ConstructL();

private:
    // data

    /**
     * RNotifier API
     */
    RNotifier iNotifier;

    /**
     * List of notifiers waiting for user action
     */
    RPointerArray<CWaitNotifierInfo> iWaitNotifiers;

    /**
     * Own
     * Notifier for showing or blinking Usb indicator
     */
    CUsbIndicatorNotifier* iIndicatorNotifier;
    };

#endif // C_USBNOTIFMANAGER_H
