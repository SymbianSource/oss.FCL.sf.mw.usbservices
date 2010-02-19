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

#include "cusbstatehostundefined.h"

#ifndef STIF

#else

#endif

#include "definitions.h"
#include "errors.h"

#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostUndefined::CUsbStateHostUndefined(
        CUsbOtgWatcher* aWatcher) :
    CUsbStateHostABase(aWatcher)
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostUndefined* CUsbStateHostUndefined::NewL(CUsbOtgWatcher* aWatcher)
   {
   FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostUndefined::NewL" ) );

   CUsbStateHostUndefined* self = new (ELeave) CUsbStateHostUndefined(aWatcher);
   CleanupStack::PushL(self);
   self->ConstructL();
   CleanupStack::Pop(self);
   return self;
   }
        
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostUndefined::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostUndefined::ConstructL" ) );

    CUsbStateHostABase::ConstructL();

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostUndefined::~CUsbStateHostUndefined()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostundefined::~CUsbStateHostUndefined" ) );

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TUsbStateIds CUsbStateHostUndefined::Id()
    {
    return EUsbStateHostUndefined;
    }

/////////////////////////////////////////////////////////////////////////////////////
// From IdPin observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostUndefined::IdPinOffL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostUndefined::IdPinOffL" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostUndefined::IdPinOnL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostUndefined::IdPinOnL" ) );
    }

// From VBus observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostUndefined::VBusDownL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostUndefined::VBusDownL" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostUndefined::VBusUpL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostUndefined::VBusUpL" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostUndefined::VBusErrorL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostUndefined::VBusErrorL" ) );
        iWatcher->Usb().BusClearError();
        HandleL(EUsbWatcherErrorInConnection, EUsbStateHostHandle);
    }

// From OTG state observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostUndefined::AIdleL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostUndefined::AIdleL" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostUndefined::AVBusErrorL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostUndefined::AVBusErrorL" ) );
        iWatcher->Usb().BusClearError();
        HandleL(EUsbWatcherErrorInConnection, EUsbStateHostHandle);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostUndefined::BIdleL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostUndefined::BIdleL" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostUndefined::BPeripheralL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostUndefined::BPeripheralL" ) );
    }

// From host state observer
void CUsbStateHostUndefined::DeviceDetachedL(TDeviceEventInformation /*aInfo*/)
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostUndefined::DeviceDetachedL" ) );
    }
