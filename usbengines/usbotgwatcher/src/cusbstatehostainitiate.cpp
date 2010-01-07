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
* Description:  Implementation
 *
*/


#include "cusbstatehostainitiate.h"
#ifndef STIF
#include "cusbnotifmanager.h"
#include "cusbtimer.h"
#else
#include "mockcusbnotifmanager.h"
#include "mockcusbtimer.h"
#endif
#include "definitions.h"

#include "errors.h"
#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostAInitiate::CUsbStateHostAInitiate(CUsbOtgWatcher* aWatcher) :
    CUsbStateHostAInitiateBase(aWatcher)
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiate::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiate::ConstructL" ) );

    CUsbStateHostAInitiateBase::ConstructL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostAInitiate* CUsbStateHostAInitiate::NewL(CUsbOtgWatcher* aWatcher)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiate::NewL" ) );

    CUsbStateHostAInitiate* self = new (ELeave) CUsbStateHostAInitiate(
            aWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostAInitiate::~CUsbStateHostAInitiate()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiate::~CUsbStateHostAInitiate" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TUsbStateIds CUsbStateHostAInitiate::Id()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiate::Id" ) );

    return EUsbStateHostAInitiate;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiate::JustAdvancedToThisStateL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiate::JustAdvancedToThisStateL" ) );

    iWatcher->PrintStateToLog();

    TInt err = iWatcher->Usb().EnableFunctionDriverLoading();

    if (KErrNone != err)
        {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiate::JustAdvancedToThisStateL EnableFunctionDriverLoading error" ) );
        iWatcher->HandleHostProblemL(EUsbWatcherErrorInConnection);
        return;
        }

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostAInitiate::JustAdvancedToThisStateL EnableFunctionDriverLoading(); = %d" ), err));

    // do BusRequest, if down

    if (CUsbVBusObserver::EVBusUp != iWatcher->VBusObserver()->VBus())
        {
        const TUint maxTrial = 3;
        TInt busReqErr(KErrGeneral);
        TUint count(0);

        while (count < maxTrial && KErrNone != busReqErr)
            {
            busReqErr = iWatcher->Usb().BusRequest();
                FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostAInitiate::JustAdvancedToThisStateL BusRequest() = %d" ), err));

            if (KErrNone != busReqErr)
                {
                err = iWatcher->Usb().BusClearError();
                if (KErrNone != err)
                    {
                    iWatcher->HandleHostProblemL(EUsbWatcherErrorInConnection);
                    return;
                    }
                }
            ++count;
            }
        if (KErrNone != busReqErr)
            {
            iWatcher->HandleHostProblemL(EUsbWatcherErrorInConnection);
            return;
            }
        }

    // start timer, waiting for device attachment
    iAttachmentTimer->After(KTimeToWaitForDeviceAttachment);

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAInitiate::JustBeforeLeavingThisStateL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAInitiate::JustBeforeLeavingThisStateL" ) );
    
        CUsbStateHostAInitiateBase::JustBeforeLeavingThisStateL();
    }
