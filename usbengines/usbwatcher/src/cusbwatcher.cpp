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
* Description:  This implements CUsbWatcher class.
*
*/


#include <ecom/ecom.h>
#include <utf.h>
#include <coreapplicationuisdomainpskeys.h>
//CleanupResetAndDestroyPushL
#include <mmf/common/mmfcontrollerpluginresolver.h>
#include <UsbWatcherInternalCRKeys.h>
#include <cusbpersonalitynotifier.h>
#include <UsbWatcherInternalPSKeys.h>
#include <e32property.h>
#include <startupdomainpskeys.h> //for global system state

#include "cusbwatcher.h"
#include "cusbactivestate.h"
#include "cusbactivepersonalityhandler.h"
#include "cusbwatchersession.h"
#include "cusbdevicelock.h"
#include "cusbdevconstarter.h"
#include "cusbglobalsystemstateobserver.h"
#include "cusbotgwatcher.h"

_LIT_SECURITY_POLICY_PASS( KAlwaysPassPolicy );
_LIT_SECURITY_POLICY_C1( KLocalServicesPolicy, ECapabilityLocalServices );

const TUint32 KUsbWatcherUseSerialNumber = 0x80000000;
const TUint32 KUsbWatcherPersonalityIdMask = 0x0000FFFF;

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that might leave.
// ----------------------------------------------------------------------------
//
CUsbWatcher::CUsbWatcher()
    : CActive( EPriorityStandard )
    {
    LOG_FUNC

    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::ConstructL()
    {
    LOG_FUNC

    LEAVEIFERROR( RProperty::Define( KPSUidUsbWatcher,
            KUsbWatcherSelectedPersonality, RProperty::EInt, KAlwaysPassPolicy,
            KLocalServicesPolicy ) );

    LEAVEIFERROR( RProperty::Set( KPSUidUsbWatcher,
            KUsbWatcherSelectedPersonality,
            KUsbWatcherSelectedPersonalityNone ) );

    iPersonalityRepository = CRepository::NewL( KCRUidUsbWatcher );

    LEAVEIFERROR( iPersonalityRepository->Get( KUsbWatcherPersonality,
            iPersonalityId ) );
    LOG1( "iPersonalityId from CenRep = %d", iPersonalityId );
    iPrevPersonalityId = iPersonalityId;
    iOldPrevPersonalityId = iPersonalityId;
    LEAVEIFERROR( iUsbMan.Connect() );

    TInt ret = iUsbMan.SetCtlSessionMode( ETrue );
    if( KErrNone == ret )
        {
        LOG( "Creating CUsbOtgWatcher..."  );
        iOtgWatcher = CUsbOtgWatcher::NewL( iUsbMan );
        }
    else if( KErrNotSupported == ret )
        {
        LOG( "Non-OTG configuration detected!"  );
        }
    else
        {
        LOG( "ERROR: is there another USB Control process? LEAVE" );
        LEAVE( ret );
        }

    iPersonalityHandler
        = CUsbActivePersonalityHandler::NewL( iUsbMan, *this );

    GetPersonalityPluginsL();
    iUsbDeviceLock = CUsbDeviceLock::NewL( *this );
    iUsbDeviceLock->Subscribe();
    iGlobalStateObserver = CUsbGlobalSystemStateObserver::NewL( *this );
    iUsbDevConStarter = CUsbDevConStarter::NewL();
    iActiveState = CUsbActiveState::NewL( iUsbMan, *this );
    }

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CUsbWatcher* CUsbWatcher::NewL()
    {
    LOG_FUNC

    CUsbWatcher* self = new ( ELeave ) CUsbWatcher();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // pop self
    return self;
    }

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
//
CUsbWatcher::~CUsbWatcher()
    {
    LOG_FUNC

    RProperty::Delete( KPSUidUsbWatcher, KUsbWatcherSelectedPersonality );

    iUsbMan.Close();
    iObservers.Reset();
    if( iUsbDeviceLock )
        {
        iUsbDeviceLock->Cancel();
        }
    delete iActiveState;
    delete iUsbDevConStarter;
    delete iPersonalityHandler;
    delete iPersonalityRepository;
    delete iOtgWatcher;
    delete iUsbDeviceLock;
    delete iGlobalStateObserver;
    iSupportedPersonalities.Close();
    }

// ----------------------------------------------------------------------------
// Get personality plugins. Note that this is done only in normal mode.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::GetPersonalityPluginsL()
    {
    LOG_FUNC
    
    // Get personality plugins
    RImplInfoPtrArray implementations;
    CleanupResetAndDestroyPushL( implementations );

    const TEComResolverParams noResolverParams;

    REComSession::ListImplementationsL( KUidPersonalityPlugIns,
            noResolverParams, KRomOnlyResolverUid, implementations );
    TInt personalityNum = implementations.Count();
    LOG1( "Number of PersonalityPlugin(s): %d", personalityNum );
    
    if( personalityNum < 1 )
        {
        LOG( "No PersonalityPlugin available. LEAVE" );
        LEAVE( KErrGeneral );
        }

    for( TInt i = 0; i < personalityNum; i++ )
        {
        TUsbSupportedPersonalityInf inf;
        TLex8 lex;
        TUint32 confValue;

        // save implementation uid
        inf.iPersonalityUid = implementations[i]->ImplementationUid();
        lex.Assign( implementations[i]->DataType() );

        if( lex.Val( confValue, EHex ) != KErrNone )
            {
            CleanupStack::PopAndDestroy( &implementations );
            iSupportedPersonalities.Close();
            LEAVE( KErrGeneral );
            }

        // check whether personality requires serial number
        if( confValue & KUsbWatcherUseSerialNumber )
            {
            inf.iUseSerialNumber = ETrue;
            }
        else
            {
            inf.iUseSerialNumber = EFalse;
            }

        // save personality id
        inf.iPersonalityId = static_cast<TInt>( ( KUsbWatcherPersonalityIdMask
                & confValue ) );

        LOG1( "PersonalityId = %d" , inf.iPersonalityId );

        iSupportedPersonalities.AppendL( inf );
        }

    CleanupStack::PopAndDestroy( &implementations );
    }

// ----------------------------------------------------------------------------
// This method notifies CUsbWatcher class about USB state changes.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::StateChangeNotify( TUsbDeviceState aStateOld,  
        TUsbDeviceState aStateNew )
    {
    LOG_FUNC

    if( IsDeviceA() ) // Will be handled by UsbOtgWatcher
        {
        LOG( "Device state change ignored by UsbWatcher in A-device state" );
        return;
        }

    LOG1( "WatcherState = %d", iState );
        
    // Notify personality handler first
    switch( aStateNew )
        {
        case EUsbDeviceStatePowered:
            // Case for Attached state missed 
            // NO break here;
        case EUsbDeviceStateAttached:
            {
            if( EUsbDeviceStateUndefined != aStateOld )
                {
                LOG1( "Not starting personality, previous state: %d", 
                    aStateOld);
                break;
                }
			LOG1( "Starting USB personality in device state: %d", aStateNew );
            iPersonalityHandler->StateChangeNotify( aStateOld, aStateNew );
            // Check AskOnConnection setting every time
            if( ( iSupportedPersonalities.Count() > 1 ) &&
                    !IsAskOnConnectionSuppression() )
                {
                // read setting if there is more than one personality
                iPersonalityRepository->Get(
                        KUsbWatcherChangeOnConnectionSetting,
                        iAskOnConnectionSetting );
                }

            if( ( iState == EUsbIdle ) && !iPersonalityChangeOngoing )
                {
                Start();
                }
            else if( ( iState != EUsbStarted ) && !iPersonalityChangeOngoing )
                {
                Cancel();
                Start();
                }

            // Let's turn ask on connection off after start cause we will
            // issue it only when cable is connected
            iAskOnConnectionSetting = KUsbWatcherChangeOnConnectionOff;

            //start usbdevcon only in normal global state
            TInt globalState =
                    CUsbGlobalSystemStateObserver::GlobalSystemState();
            if( ( ESwStateNormalRfOn == globalState ) ||
                    ( ESwStateNormalRfOff == globalState ) ||
                    ( ESwStateNormalBTSap == globalState ) )
                {
                iUsbDevConStarter->Start();
                }
            }
            break;

        case EUsbDeviceStateUndefined:
            {
            iGlobalStateObserver->Cancel();
            // Notify clients currently loaded personality will be unloaded
            iPersonalityHandler->StateChangeNotify( aStateOld, aStateNew );
            iUsbDevConStarter->StopRestarting();
            // This must be done before Notify()
            if( iSetPreviousPersonalityOnDisconnect )
                {
                TInt temp = iPersonalityId;
                iPersonalityId = iPrevPersonalityId;
                iPrevPersonalityId = temp;
                WritePersonalityId( iPersonalityId );
                iSetPreviousPersonalityOnDisconnect = EFalse;
                }

            Notify( KErrCancel );
            if (iState == EUsbStarting)
                {
                LOG( "StartCancel USB" );
                Cancel();
                }
            else
                {
                LOG( "Stop USB" );
                
                if( EUsbConfirmStop == iState )
                    {
                    // We have request pending on personality handler
                    Cancel();
                    }
                
                    Stop();
                    }
            }
            break;

        default:
            if ( EUsbStarted == iState )
                {
                iPersonalityHandler->StateChangeNotify( aStateOld, aStateNew );
                }
            break;
        }
    }

