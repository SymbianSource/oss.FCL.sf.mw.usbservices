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
* Description:  Personality switch function.
 *
*/

#ifndef C_USBPERSONALITYSWITCH_H
#define C_USBPERSONALITYSWITCH_H

#include <e32base.h>
#include <d32usbdi.h> 

const TInt KGetAllPersonalitiesReq = 0x82;
const TInt KSetPersonalityReq = 0x89;

const TInt KNumOfPersShift = 3;
const TInt KCurrPersShift = 2;
const TInt KFirstPersonalityIdShift = 4;
const TInt KLenghtOfPersonalityData = 2;

class MUsbPersonalitySwitchObserver
    {
public:
    enum TState
        {
        EIdle = 0,
        ERequestingAllPersonalities,
        ERequestingSetPersonality,
        EGetAllPersonalitiesCompleted,
        ESetPersonalityCompleted,
        ERequestCancelled,
        ERequestFailed
        };
public:
    virtual void UsbPersonalitySwitchStateChangedL(TState aState, TInt aData) = 0;
    };

/* *
 * Reads personalities, if possible, switches peripheral to mass storage.
 * It is assumed that USB is started already
 */
NONSHARABLE_CLASS(CUsbPersonalitySwitch) : public CActive
    {
    enum TState
    {
    EIdle = 0,
    EGetAllPersonalities,
    ESetPersonality /*,
    EGetAllPersResult,
    EGetSetPersResult*/
    };
    
public:

    /**
     * Two-phased constructor
     *
     * @param aObserver will get call back
     * @param aTimerId timer id
     */
    static CUsbPersonalitySwitch * NewL(MUsbPersonalitySwitchObserver* aObserver);

    /**
     * Destructor.
     */
    virtual ~CUsbPersonalitySwitch();

    /**
     * calls RunL after aMilliseconds
     * @param aMilliseconds time in millisecs
     */
    TInt SwitchPersonalityL(TUint32 aDeviceId, TInt aPersonalityToBeSet);
    
    void CancelSwitchPersonalityL();

public:

    // from base class CActive
    /**
     * Called when request is completed
     */
    void RunL();

    /**
     * called when RunL leaves
     * @param aError error code
     * @return error code
     */
    TInt RunError(TInt aError);

    /**
     * Called when request is cancelled
     */
    void DoCancel();

private:

    /**
     * Default constructor
     * @param aObserver will get call back
     * @param aTimerId timer id 
     */
    CUsbPersonalitySwitch(MUsbPersonalitySwitchObserver* aObserver);

    /**
     * 2nd phase construction
     */
    void ConstructL();
    
    void Reset();

private:
    // data

    /**
     * Observer
     * not own
     */
    MUsbPersonalitySwitchObserver* iObserver;
    
    RUsbInterface iUsbInterface;

    /**
     * timer id
     */
    TInt iPersonalityToBeSet;
    
    RUsbInterface::TUsbTransferRequestDetails iTransfer;
    
    RBuf8 iSendData;
    
    RBuf8 iRcvData;
    
    TState iState;
    };

#endif //  C_USBPERSONALITYSWITCH_H
