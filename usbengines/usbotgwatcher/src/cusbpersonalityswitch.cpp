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
* Description:  Switches personality
 *
*/

#include <e32base.h>
#include <usb.h>
#include <usbpersonalityids.h>

#include "cusbpersonalityswitch.h"

#include "panic.h"
#include "debug.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbPersonalitySwitch::CUsbPersonalitySwitch(
        MUsbPersonalitySwitchObserver* aObserver) :
    CActive(CActive::EPriorityStandard), iObserver(aObserver), iState(EIdle)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbPersonalitySwitch::~CUsbPersonalitySwitch()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::~CUsbPersonalitySwitch" ) );
    Cancel();
    iUsbInterface.Close();
    iSendData.Close();
    iRcvData.Close();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbPersonalitySwitch::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::ConstructL" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbPersonalitySwitch* CUsbPersonalitySwitch::NewL(
        MUsbPersonalitySwitchObserver* aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::NewL" ) );

    CUsbPersonalitySwitch* self = new (ELeave) CUsbPersonalitySwitch(
            aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self); // pop self
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbPersonalitySwitch::SwitchPersonalityL(TUint32 aDeviceId,
        TInt aPersonalityToBeSet)
    {
        FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::SwitchPersonality aDeviceId %d, aPersonalityToBeSet %d" ), aDeviceId, aPersonalityToBeSet))

    if (IsActive() || EIdle != iState)
        {
        Cancel();
        iObserver->UsbPersonalitySwitchStateChangedL(
                MUsbPersonalitySwitchObserver::ERequestCancelled, KErrNone);
        }

    // create usb interface for the device id
    TUint32 token(0); // = RFdc.InterfaceToken();
    TInt err = iUsbInterface.Open(token);

        FTRACE(FPrint(_L("[USBOTGWATCHER]\tCUsbPersonalitySwitch::SwitchPersonality UsbInterface Open err = %d" ), err));
    if (KErrNone != err)
        {
        return err;
        }

    iPersonalityToBeSet = aPersonalityToBeSet;

    // get list of supported personalities
    iTransfer.iRequestType = KUsbRequestType_DirToHost
            | KUsbRequestType_TypeVendor | KUsbRequestType_DestDevice;

    iTransfer.iRequest = KGetAllPersonalitiesReq;
    iTransfer.iValue = 0;
    iTransfer.iIndex = 0;
    iTransfer.iFlags
            = RUsbInterface::TUsbTransferRequestDetails::EShortTransferOk;

    Reset();

    iState = EGetAllPersonalities;

        FLOG( _L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::SwitchPersonalityL Sending GetAllPersonalities request." ) );
    iUsbInterface.Ep0Transfer(iTransfer, iSendData, iRcvData, iStatus);
    SetActive();

    iObserver->UsbPersonalitySwitchStateChangedL(
            MUsbPersonalitySwitchObserver::ERequestingAllPersonalities,
            KErrNone);

    return KErrNone;
    }

void CUsbPersonalitySwitch::CancelSwitchPersonalityL()
    {
        FLOG(_L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::CancelSwitchPersonalityL"));

    if (IsActive())
        {
        Cancel();
        iObserver->UsbPersonalitySwitchStateChangedL(
                MUsbPersonalitySwitchObserver::ERequestCancelled, KErrNone);
        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbPersonalitySwitch::RunL()
    {

    if (KErrNone != iStatus.Int())
        {
            FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::RunL iStatus %d" ), iStatus.Int()));
        User::Leave(iStatus.Int());
        }

    switch (iState)
        {

        case EGetAllPersonalities:
            {
                FLOG(_L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::RunL GetAllPersonalities completed."));

            TInt numOfPersonalities = iRcvData[KNumOfPersShift];
                FTRACE(FPrint(_L("[USBOTGWATCHER]\tCUsbPersonalitySwitch::RunL Amount of personalities in peripheral = %d" ), numOfPersonalities));

            // if no personalities then return
            if (0 == numOfPersonalities)
                {
                    FLOG(_L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::RunL No personailies found in peripheral."));
                Reset();
                iObserver->UsbPersonalitySwitchStateChangedL(
                        MUsbPersonalitySwitchObserver::EGetAllPersonalitiesCompleted,
                        KErrNotFound);
                return;
                }

            // if needed pers already set, then return
            TInt currentPersonality = iRcvData[KCurrPersShift];
            if (iPersonalityToBeSet == currentPersonality)
                {
                    FLOG(_L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::RunL Needed personality already set in peripheral."));
                Reset();
                iObserver->UsbPersonalitySwitchStateChangedL(
                        MUsbPersonalitySwitchObserver::EGetAllPersonalitiesCompleted,
                        KErrInUse);
                return;
                }

            // check if device supports needed personality
            TInt count(0);
            while (count < numOfPersonalities && iPersonalityToBeSet
                    != iRcvData[KFirstPersonalityIdShift
                            + KLenghtOfPersonalityData * count])
                {
                ++count;
                }

            if (count == numOfPersonalities) // not found == not supported
                {
                    FLOG(_L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::RunL Peripheral does not support needed personality."));
                Reset();
                iObserver->UsbPersonalitySwitchStateChangedL(
                        MUsbPersonalitySwitchObserver::EGetAllPersonalitiesCompleted,
                        KErrNotFound);
                return;
                }

            iObserver->UsbPersonalitySwitchStateChangedL(
                    MUsbPersonalitySwitchObserver::EGetAllPersonalitiesCompleted,
                    KErrNone);

            // set personality
            iTransfer.iRequestType = KUsbRequestType_DirToDev
                    | KUsbRequestType_TypeVendor | KUsbRequestType_DestDevice;

            iTransfer.iRequest = KSetPersonalityReq;
            iTransfer.iValue = iPersonalityToBeSet;
            iTransfer.iIndex = 0;
            iTransfer.iFlags
                    = RUsbInterface::TUsbTransferRequestDetails::EShortTransferOk;

            iSendData.Close();
            iRcvData.Close();

            iState = ESetPersonality;

                FLOG( _L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::RunL Sending SetPersonality request." ) );
            iUsbInterface.Ep0Transfer(iTransfer, iSendData, iRcvData, iStatus);
            SetActive();
            iObserver->UsbPersonalitySwitchStateChangedL(
                    MUsbPersonalitySwitchObserver::ERequestingSetPersonality,
                    KErrNone);

            break;
            }
        case ESetPersonality:
            {
                FLOG(_L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::RunL SetPersonality completed."));
            Reset();
            iObserver->UsbPersonalitySwitchStateChangedL(
                    MUsbPersonalitySwitchObserver::ESetPersonalityCompleted,
                    KErrNone);

            break;
            }
            /*  case EGetAllPersResult:
             {
             FLOG( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::RunL GetAllPersResult completed.");

             break;
             }
             case EGetSetPersResult:
             {
             FLOG( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::RunL GetSetPersResult completed.");

             break;
             }  */

        case EIdle: // do not break 
        default:
            {
            Panic(EUnexpectedUsbSwitchPersonalityState);
            }
        }

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbPersonalitySwitch::RunError(TInt aError)
    {
        FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::RunError aError %d" ), aError ));
    Reset();
    TRAP_IGNORE (iObserver->UsbPersonalitySwitchStateChangedL(
            MUsbPersonalitySwitchObserver::ERequestFailed, aError) );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbPersonalitySwitch::DoCancel()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::DoCancel" ) )

    iUsbInterface.CancelEP0Transfer();
    Reset();
    }

void CUsbPersonalitySwitch::Reset()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbPersonalitySwitch::Reset" ) )

    iState = EIdle;
    iSendData.Close();
    iRcvData.Close();
    }
