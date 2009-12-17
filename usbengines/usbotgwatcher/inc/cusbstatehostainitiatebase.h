/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef C_USBSTATEHOSTAINITIATEBASE_H
#define C_USBSTATEHOSTAINITIATEBASE_H

#include "cusbotgwatcher.h"
#include "cusbstatehostabase.h"
#include "cusbpersonalityswitch.h"
#ifndef STIF
#include "cusbtimer.h"
#else
#include "mockcusbtimer.h"
#endif

/**
 *  This class implements besic behaviour when Id pin on, and vbus just turned down or just turned on,
 *  specific concrete case/state is implemented in subclass
 */
NONSHARABLE_CLASS( CUsbStateHostAInitiateBase ) : public CUsbStateHostABase,
        MUsbTimerObserver, MUsbPersonalitySwitchObserver

    {
    friend class CTestUsbOtgWatcher;

public:

    /**
     * Destructor.
     */
    virtual ~CUsbStateHostAInitiateBase();

    // From MUsbTimerObserver
    /**
     * timer expired call back
     * @param aTimerId timer id
     */
    void TimerElapsedL(TUsbTimerId aTimerId);
    
    void UsbPersonalitySwitchStateChangedL(MUsbPersonalitySwitchObserver::TState aState, TInt aData);

protected:

    /**
     * Default construction
     * @param aWatcher owner
     */
    CUsbStateHostAInitiateBase(CUsbOtgWatcher* aWatcher);

    /**
     * 2nd phase construction
     */
    void ConstructL();

    //from CUsbState

    /**
     * This is called when switched to this state, 
     * because If all conditions for transition to another state exist, nothing will 
     * trigger it and transition will not happen. This forces the transition in such cases.
     *
     */
    virtual void JustBeforeLeavingThisStateL();

    // From VBus observer
    /**
     * VBus down
     */
    virtual void VBusDownL();
    /**
     * VBus UP
     */
    virtual void VBusUpL();

    /**
     * AHost
     */
    virtual void AHostL();

    /**
     * APeripheral
     */
    virtual void APeripheralL();

    // From Host Event notification observer
    /**
     * Device is attached
     * @param Device event data
     */
    virtual void DeviceAttachedL(TDeviceEventInformation);
    /**
     * Device is detached
     * @param Device event data
     */
    virtual void DriverLoadSuccessL(TDeviceEventInformation);
    /**
     * Drivers loaded partially
     * @param Device event data
     */
    virtual void DriverLoadPartialSuccessL(TDeviceEventInformation);
    /**
     * Drivers loading failed
     * @param Device event data
     */
    virtual void DriverLoadFailureL(TDeviceEventInformation);

    // From message notification observer
    /**
     * SRP request received
     */
    virtual void SrpReceivedL();

protected:
    // data

    /*
     * attachment timer
     * own
     */
    CUsbTimer* iAttachmentTimer;
    
    CUsbPersonalitySwitch* iUsbPersonalitySwitch;
    };

#endif //  C_USBSTATEHOSTAINITIATEBASE_H
