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


#include "cusbstatehostaidle.h"
#include "cusbnotifmanager.h"

#ifndef STIF
#include "cusbtimer.h"
#else
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
CUsbStateHostAIdle::CUsbStateHostAIdle(CUsbOtgWatcher* aWatcher) :
    CUsbStateHostAInitiateBase(aWatcher)
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAIdle::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAIdle::ConstructL" ) );

    CUsbStateHostAInitiateBase::ConstructL();
    }
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//

CUsbStateHostAIdle* CUsbStateHostAIdle::NewL(CUsbOtgWatcher* aWatcher)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAIdle::NewL" ) );

    CUsbStateHostAIdle* self = new (ELeave) CUsbStateHostAIdle(aWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostAIdle::~CUsbStateHostAIdle()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAIdle::~CUsbStateHostAIdle" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TUsbStateIds CUsbStateHostAIdle::Id()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAIdle::Id" ) );

    return EUsbStateHostAIdle;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAIdle::JustAdvancedToThisStateL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAIdle::JustAdvancedToThisStateL" ) );

    iWatcher->PrintStateToLog();
    
    iWatcher->NotifManager()->ShowIndicatorL(EFalse);

    // do BusDrop, if Up
    if (CUsbVBusObserver::EVBusUp == iWatcher->VBusObserver()->VBus())
        {
        TInt err = iWatcher->Usb().BusDrop();
        if (err != KErrNone)
            {
            err = iWatcher->Usb().BusClearError();
            if (KErrNone != err)
                {
                iWatcher->HandleHostProblemL(EUsbWatcherErrorInConnection);
                }
            }
        }
    }
