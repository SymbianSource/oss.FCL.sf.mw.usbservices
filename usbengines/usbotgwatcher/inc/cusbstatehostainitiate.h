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


#ifndef C_USBSTATEHOSTAINITIATE_H
#define C_USBSTATEHOSTAINITIATE_H

#include "cusbotgwatcher.h"
#include "cusbstatehostainitiatebase.h"
#ifndef STIF
#include "cusbtimer.h"
#else
#include "mockcusbtimer.h"
#endif

/**
 *  This class implements behaviour when Id pin just detected (initiating host role, loading drivers)
 *
 */
NONSHARABLE_CLASS( CUsbStateHostAInitiate ) : public CUsbStateHostAInitiateBase
    {
    friend class CtUsbOtgWatcher;

public:

    /**
     * Two-phased constructor.
     * @param aWatcher owner
     */
    static CUsbStateHostAInitiate* NewL(CUsbOtgWatcher* aWatcher);

    /**
     * Destructor.
     */
    virtual ~CUsbStateHostAInitiate();

private:

    /**
     * default construction
     * @param aWatcher owner
     */
    CUsbStateHostAInitiate(CUsbOtgWatcher* aWatcher);

    /**
     * 2nd phase construction
     */
    void ConstructL();

    //from CUsbState
    /**
     * state id
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
     * This is called when switched to this state, 
     * because If all conditions for transition to another state exist, nothing will 
     * trigger it and transition will not happen. This forces the transition in such cases.
     *
     */
    virtual void JustBeforeLeavingThisStateL();

    };

#endif //  C_USBSTATEHOSTINITIATE_H