// ----------------------------------------------------------------------------
// From class CActive.
// This method cancels any outstanding request.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::DoCancel()
    {
    LOG_FUNC

    LOG1( "WatcherState = %d", iState );
    if( ( EUsbStarting == iState ) || ( EUsbStopping == iState ) )
        {
        iPersonalityHandler->Cancel();

        RProperty::Set( KPSUidUsbWatcher,
                KUsbWatcherSelectedPersonality,
                KUsbWatcherSelectedPersonalityNone );
        LOG( "personality set to none" );

        iState = EUsbIdle;
        }
    else if( EUsbConfirmStop == iState )
        {
        iPersonalityHandler->Cancel();
        iState = EUsbStarted;
        }
    else
        {
        LOG( "Nothingh to cancel" );
        }
    }

// ----------------------------------------------------------------------------
// This method implements state machine for personality start and stop.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::RunL()
    {
    LOG_FUNC

    LOG1( "WatcherState = %d", iState );
    TInt ret = iStatus.Int();
    if ( KErrNone != ret  )
        {
        LOG1( "ERROR: CUsbWatcher::RunL iStatus = %d", ret );
        }

    switch ( iState )
        {
        case EUsbStarting:
            LOG( "Personality started" );
            Notify( ret );
            iState = EUsbStarted;
            if( iStopStartScenario )
                {
                iStopStartScenario = EFalse;
                }
            break;

        case EUsbStopping:
            LOG( "Personality stopped" );
            iState = EUsbIdle;
            if( iStopStartScenario )
                {
                Start();
                }
            else
                {
                RProperty::Set( KPSUidUsbWatcher,
                    KUsbWatcherSelectedPersonality,
                    KUsbWatcherSelectedPersonalityNone );
                }
            break;

        case EUsbStarted:
            if( iStopStartScenario )
                {
                break;
                }

            iPersonalityHandler->StopPersonality( iStatus );
            SetActive();
            iState = EUsbStopping;
            break;

        case EUsbIdle:
            if( iStopStartScenario )
                {
                Start();
                }
            break;

        case EUsbConfirmStop:
            if( iStatus == KErrNone )
                {
                iStopStartScenario = ETrue;
                iPersonalityHandler->StopPersonality( iStatus );
                SetActive();
                iState = EUsbStopping;
                }
            else
                {
                Notify( ret );
                iState = EUsbStarted;
                }
            break;

        default:
            LOG( "ERROR: unexpected state" );
            PANIC( KErrGeneral );
            break;
        }
    }

