/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  USB Watcher session class implementation.
*
*/


#include "cusbwatchersession.h"
#include "usbwatchershared.h"
#include "cusbwatcher.h"
#include "cusbwatcherserver.h"
#include "debug.h"

// ----------------------------------------------------------------------------
// Symbian two-phase constructor
// ----------------------------------------------------------------------------
//
CUsbWatcherSession* CUsbWatcherSession::NewL( CUsbWatcherServer* aServer )
    {
    LOG_FUNC

    CUsbWatcherSession* r = new ( ELeave ) CUsbWatcherSession( aServer );
    CleanupStack::PushL( r );
    r->ConstructL();
    CleanupStack::Pop();
    return r;
    }

// ----------------------------------------------------------------------------
// C++ constructor
// ----------------------------------------------------------------------------
//
CUsbWatcherSession::CUsbWatcherSession( CUsbWatcherServer* aServer )
    : iUsbWatcherServer( aServer )
    {
    LOG_FUNC

    }

// ----------------------------------------------------------------------------
// Second-phase constructor
// ----------------------------------------------------------------------------
//
void CUsbWatcherSession::ConstructL()
    {
    LOG_FUNC

    iUsbWatcherServer->Watcher().RegisterObserverL( this );
    }

// ----------------------------------------------------------------------------
// Desstructor
// ----------------------------------------------------------------------------
//
CUsbWatcherSession::~CUsbWatcherSession()
    {
    LOG_FUNC

    // if server isn't exist then session can not be exist
    if ( iUsbWatcherServer )
        {
        iUsbWatcherServer->Watcher().DeRegisterObserver( this );
        }
    }

// ----------------------------------------------------------------------------
// Request handle entry point
// ----------------------------------------------------------------------------
//
void CUsbWatcherSession::ServiceL( const RMessage2& aMessage )
    {
    LOG_FUNC

    DispatchMessageL( aMessage );
    }

// ----------------------------------------------------------------------------
// Request dispatch function
// ----------------------------------------------------------------------------
//
void CUsbWatcherSession::DispatchMessageL( const RMessage2& aMessage )
    {
    LOG_FUNC

    LOG1( "Message = %d", aMessage.Function() );

    TBool complete( ETrue );
    TInt ret( KErrNone );


    switch ( aMessage.Function() )
        {
        case EUsbWatcherSetPersonality:
            ret = SetPersonality( aMessage, complete );
            break;

        case EUsbWatcherCancelSetPersonality:
            ret = CancelSetPersonality( aMessage, complete );
            break;

        case EUsbWatcherSetPreviousPersonality:
            ret = SetPreviousPersonality( aMessage, complete );
            break;

        case EUsbWatcherSetPreviousPersonalitySync:
            ret = SetPreviousPersonalitySync( aMessage, complete );
            break;

        case EUsbWatcherCancelSetPreviousPersonality:
            ret = CancelSetPreviousPersonality( aMessage, complete );
            break;

        case EUsbWatcherSetPreviousPersonalityOnDisconnect:
            ret = SetPreviousPersonalityOnDisconnect( aMessage, complete );
            break;

        default:
            aMessage.Panic( KUsbWatcherCliPncCat, EUsbWatcherPanicIllegalIPC );
            break;
        }

    if ( complete )
        {
        aMessage.Complete( ret );
        }
    }

