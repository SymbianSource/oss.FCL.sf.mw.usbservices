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


#include <usbuinotif.h>

#include "cusbstatehostaperipheral.h"

#ifndef STIF
#include "cusbnotifmanager.h"
#include "cusbtimer.h"
#else
#include "mockcusbnotifmanager.h"
#include "mockcusbtimer.h"
#endif

#include "errors.h"
#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostAPeripheral::CUsbStateHostAPeripheral(CUsbOtgWatcher* aWatcher) :
    CUsbStateHostABase(aWatcher)
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAPeripheral::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAPeripheral::ConstructL" ) );

    CUsbStateHostABase::ConstructL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostAPeripheral* CUsbStateHostAPeripheral::NewL(
        CUsbOtgWatcher* aWatcher)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAPeripheral::NewL" ) );

    CUsbStateHostAPeripheral* self = new (ELeave) CUsbStateHostAPeripheral(
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
CUsbStateHostAPeripheral::~CUsbStateHostAPeripheral()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAPeripheral::~CUsbStateHostAPeripheral" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAPeripheral::JustAdvancedToThisStateL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAPeripheral::JustAdvancedToThisStateL" ) );
    iWatcher->PrintStateToLog();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TUsbStateIds CUsbStateHostAPeripheral::Id()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAPeripheral::Id" ) );
    return EUsbStateHostAPeripheral;
    }

// From OTG state observer

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAPeripheral::AHostL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAPeripheral::AHostL" ) );
        ChangeHostStateL(EUsbStateHostAInitiate);
    }

