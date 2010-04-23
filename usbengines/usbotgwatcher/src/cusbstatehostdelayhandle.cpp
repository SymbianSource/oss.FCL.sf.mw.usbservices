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

#include "cusbstatehostdelayhandle.h"
#ifndef STIF
#include "cusbnotifmanager.h"
#else
#include "mockcusbnotifmanager.h"
#endif

#include "errors.h"
#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostDelayHandle::CUsbStateHostDelayHandle(CUsbOtgWatcher* aWatcher) :
    CUsbStateHostHandle(aWatcher)
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostDelayHandle* CUsbStateHostDelayHandle::NewL(CUsbOtgWatcher* aWatcher)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::NewL" ) );

    CUsbStateHostDelayHandle* self = new (ELeave) CUsbStateHostDelayHandle(aWatcher);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::ConstructL" ) );

    iTooMuchPowerTimer = CUsbTimer::NewL(this, ETooMuchPowerRequiredTimer);
    iDriversNotFoundTimer = CUsbTimer::NewL(this, EDriversNotFoundTimer);

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostDelayHandle::~CUsbStateHostDelayHandle()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::~CUsbStateHostDelayHandle" ) );

    delete iTooMuchPowerTimer;
    delete iDriversNotFoundTimer;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TUsbStateIds CUsbStateHostDelayHandle::Id()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::Id" ) );

    return EUsbStateHostDelayHandle;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::JustAdvancedToThisStateL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::JustAdvancedToThisStateL" ) );
        // first do something specific to entering this state
        // nothing by now, but something in the future
        
        // then do general things, defined in superclass
        CUsbStateHostHandle::JustAdvancedToThisStateL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::JustBeforeLeavingThisStateL()
    {
    iTooMuchPowerTimer->Cancel();
    iDriversNotFoundTimer->Cancel();
    
    // do general things for error handling 
    CUsbStateHostHandle::JustBeforeLeavingThisStateL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::DoHandleL()
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::DoHandleL iWhat = %d" ), iWhat));

    switch (iWhat)
        {
        case EUsbWatcherErrDriversNotFound:
            {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::DoHandleL DriversNotFound" ) );
                
            iDriversNotFoundTimer->After(KTimeDriversNotFound);
            break;
                
            }

        case EUsbWatcherErrDeviceRequiresTooMuchPowerOnEnumeration:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::DoHandleL EUsbWatcherErrDeviceRequiresTooMuchPowerOnEnumeration" ) );

            iTooMuchPowerTimer->After(KTimeTooMuchPowerRequired);
            break;
            }

        default:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::DoHandleL Pass request to CUsbStateHostHandle" ) );
                Panic(EUnexpectedSituationToHandle);
            break;
            }
        }
    }

/////////////////////////////////////////////////////////////////////////////////////
// From VBus observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::VBusUpL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::VBusUpL" ) );
        // this is not expected, due to in this state vbus already up, since entering the state
        Panic(EVBusUpNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::VBusDownL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::VBusDownL" ) );
        // this is not expected, due to in this state vbus up during all the lifetime
        Panic(EVBusDownNotExpected);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::VBusErrorL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::VBusErrorL" ) );
        iWatcher->Usb().BusClearError();
        HandleL(EUsbWatcherErrDeviceRequiresTooMuchPower, EUsbStateHostHandle);
    }

// ---------------------------------------------------------------------------
// this will drop VBus in a while => 
// handle it in another state
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::AVBusErrorL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::AVBusErrorL" ) );
        HandleL(EUsbWatcherErrDeviceRequiresTooMuchPower, EUsbStateHostHandle);
    }

// From Host Event notification observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::DeviceAttachedL(
        TDeviceEventInformation aDevEventInfo)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::DeviceAttachedL" ) );

        ChangeHostStateL(EUsbStateHostAInitiate);
        iWatcher->DeviceAttachedL(aDevEventInfo);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::DeviceDetachedL(TDeviceEventInformation)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::DeviceDetachedL" ) );
        ChangeHostStateL(EUsbStateHostAInitiate);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::BadHubPositionL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::BadHubPositionL" ) );

        HandleL(EUsbWatcherHubsNotSupported, EUsbStateHostHandle);
    }


// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::SrpReceivedL()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::SrpReceivedL" ) );
    // srp is not expected due to vbus is up in thes state   
    Panic(ESrpNotExpected);
    }

// From TimerObserver
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbStateHostDelayHandle::TimerElapsedL(TUsbTimerId aTimerId)
    {
    switch (aTimerId)
        {
        case ETooMuchPowerRequiredTimer:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::TimerElapsedL - ETooMuchPowerRequiredTimer" ) );
                HandleL(EUsbWatcherErrDeviceRequiresTooMuchPower, EUsbStateHostHandle);
            break;
            }
        case EDriversNotFoundTimer:
            {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::TimerElapsedL - EDriversNotFoundTimer" ) );
            HandleL(EUsbWatcherErrUnsupportedDevice, EUsbStateHostHandle);

            break;
            }
        default:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbStateHostDelayHandle::TimerElapsedL - Unknown timer" ) );
            Panic(EWrongTimerId);
            }
        }
    }
