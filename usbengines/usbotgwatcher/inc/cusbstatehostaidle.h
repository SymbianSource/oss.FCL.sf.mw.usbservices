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
* Description:  Iplementation of the concret state
 *
*/


#ifndef C_USBSTATEHOSTAIDLE_H
#define C_USBSTATEHOSTAIDLE_H

#include "cusbotgwatcher.h"
#include "cusbstatehostainitiatebase.h"
#ifndef STIF
#include "cusbtimer.h"
#else
#include "mockcusbtimer.h"
#endif

/**
 *  This class implements basic behaviour in Idle state (VBus down, IdPin on)
 *
 */
NONSHARABLE_CLASS( CUsbStateHostAIdle ) : public CUsbStateHostAInitiateBase

    {

public:

    /**
     * Two-phased constructor.
     * @param aWatcher owner
     */
    static CUsbStateHostAIdle* NewL(CUsbOtgWatcher* aWatcher);

    /**
     * Destructor.
     */
    virtual ~CUsbStateHostAIdle();

private:

    /**
     * Default construction
     * @param aWatcher owner
     */
    CUsbStateHostAIdle(CUsbOtgWatcher* aWatcher);

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

    };

#endif //  C_USBSTATEHOSTAIDLE_H