// ----------------------------------------------------------------------------
// Set certain personality
// ----------------------------------------------------------------------------
//
TInt CUsbWatcherSession::SetPersonality( const RMessage2& aMessage,
        TBool& aComplete )
    {
    LOG_FUNC

    if( iSetPersonalityOutstanding )
        {
        LOG( "Completing outstanding" );
        iSetPersonalityMessage.Complete( KErrNone );
        iSetPersonalityOutstanding = EFalse;
        }

    // Cancel all other pending requests
    iUsbWatcherServer->Watcher().Notify( KErrCancel );

    iSetPersonalityMessage = aMessage;
    aComplete = EFalse;
    iSetPersonalityOutstanding = ETrue;

    //Set force parameter to this session.
    SetAskOnConnectionSuppression( aMessage.Int1() );

    iUsbWatcherServer->Watcher().SetPersonality( aMessage.Int0(),
        static_cast<TBool>( aMessage.Int2() ) );

    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Cancel pending set personality request
// ----------------------------------------------------------------------------
//
TInt CUsbWatcherSession::CancelSetPersonality( const RMessage2& aMessage,
        TBool& aComplete )
    {
    LOG_FUNC

    if ( !iSetPersonalityOutstanding )
        {
        LOG( "No outstanding SetPersonality request" );
        return KErrNone;
        }

    SetAskOnConnectionSuppression( EFalse );
    aComplete = EFalse;
    iCancelSetPersonalityMessage = aMessage;
    iCancelSetPersonalityOutstanding = ETrue;

    iUsbWatcherServer->Watcher().CancelSetPersonality();

    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Change to previous personality, asynchronous version
// ----------------------------------------------------------------------------
//
TInt CUsbWatcherSession::SetPreviousPersonality( const RMessage2& aMessage,
        TBool& aComplete )
    {
    LOG_FUNC

    if( iSetPreviousPersonalityOutstanding )
        {
        iSetPreviousPersonalityMessage.Complete( KErrNone );
        iSetPreviousPersonalityOutstanding = EFalse;
        }

    // Cancel all other pending requests
    iUsbWatcherServer->Watcher().Notify( KErrCancel );

    SetAskOnConnectionSuppression( EFalse );
    iSetPreviousPersonalityOutstanding = ETrue;
    iSetPreviousPersonalityMessage = aMessage;
    aComplete = EFalse;

    iUsbWatcherServer->Watcher().SetPreviousPersonality();

    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Change to previous personality, synchronous version
// ----------------------------------------------------------------------------
//
TInt CUsbWatcherSession::SetPreviousPersonalitySync( const RMessage2& /*aMsg*/,
        TBool& /*aComplete*/ )
    {
    LOG_FUNC

    if( iSetPreviousPersonalityOutstanding )
        {
        iSetPreviousPersonalityMessage.Complete( KErrNone );
        iSetPreviousPersonalityOutstanding = EFalse;
        }

    // Cancel all other pending requests
    iUsbWatcherServer->Watcher().Notify( KErrCancel );

    SetAskOnConnectionSuppression( EFalse );
    iUsbWatcherServer->Watcher().SetPreviousPersonality();

    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Cancel pending request to set previous personality
// ----------------------------------------------------------------------------
//
TInt CUsbWatcherSession::CancelSetPreviousPersonality( const RMessage2& aMsg,
        TBool& aComplete )
    {
    LOG_FUNC

    if( !iSetPreviousPersonalityOutstanding )
        {
        return KErrNone;
        }

    aComplete = EFalse;
    iCancelSetPreviousPersonalityMessage = aMsg;
    iCancelSetPreviousPersonalityOutstanding = ETrue;

    iUsbWatcherServer->Watcher().CancelSetPreviousPersonality();

    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Set the flag to restore personality when disconnected
// ----------------------------------------------------------------------------
//
TInt CUsbWatcherSession::SetPreviousPersonalityOnDisconnect( const RMessage2&
        /*aMessage*/, TBool& /*aComplete*/ )
    {
    LOG_FUNC

    //connected currently, so ask on connection can be enabled
    SetAskOnConnectionSuppression( EFalse );
    iUsbWatcherServer->Watcher().SetPreviousPersonalityOnDisconnect();

    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Complete request
// ----------------------------------------------------------------------------
//
void CUsbWatcherSession::Notify( TInt /*aPersonalityId*/, TInt aStatus )
    {
    LOG_FUNC

    if( iCancelSetPersonalityOutstanding )
        {
        iCancelSetPersonalityOutstanding = EFalse;
        iCancelSetPersonalityMessage.Complete( KErrNone );

        iSetPersonalityMessage.Complete( aStatus );
        iSetPersonalityOutstanding = EFalse;
        }

    if ( iSetPersonalityOutstanding )
        {
        iSetPersonalityMessage.Complete( aStatus );
        iSetPersonalityOutstanding = EFalse;
        }

    if ( iCancelSetPreviousPersonalityOutstanding )
        {
        iCancelSetPreviousPersonalityOutstanding = EFalse;
        iCancelSetPreviousPersonalityMessage.Complete( KErrNone );

        iSetPreviousPersonalityMessage.Complete( aStatus );
        iSetPreviousPersonalityOutstanding = EFalse;
        }

    if ( iSetPreviousPersonalityOutstanding )
        {
        iSetPreviousPersonalityMessage.Complete( aStatus );
        iSetPreviousPersonalityOutstanding = EFalse;
        }
    }

// ----------------------------------------------------------------------------
// Set or clear AskOnConnection suppression
// ----------------------------------------------------------------------------
//
void CUsbWatcherSession::SetAskOnConnectionSuppression( TBool aSuppress )
    {
    LOG_FUNC

    iSuppressAskOnConnection = aSuppress;
    }

// ----------------------------------------------------------------------------
// Check if AskOnConnection is suppressed
// ----------------------------------------------------------------------------
//
TBool CUsbWatcherSession::IsAskOnConnectionSuppressed()
    {
    LOG_FUNC

    return iSuppressAskOnConnection;
    }

// End of file
