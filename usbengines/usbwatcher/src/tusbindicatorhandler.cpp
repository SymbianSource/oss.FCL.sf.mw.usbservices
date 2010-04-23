/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This implements TUsbIndicatorHandler class.
*
*/


#include <e32std.h>
#include <AknNotifySignature.h> // SAknNotifierPackage
#include <avkon.hrh> //EAknIndicatorUSBConnection
#include "tusbindicatorhandler.h"
#include "debug.h"

// ---------------------------------------------------------------------------
// C++ Constructor 
// ---------------------------------------------------------------------------
//
TUsbIndicatorHandler::TUsbIndicatorHandler() 
    : iDevStateB4Suspend( EUsbDeviceStateUndefined )
    , iUsbIndicator(EFalse)
    {
    }

// ---------------------------------------------------------------------------
// Change USB Indicator 
// ---------------------------------------------------------------------------
//
void TUsbIndicatorHandler::HandleDeviceStateChange( TUsbDeviceState aStateOld, 
            TUsbDeviceState aStateNew )
    {
    LOG_FUNC

    LOG2( "USB device state changed: %d ==> %d", aStateOld, aStateNew );
    
    if ( EUsbDeviceStateConfigured == aStateNew ) // Entering configed state 
        {
        // filter out case of configed -> suspended -> configed
        if ( ( EUsbDeviceStateSuspended != aStateOld ) || 
             ( EUsbDeviceStateConfigured != iDevStateB4Suspend) )
            {
            ShowUsbIndicator( ETrue );
            }
        }
    else if ( EUsbDeviceStateSuspended == aStateNew )
    // If current state is suspend, we do not change the indicator, but save
    // the previous state
        {
        iDevStateB4Suspend = aStateOld;
        }
    else // New device state is not configured, nor suspended
        {
        // Hide USB indicator if previous state is either configured, or
        // suspended AND state before is not configured
        if( ( EUsbDeviceStateConfigured == aStateOld ) ||
            ( ( EUsbDeviceStateSuspended == aStateOld ) &&
              ( EUsbDeviceStateConfigured == iDevStateB4Suspend ) ) ) 
            {
            ShowUsbIndicator( EFalse );
            }
        } 
    }
    
// ---------------------------------------------------------------------------
// Show USB Indicator
// ---------------------------------------------------------------------------
//
void TUsbIndicatorHandler::ShowUsbIndicator( TInt aUsbIndicatorState )
    {
    LOG_FUNC
    
    iUsbIndicator = aUsbIndicatorState;
    LOG1( "USB indicator state %d", aUsbIndicatorState );
    RNotifier notifier;
    TInt err = notifier.Connect();
    if ( KErrNone == err )
        {
        TPckgBuf< SAknNotifierPackage<SAknSmallIndicatorParams> > pckg;
        pckg().iParamData.iSmallIndicatorUid =
            TUid::Uid( EAknIndicatorUSBConnection );
        pckg().iParamData.iValue = aUsbIndicatorState ? 
            EAknIndicatorStateOn : EAknIndicatorStateOff ;
        notifier.StartNotifier( KAknSmallIndicatorUid, pckg );
        notifier.Close();
        }
    else
        {
        LOG1( "RNotifier::Connect error: %d", err );
        }
    }

// End of file
