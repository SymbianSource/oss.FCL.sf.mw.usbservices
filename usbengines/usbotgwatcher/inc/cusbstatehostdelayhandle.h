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


#ifndef C_USBSTATEHOSTDALAYHANDLE_H
#define C_USBSTATEHOSTDELAYHANDLE_H

#include "cusbotgwatcher.h"
#include "cusbstatehosthandle.h"

#ifndef STIF
#include "cusbnotifmanager.h"
#include "cusbtimer.h"
#else
#include "mockcusbnotifmanager.h"
#include "mockcusbtimer.h"
#endif

/**
 *  This class implements behaviour when some problem needs vbus drop in a while, and mostly, user action.
 *
 */
NONSHARABLE_CLASS( CUsbStateHostDelayHandle ) : public CUsbStateHostHandle,
      //  MWaitNotifierObserver,
        MUsbTimerObserver
// this means only one wait notifier at a time can be shown by this state, might be redesigned
    {
    friend class CtUsbOtgWatcher;
    friend class CUsbOtgWatcher;

public:

    /**
     * Two-phased constructor.
     * @param aWatcher owner
     */
    static CUsbStateHostDelayHandle* NewL(CUsbOtgWatcher* aWatcher);

    /**
     * Destructor.
     */
    virtual ~CUsbStateHostDelayHandle();

    // From MUsbTimerObserver
    /**
     * Call back when timer expired
     * @param aTimerId timer id
     */
    void TimerElapsedL(TUsbTimerId aTimerId);

    // From VBus observer
    /**
     * VBus down
     */
    void VBusUpL();
    /**
     * VBus down
     */
    void VBusDownL();
    /**
     * VBus error happened
     */
    void VBusErrorL();

    // From OTG state observer
    /**
     * VBus error happen
     */
    void AVBusErrorL();

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
     * Connected to hub in wrong level 
     */
    void BadHubPositionL();

    /**
     * SRP request received
     */
    void SrpReceivedL();

//from CUsbState
    /**
     * State id
     * @return state id
     */
    TUsbStateIds Id();

    /**
     * This is called when switched to this state, 
     * because If all conditions for transition to another state exist, nothing will 
     * trigger it and transition will not happen. This forces the transition in such cases.
     *
     */
    void JustAdvancedToThisStateL();

    /**
     * This is called when leaving this state, 
     *
     */
    void JustBeforeLeavingThisStateL();
    
    /**
     * handles issue
     */
    void DoHandleL();


private:

    /**
     * Default constructor
     * @param aWatcher owner
     */
    CUsbStateHostDelayHandle(CUsbOtgWatcher* aWatcher);

    /**
     * 2nd phase construction
     */
    void ConstructL();

    
private:
    // data

    /** 
     * too much power timer
     * own
     */
    CUsbTimer* iTooMuchPowerTimer;
    
    /** 
     * drivers not found
     * own
     */
    CUsbTimer* iDriversNotFoundTimer;
    };

#endif //  C_USBSTATEHOSTHANDLE_H
