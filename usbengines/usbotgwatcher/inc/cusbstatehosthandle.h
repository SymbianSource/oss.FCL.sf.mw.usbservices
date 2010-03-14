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
* Description:  Implements concrete state
 *
*/


#ifndef C_USBSTATEHOSTHANDLE_H
#define C_USBSTATEHOSTHANDLE_H

#include "cusbotgwatcher.h"
#include "cusbstate.h"

#ifndef STIF
#include "cusbnotifmanager.h"
#include "cusbtimer.h"
#else
#include "mockcusbnotifmanager.h"
#include "mockcusbtimer.h"
#endif

/**
 *  This class implements behaviour when some problem needs vbus drop, and mostly, user action.
 *
 */
NONSHARABLE_CLASS( CUsbStateHostHandle ) : public CUsbState,
        MWaitNotifierObserver 
// this means only one wait notifier at a time can be shown by this state, might be redesigned
    {
    friend class CtUsbOtgWatcher;
    friend class CUsbOtgWatcher;

public:

    /**
     * Two-phased constructor.
     * @param aWatcher owner
     */
    static CUsbStateHostHandle* NewL(CUsbOtgWatcher* aWatcher);

    /**
     * Destructor.
     */
    virtual ~CUsbStateHostHandle();

    /**
     * Sets id of the issue to be handled
     * @param aWhat issue id
     */
    void SetWhat(TInt aWhat);

    // from MWaitNotifierObserver
    /**
     * Callback when notifier is completed
     * @param aFeedback how notifier completed
     */
    void WaitNotifierCompletedL(TInt aFeedback);

    // From VBus observer
    /**
     * VBus down
     */
    virtual void VBusUpL();
    /**
     * VBus down
     */
    virtual void VBusDownL();
    /**
     * VBus error happened
     */
    virtual void VBusErrorL();

    // From OTG state observer
    /**
     * Became to Idle when A
     */
    virtual void AIdleL();
    /**
     * Became a Host when A
     */
    virtual void AHostL();
    /**
     * Became a peripheral when A
     */
    virtual void APeripheralL();
    /**
     * VBus error happen
     */
    virtual void AVBusErrorL();

    // From bus activity observer
    /**
     * Bus is in idle
     */
    virtual void BusIdleL();
    /**
     * Bus active
     */
    virtual void BusActiveL();

    // From Host Event notification observer
    /**
     * Device is attached
     * @param aInfo Device event data
     */
    void DeviceAttachedL(TDeviceEventInformation aInfo);
    /**
     * Device is detached
     * @param aInfo Device event data
     */
    void DeviceDetachedL(TDeviceEventInformation aInfo);
    
    /**
      * Drivers successfully loaded
      * @param aInfo Device event data
      */
    virtual void DriverLoadSuccessL(TDeviceEventInformation aInfo);
     /**
      * Drivers loaded partially
      * @param aInfo Device event data
      */
    virtual void DriverLoadPartialSuccessL(TDeviceEventInformation aInfo);
     /**
      * Drivers loading failed
      * @param aInfo Device event data
      */
    virtual void DriverLoadFailureL(TDeviceEventInformation aInfo);

    // From message notification observer
    /**
     * Message received
     * @param aMessage message id
     */
    virtual void MessageNotificationReceivedL(TInt aMessage);

    /**
     * SRP request received
     */
    virtual void SrpReceivedL();
    /**
     * Session request received
     */
    virtual void SessionRequestedL();

    //from CUsbState
    /**
     * State id
     * @return state id
     */
    virtual TUsbStateIds Id();

    /**
     * This is called when switched to this state, 
     * because If all conditions for transition to another state exist, nothing will 
     * trigger it and transition will not happen. This forces the transition in such cases.
     *
     */
    virtual void JustAdvancedToThisStateL();

    /**
     * This is called when leaving this state, 
     *
     */
    virtual void JustBeforeLeavingThisStateL();

    /**
     * handles issue
     */
    virtual void DoHandleL();

protected:

    /**
     * Default constructor
     * @param aWatcher owner
     */
    CUsbStateHostHandle(CUsbOtgWatcher* aWatcher);

    /**
     * 2nd phase construction
     */
    void ConstructL();

protected:
    // data

    /**
     * identifies situation to be handled
     */
    TInt iWhat;

    };

#endif //  C_USBSTATEHOSTHANDLE_H
