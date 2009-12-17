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


#include <e32base.h>
#include <UsbWatcherInternalPSKeys.h>

#include "cusbstatehostahost.h"
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
CUsbStateHostAHost::CUsbStateHostAHost(CUsbOtgWatcher* aWatcher) :
    CUsbStateHostABase(aWatcher)
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAHost::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAHost::ConstructL" ) );

    CUsbStateHostABase::ConstructL();

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostAHost* CUsbStateHostAHost::NewL(CUsbOtgWatcher* aWatcher)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAHost::NewL" ) );

    CUsbStateHostAHost* self = new (ELeave) CUsbStateHostAHost(aWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostAHost::~CUsbStateHostAHost()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAHost::~CUsbStateHostAHost" ) );

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TUsbStateIds CUsbStateHostAHost::Id()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAHost::Id" ) );

    return EUsbStateHostAHost;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAHost::JustAdvancedToThisStateL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAHost::JustAdvancedToThisStateL" ) );
    
    User::LeaveIfError( RProperty::Set( KPSUidUsbWatcher,
                KUsbWatcherIsPeripheralConnected,
                KUsbWatcherPeripheralIsConnected ) );
    
    iWatcher->NotifManager()->ShowIndicatorL(ETrue);
    
    iWatcher->PrintStateToLog();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAHost::JustBeforeLeavingThisStateL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAHost::JustBeforeLeavingThisStateL" ) );
        
        User::LeaveIfError( RProperty::Set( KPSUidUsbWatcher,
                    KUsbWatcherIsPeripheralConnected,
                    KUsbWatcherPeripheralIsNotConnected ) );
        
        iWatcher->NotifManager()->ShowIndicatorL(EFalse);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAHost::AHostL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAHost::AHostL" ) );
    // do nothing
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAHost::APeripheralL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostABase::AHostL" ) );
    ChangeHostStateL(EUsbStateHostAPeripheral);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAHost::DeviceDetachedL(TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAHost::DeviceDetachedL" ) );
    ChangeHostStateL(EUsbStateHostAInitiate);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostAHost::BadHubPositionL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostAHost::BadHubPositionL" ) );
    Panic(EBadHubPositionEventNotExpected);
    }
