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
* Description:  Implementation of the concret state
 *
*/


#ifndef C_USBSTATEHOSTAHOST_H
#define C_USBSTATEHOSTAHOST_H

#include "cusbotgwatcher.h"
#include "cusbstatehostabase.h"
#ifndef STIF
#include "cusbtimer.h"
#else
#include "mockcusbtimer.h"
#endif

/**
 *  This class implements behaviour in Host state (Drivers loaded, device is in Host role)
 *
 */
NONSHARABLE_CLASS( CUsbStateHostAHost ) : public CUsbStateHostABase
    {
public:

    /**
     * Two-phased constructor.
     * @param aWatcher Owner
     */
    static CUsbStateHostAHost* NewL(CUsbOtgWatcher* aWatcher);

    /**
     * Destructor.
     */
    virtual ~CUsbStateHostAHost();

private:

    /**
     * Default construction
     * @param aWatcher owner
     */
    CUsbStateHostAHost(CUsbOtgWatcher* aWatcher);

    /**
     * 2nd phase construction
     */
    void ConstructL();

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
     * State machine calls this, state object to perform any initial  
     * activity, just before leaving this state
     *
     */
    void JustBeforeLeavingThisStateL();

    // From OTG state observer
    /**
     * Became a Host when A
     */
    void AHostL();

    /**
     * Became a Peripheral when A
     */
    void APeripheralL();
    
    // From Host Event notification observer
    /**
     * Device is detached
     * @param aInfo Device event data
     */
    void DeviceDetachedL(TDeviceEventInformation aInfo);

    // From message notification observer
    /**
     * Connected to hub in wrong level 
     */
    void BadHubPositionL();

private:
    // data
    };

#endif //  C_USBSTATEHOSTAHOST_H