// ----------------------------------------------------------------------------
// This method is not called cause RunL() never leaves.
// ----------------------------------------------------------------------------
//
TInt CUsbWatcher::RunError(TInt /*aError*/)
    {
    LOG_FUNC
    // Left empty cause this can't happend
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Function is called when state of the device is locked.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::Lock()
    {
    LOG_FUNC

    if( IsDeviceA() ) // Not applicable in case of A-device
        {
        LOG( "Lock ignored in A-device state" );
        return;
        }

    
    TUsbDeviceState state = iActiveState->CurrentState();

    LOG1( "USB device state after lock: %d", state );

    if( EUsbDeviceStateUndefined != state ) // Stop personality
        {
        LOG( "Stopping USB persoanlity on device remote-lock" );

        iPersonalityHandler->CancelCableConnectedNotifier();

        if( iState != EUsbStarted )
            {
            Cancel();
            }
        else
            {
            Stop();
            }
        }
    }

// ----------------------------------------------------------------------------
// Function is called when state of the device is unlocked.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::Unlock()
    {
    LOG_FUNC

    if( IsDeviceA() ) // Not applicable in case of A-device
        {
        LOG( "Unlock ignored in A-device state" );
        return;
        }

    TUsbDeviceState state = iActiveState->CurrentState();

    LOG1( "USB device state after unlock: %d", state );
    if( EUsbDeviceStateAttached == state || EUsbDeviceStatePowered == state)
        {
        LOG( "Starting USB personality" );
        TInt err = iPersonalityRepository->Get(
           KUsbWatcherChangeOnConnectionSetting, iAskOnConnectionSetting );
        if( KErrNone == err )
            {
            Start();
            iAskOnConnectionSetting = KUsbWatcherChangeOnConnectionOff;
            }
        else
            {
            LOG1( "Error: CRepository::Get = %d", err );
            }
        }
    }

// ----------------------------------------------------------------------------
// This method is called when client wants to set new personality.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::SetPersonality( TInt aPersonalityId, TBool aNonBlocking )
    {
    LOG_FUNC

    LOG2( "aPersonalityId=%d, aNonBlocking=%d", aPersonalityId, aNonBlocking );

    if( IsDeviceA() ) 
        {
        LOG( "SetPersonality not allowed in A-device state" );
        Notify( KErrAccessDenied );
        return;
        }
    
    // Check if personality is exist
    TBool found = EFalse;

    for( TInt i = 0; i < iSupportedPersonalities.Count(); i++ )
        {
        if( iSupportedPersonalities[i].iPersonalityId == aPersonalityId )
            {
            found = ETrue;
            }
        }

    if( !found )
        {
        Notify( KErrNotFound );
        return;
        }

    iSetPersonalityOngoing = ETrue;
    iPersonalityChangeOngoing = ETrue;

    //The previous personality is not changed, if the client wanted the
    //previous personality change to be temporary. The client has to call
    //SetPreviousPersonalityOnDisconnect after each SetPersonality to be
    //intended as temporary.
    if( iSetPreviousPersonalityOnDisconnect )
        {
        iSetPreviousPersonalityOnDisconnect = EFalse;
        }
    else
        {
        iOldPrevPersonalityId = iPrevPersonalityId;
        iPrevPersonalityId = iPersonalityId;
        LOG( "CUsbWatcher::SetPersonality setting previous" );
        }
    LOG1( " iPrevPersonalityId = %d", iPrevPersonalityId );
    if( iPersonalityId != aPersonalityId )
        {
        iPersonalityId = aPersonalityId;
        SwitchPersonality( aNonBlocking );
        }
    else
        {
        Notify( KErrNone );
        }
    }

// ----------------------------------------------------------------------------
// This method is called when client wants to cancel set personality.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::CancelSetPersonality()
    {
    LOG_FUNC

    Notify( KErrCancel );
    iPersonalityChangeOngoing = ETrue;
    iStopStartScenario = EFalse;
    SwitchPersonality();
    }

// ----------------------------------------------------------------------------
// This method is called when client wants to set previous personality.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::SetPreviousPersonality()
    {
    LOG_FUNC

    if( IsDeviceA() ) 
        {
        LOG( "SetPreviousPersonality not allowed in A-device state" );
        Notify( KErrAccessDenied );
        return;
        }
    
    TInt temp = iPersonalityId;
    iSetPreviousPersonalityOnDisconnect = EFalse;

    iPersonalityId = iPrevPersonalityId;
    iPrevPersonalityId = temp;
    iSetPreviousPersonalityOngoing = ETrue;
    iPersonalityChangeOngoing = ETrue;

    if( iPersonalityId != iPrevPersonalityId )
        {
        SwitchPersonality();
        }
    else
        {
        Notify( KErrNone );
        }
    }

// ----------------------------------------------------------------------------
// This method is called when client wants to cancel set previous personality.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::CancelSetPreviousPersonality()
    {
    LOG_FUNC

    Notify( KErrCancel );
    iPersonalityChangeOngoing = ETrue;
    iStopStartScenario = EFalse;
    SwitchPersonality();
    }

// ----------------------------------------------------------------------------
// This method is called when client wants to set previous personality on
// disconnect.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::SetPreviousPersonalityOnDisconnect()
    {
    LOG_FUNC

    if( IsDeviceA() ) 
        {
        LOG( "Not allowed in A-device state" );
        Notify( KErrAccessDenied );
        return;
        }

    TUsbDeviceState state = iActiveState->CurrentState();
    LOG1( "Device state : %d", state );
    if( state != EUsbDeviceStateUndefined )
        {
        iSetPreviousPersonalityOnDisconnect = ETrue;
        }
    }

// ----------------------------------------------------------------------------
// This method starts personality change, if there is cable connected.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::SwitchPersonality( TBool aNonBlocking )
    {
    LOG_FUNC

    if( IsDeviceA() ) 
        {
        LOG( "Not allowed in A-device state" );
        Notify( KErrAccessDenied );
        return;
        }

    TUsbDeviceState state = iActiveState->CurrentState();
    LOG1( "Device state : %d", state );

    if( state != EUsbDeviceStateUndefined )
        {
        switch ( iState )
            {
            case EUsbStopping:
            case EUsbStarting:
                {
                LOG( "CUsbWatcher::SwitchPersonality: Cancel & Start USB" );
                Cancel();
                Start();
                }
                break;
            case EUsbConfirmStop:
                {
                Cancel();
                iState = EUsbStarted;
                StopStart( aNonBlocking );
                }
                break;
            case EUsbStarted:
                {
                LOG( "CUsbWatcher::SwitchPersonality: Stop & Start USB" );
                StopStart( aNonBlocking );
                }
                break;
            default:
                {
                LOG( "CUsbWatcher::SwitchPersonality: Start USB" );
                Start();
                }
                break;
            }
        }
    else
        {
        // if no connection -> just save the setting
        LOG( "CUsbWatcher::SwitchPersonality: Notify" );
        Notify( KErrNone );
        }
    }

// ----------------------------------------------------------------------------
// This method starts personality loading or continues stop start scenario.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::Start()
    {
    LOG_FUNC

    TInt globalState = CUsbGlobalSystemStateObserver::GlobalSystemState();

    if( iState == EUsbIdle )
        {
        iStarted = EFalse;
        if( globalState == ESwStateCharging )
            {
            LOG( "Global state: charging" );
            iGlobalStateObserver->Subscribe();
            iPrevPersonalityId=iPersonalityId;
            TInt ret = GetChargingPersonalityId( iPersonalityId );
            //do not start if charging personality not assigned
            if( KErrNone == ret )
                {
                RProperty::Set( KPSUidUsbWatcher,
                        KUsbWatcherSelectedPersonality, iPersonalityId );
                iStarted = ETrue;
                // Restore personality to normal in charging mode
                iSetPreviousPersonalityOnDisconnect = ETrue;
                iPersonalityHandler->StartPersonality( iPersonalityId,
                    KUsbWatcherChangeOnConnectionOff, iStatus );
                }
            else
                {
                LOG1( "GetChargingPersonalityId = %d. Not started", ret );
                }
            }
        else if( ( ( ESwStateNormalRfOn == globalState ) ||
                   ( ESwStateNormalRfOff == globalState ) ||
                   ( ESwStateNormalBTSap == globalState ) ))
            {
            LOG( "Global state: normal" );
            if( ! iUsbDeviceLock->Locked() )
                {
                iGlobalStateObserver->Cancel();
                RProperty::Set( KPSUidUsbWatcher,
                            KUsbWatcherSelectedPersonality, iPersonalityId );
                iStarted = ETrue;
                iPersonalityHandler->StartPersonality( iPersonalityId,
                        iAskOnConnectionSetting, iStatus );
                }
            else
                {
                LOG( "Device LOCKED, USB personality NOT start" );
                }
            }
        else
            {
            LOG1( "Global state: = %d", globalState );
            //Cable connected in e.g. ChargingToNormal state and
            // personality started later in a normal state.
            iGlobalStateObserver->Subscribe();
            }
        if( iStarted )
            {
            SetActive();
            iState = EUsbStarting;
            }
        }
    else
        {
        LOG( "Tryign to call CUsbWatcher::Start in non-idle state " );
        PANIC( KErrGeneral );
        }
    }

// ----------------------------------------------------------------------------
// This method starts personality unloading or cancels personality start.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::Stop()
    {
    LOG_FUNC

    LOG1( "WatcherState = %d", iState );
    if( EUsbStarted == iState )
        {
        iPersonalityHandler->StopPersonality( iStatus );
        SetActive();
        iState = EUsbStopping;
        
        }
    else if( EUsbStarting  == iState )
        {
        LOG( "Cancel ongoing start." );
        Cancel();
        }
    
    else
        {
        LOG( "Wrong state for Stop" );
        }
    }

// ----------------------------------------------------------------------------
// This method is used when there is need to change currently loaded
// personality.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::StopStart( TBool aNonBlocking )
    {
    LOG_FUNC

    LOG1( "WatcherState = %d", iState );
    if( iState == EUsbStarted )
        {
        iState = EUsbConfirmStop;
        if( !aNonBlocking )
            {
            iPersonalityHandler->ConfirmPersonalityUnload( iStatus );
            SetActive();
            }
        else
            {
            LOG( "CUsbWatcher::StopStart not confirming" );
            SetActive();
            TRequestStatus* status = &iStatus;
            User::RequestComplete( status, KErrNone );
            }
        }
    else
        {
        LOG( "Wrong state to StopStart" );
        }
    }

// ----------------------------------------------------------------------------
// CUsbWatcherSession uses this method to register observer.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::RegisterObserverL( MUsbWatcherNotify* aObserver )
    {
    LOG_FUNC

    TInt index = iObservers.Find( aObserver );
    if( index < 0 )
        {
        iObservers.AppendL( aObserver );
        }
    }

// ----------------------------------------------------------------------------
// CUsbWatcherSession uses this method to deregister observer.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::DeRegisterObserver( MUsbWatcherNotify* aObserver )
    {
    LOG_FUNC

    TInt index = iObservers.Find( aObserver );

    if( index >= 0 )
        {
        iObservers.Remove( index );
        }
    }

// ----------------------------------------------------------------------------
// This method is used to complete any outstanding request.
// ----------------------------------------------------------------------------
//
void CUsbWatcher::Notify( TInt aStatus )
    {
    LOG_FUNC

    LOG1( "aStatus = %d", aStatus );
    TInt status = aStatus;

    // clear every time when notified
    iPersonalityChangeOngoing = EFalse;

    if( iSetPersonalityOngoing || iChangePersonalityOngoing )
        {
        iSetPersonalityOngoing = EFalse;
        iChangePersonalityOngoing = EFalse;

        if( status == KErrNone )
            {
            status = WritePersonalityId( iPersonalityId );
            }
        else
            {
            // in case of error return to state before SetPersonality
            iPersonalityId = iPrevPersonalityId;
            iPrevPersonalityId = iOldPrevPersonalityId;
            }
        }

    if( iSetPreviousPersonalityOngoing )
        {
        iSetPreviousPersonalityOngoing = EFalse;

        if( status == KErrNone )
            {
            WritePersonalityId( iPersonalityId );
            }
        else
            {
            // in case of error return to state before SetPreviousPersonality
            TInt temp = iPrevPersonalityId;

            iPrevPersonalityId = iPersonalityId;
            iPersonalityId = temp;
            }
        }

    for( TInt i = 0; i < iObservers.Count(); i++ )
        {
        iObservers[i]->Notify( iPersonalityId, status );
        }
    }

// ----------------------------------------------------------------------------
// Stop loaded personality. Called from global state handler
// ----------------------------------------------------------------------------
//
void CUsbWatcher::StopPersonality()
    {
    LOG_FUNC
    // restore settings
    iPersonalityId = iPrevPersonalityId;

    Stop();
    iStarted = EFalse;
    }

// ----------------------------------------------------------------------------
// Start personality. Called from global state handler
// ----------------------------------------------------------------------------
//
void CUsbWatcher::StartPersonality()
    {
    LOG_FUNC

    if( !iStarted )
        {
        Start();
        }

    //start usbdevcon only in normal global state
    TInt globalState = CUsbGlobalSystemStateObserver::GlobalSystemState();
    if( ( globalState == ESwStateNormalRfOn ) ||
            ( globalState == ESwStateNormalRfOff ) ||
            ( globalState == ESwStateNormalBTSap ) )
        {
        iUsbDevConStarter->Start();
        }
    }

// ----------------------------------------------------------------------------
// Read default personality from ini file. Used in charging mode
// ----------------------------------------------------------------------------
//
TInt CUsbWatcher::GetChargingPersonalityId( TInt& aPersonalityId )
    {
    LOG_FUNC

    TInt ret = iPersonalityRepository->Get(
            KUsbWatcherChargingDefaultPersonality, aPersonalityId );
    return ret;
    }

// ----------------------------------------------------------------------------
// Check if there is an observer with ask on connection suppression
// ----------------------------------------------------------------------------
//
TBool CUsbWatcher::IsAskOnConnectionSuppression()
    {
    LOG_FUNC
    
    TBool ret( EFalse );
    for( TInt i = 0; i < iObservers.Count(); i++ )
        {
        if( iObservers[i]->IsAskOnConnectionSuppressed() )
            {
            ret = ETrue;
            break;
            }
        }
    LOG1( "Return = %d", ret );
    return ret;
    }

// ----------------------------------------------------------------------------
// Check current A or B device state
// ----------------------------------------------------------------------------
//
TBool CUsbWatcher::IsDeviceA()
    {
    //NOT LOGGED
    // return EFalse in non-OTG configuration otherwise ask UsbOtgWatcher
    return iOtgWatcher ? iOtgWatcher->IsDeviceA() : EFalse;
    }

// End of file
