/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "UsbWatcherTest.h"
#include "testdebug.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUsbWatcherTest::TestConstructL
// Construct here all test resources for test methods. 
// Called from ConstructL. 
// -----------------------------------------------------------------------------

void CUsbWatcherTest::TestConstructL()
    {
    TRACE_FUNC_ENTRY
    
    iStubber = CStubber::NewL();

    iWatcher = new ( ELeave ) RUsbWatcher();
    iCenRep = CRepository::NewL( KCRUidUsbWatcher );
    STIF_LOG( "Usb man initialization" );
    iUsbMan = new ( ELeave ) RUsb();
    User::LeaveIfError( iUsbMan -> Connect() );  
    User::LeaveIfError( BackupPersonalitySetting() );
    
    TRACE_FUNC_EXIT
    }

// -----------------------------------------------------------------------------
// CUsbWatcherTest::TestDelete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------

void CUsbWatcherTest::TestDelete() 
    {
    TRACE_FUNC_ENTRY

    if ( RestorePersonalitySetting() != KErrNone )
        STIF_LOG( "Warning: personality restore failed" );
    
    delete iWatcher;
    iWatcher = NULL;
    
    delete iCenRep;
    iCenRep = NULL;
    
    iUsbMan -> Close();
    delete iUsbMan;
    iUsbMan = NULL;
    
    delete iStubber;
    iStubber = NULL;
    
    TRACE_FUNC_EXIT
    }

// -----------------------------------------------------------------------------
// CUsbWatcherTest::TestBlocksInfoL
// Add new test block parameter entries below
// -----------------------------------------------------------------------------

void CUsbWatcherTest::TestBlocksInfoL()
    {
    
    TESTENTRY( "ExampleTestL", CUsbWatcherTest::ExampleTestL )
            {
            TESTPARAM( eGetString, eTestOption1 )
            TESTPARAM( eGetString, eTestOption2 )
            TESTPARAM( eGetInt, eTestIntOption1 )
            TESTPARAM( eGetChar, eTestCharOption1 )
            }
    
    TESTENTRY( "InitializationTest", CUsbWatcherTest::InitializationTestL )
            {
            TESTPARAM( eGetString, eTestOption1 )
            }

    TESTENTRY( "PersonalityManagementTest", CUsbWatcherTest::PersonalityManagementTestL )
            {
            TESTPARAM( eGetString, eTestOption1 )
            TESTPARAM( eGetString, eTestOption2 )
            }
    
    TESTENTRY( "PersonalitySwitchTest", CUsbWatcherTest::PersonalitySwitchTestL )
            {
            TESTPARAM( eGetString, eTestOption1 )
            }

    TESTENTRY( "BranchTest", CUsbWatcherTest::BranchTestL )
            {
            TESTPARAM( eGetString, eTestOption1 )
            }
    }

// Add test block methods implementation here
// -----------------------------------------------------------------------------
// CUsbWatcherTest::ExampleTestL
// -----------------------------------------------------------------------------

void CUsbWatcherTest::ExampleTestL( TUsbWatcherTestBlockParams& aParams, TUsbWatcherTestResult& aTestResult )
    {
    TRACE_FUNC_ENTRY
    
    //iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );

    if ( !aParams.iTestOption1.Compare( _L( "API" ) ) )
        {
		TRACE_INFO( "Api test option: %S", &aParams.iTestOption1 )
		TRACE_INFO( "Api test sub-option: %S", &aParams.iTestOption2 )
		TRACE_INFO( "Api test int option: %d", aParams.iTestIntOption1 )
		TRACE_INFO( "Api test char option: %c", TText( aParams.iTestCharOption1 ) )
        }
    else if ( !aParams.iTestOption1.Compare( _L( "MODULE" ) ) )
        {
		TRACE_INFO( "Module test option: %S", &aParams.iTestOption1 )
		TRACE_INFO( "Module test sub-option: %S", &aParams.iTestOption2 )
		TRACE_INFO( "Module test int option: %d", aParams.iTestIntOption1 )
		TRACE_INFO( "Module test char option: %c", TText( aParams.iTestCharOption1 ) )
        }
    else if ( !aParams.iTestOption1.Compare( _L( "BRANCH" ) ) )
        {
		TRACE_INFO( "Branch test option: %S", &aParams.iTestOption1 )
		TRACE_INFO( "Branch test sub-option: %S", &aParams.iTestOption2 )
		TRACE_INFO( "Branch test int option: %d", aParams.iTestIntOption1 )
		TRACE_INFO( "Branch test char option: %c", TText( aParams.iTestCharOption1 ) )
        }
    else
        {
        TRACE( "Invalid test parameter" )
        TRACE_FUNC_EXIT
        User::Leave( KErrNotFound );
        }
    
    aTestResult = ETestCasePassed;
    TRACE_FUNC_EXIT
    }


// Add other member functions implementation here

// -----------------------------------------------------------------------------
// CUSBWatcherApiTest::InitializationTest
// -----------------------------------------------------------------------------

void CUsbWatcherTest::InitializationTestL( TUsbWatcherTestBlockParams& aParams, 
                                            TUsbWatcherTestResult& aTestResult )
    {
    TInt res;   
    TPtrC testOptionName( aParams.iTestOption1 );
        
    if ( !testOptionName.Compare( _L( "VERSION" ) ) )
        {
        TRACE( "Test option: VERSION" );
        TVersion returnedVersion;
        TVersion expectedVersion( KUsbWatcherSrvMajorVersionNumber,
                                KUsbWatcherSrvMinorVersionNumber,
                                KUsbWatcherSrvBuildVersionNumber );
        TRACE( "  >Version" );
        returnedVersion = iWatcher -> Version();
        TRACE( "  <Version" );
        if( expectedVersion.iMajor != returnedVersion.iMajor )
            User::Leave(KErrNotFound);
        if( expectedVersion.iMinor != returnedVersion.iMinor )
            User::Leave(KErrNotFound);
        if( expectedVersion.iBuild != returnedVersion.iBuild )
            User::Leave(KErrNotFound);
        }
    else if ( !testOptionName.Compare( _L( "SINGLE_CONN" ) ) )
        {
        TRACE( "Test option: SINGLE_CONN" );
        TRACE( ">Connect" );
        res = iWatcher -> Connect();
        TRACE( "  <Connect" );
        TRACE_INFO( "USB watcher connection value: %d, expected: %d", res, KErrNone );
        User::LeaveIfError( res );
        }
    else
        {
        TRACE( "Test option: not supported" );
        User::Leave( KErrNotFound );
        }
        
    aTestResult = ETestCasePassed;
    }

// -----------------------------------------------------------------------------
// CUSBWatcherApiTest::PersonalityManagementTest
// -----------------------------------------------------------------------------

void CUsbWatcherTest::PersonalityManagementTestL( TUsbWatcherTestBlockParams& aParams, 
                                                    TUsbWatcherTestResult& aTestResult )
    {
    TInt res;
    TInt newPersonalityId;
    TPtrC testOptionName( aParams.iTestOption2 );
    TPtrC usbPersonalityName( aParams.iTestOption1 );
    TInt currentPersonalityId;
    
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetPersonalityProperty, 0, 0, _L8( "stub" ) ) );
    
    res = iUsbMan -> GetCurrentPersonalityId( currentPersonalityId );
    if ( res != KErrNone )
        {
        TRACE_INFO( "GetCurrentPersonalityId failed with value: %d", res );
        User::Leave(res);
        }
        
    res = GetPersonalityIdFromString( usbPersonalityName, newPersonalityId );
    if ( res != KErrNone )
        {
        TRACE_INFO( "GetPersonalityIdFromString failed with value: %d", res );
        User::Leave(res);
        }
    
    TRACE( "  >Connect" );
    res = iWatcher -> Connect();
    TRACE( "  <Connect" );
    TRACE_INFO( "USB watcher connection value: %d, expected: %d", res, KErrNone );
    User::LeaveIfError( res );
    
    if ( !testOptionName.Compare( _L( "SET_PERSONALITY" ) ) && newPersonalityId == KUsbPersonalityIdPCSuiteMTP )
        {     
        TRequestStatus status;
        TInt setPersonalityId;
        
        if ( currentPersonalityId != newPersonalityId )
            {
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EDeviceStateNotification, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EDeviceStateNotification, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EDeviceStateNotification, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
            }
                
        res = iUsbMan -> GetCurrentPersonalityId( currentPersonalityId );
        
        TRACE( "  >SetPersonality" );
        iWatcher -> SetPersonality( status, KUsbPersonalityIdMS, EFalse, EFalse );
        User::WaitForRequest( status );
        TRACE( "  <SetPersonality" );
        TRACE_INFO( "Set initial test personality value: %d", status.Int() );
        res = iCenRep -> Get( KUsbWatcherPersonality, setPersonalityId );
        TRACE_INFO( "Set initial test personality value: %d, personality ID: %d", status.Int(), setPersonalityId );
        User::LeaveIfError( status.Int() );
        if( KUsbPersonalityIdMS != setPersonalityId )
            User::Leave(KErrNotFound); 
        }
    else
        {
        TRequestStatus status;
        TInt setPersonalityId;
        
        if ( currentPersonalityId != newPersonalityId )
            {
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EDeviceStateNotification, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
            }
        
        TRACE( "  >SetPersonality" );
        iWatcher -> SetPersonality( status, KUsbPersonalityIdPCSuiteMTP, EFalse, EFalse );
        User::WaitForRequest( status );        
        TRACE( "  <SetPersonality" );
        res = iCenRep -> Get( KUsbWatcherPersonality, setPersonalityId );
        TRACE_INFO( "Set initial test personality value: %d, personality ID: %d", status.Int(), setPersonalityId );
        User::LeaveIfError( status.Int() ); 
        if( KUsbPersonalityIdPCSuiteMTP != setPersonalityId )
            User::Leave(KErrNotFound);
        }
        
    if ( !testOptionName.Compare( _L( "SET_PERSONALITY" ) ) )
        {
        TRACE( "Test option: SET_PERSONALITY" );
        aTestResult = SetPersonalityTest( newPersonalityId, ETrue );
        }
    else if ( !testOptionName.Compare( _L( "CANCEL_SET_PERSONALITY" ) ) )
        {
        TRACE( "Test option: CANCEL_SET_PERSONALITY" );
        aTestResult = CancelSetPersonalityTest( newPersonalityId, ETrue );
        }
    else if ( !testOptionName.Compare( _L( "SET_PREVIOUS_PERSONALITY" ) ) )
        {
        TRACE( "Test option: SET_PREVIOUS_PERSONALITY" );
        aTestResult = SetPreviousPersonalityTest( newPersonalityId, ETrue );
        }
    else if ( !testOptionName.Compare( _L( "SET_PREVIOUS_PERSONALITY_ON_DISC" ) ) )
        {
        TRACE( "Test option: SET_PREVIOUS_PERSONALITY_ON_DISC" );
        aTestResult = SetPreviousPersonalityOnDiscTest( newPersonalityId, ETrue );
        }
    else if ( !testOptionName.Compare( _L( "SET_PREVIOUS_PERSONALITY_SYNC" ) ) )
        {
        TRACE( "Test option: SET_PREVIOUS_PERSONALITY_SYNC" );
        aTestResult = SetPreviousPersonalitySyncTest( newPersonalityId, ETrue );
        }
    else if ( !testOptionName.Compare( _L( "CANCEL_SET_PREVIOUS_PERSONALITY" ) ) )
        {
        TRACE( "Test option: CANCEL_SET_PREVIOUS_PERSONALITY" );
        aTestResult = CancelSetPreviousPersonalityTest( newPersonalityId, ETrue );
        }
    else if ( !testOptionName.Compare( _L( "SET_PERSONALITY_DISC" ) ) )
        {
        TRACE( "Test option: SET_PERSONALITY_DISC" );
        aTestResult = SetPersonalityTest( newPersonalityId, EFalse );
        }
    else if ( !testOptionName.Compare( _L( "CANCEL_SET_PERSONALITY_DISC" ) ) )
        {
        TRACE( "Test option: CANCEL_SET_PERSONALITY_DISC" );
        aTestResult = CancelSetPersonalityTest( newPersonalityId, EFalse );
        }
    else if ( !testOptionName.Compare( _L( "SET_PREVIOUS_PERSONALITY_DISC" ) ) )
        {
        TRACE( "Test option: SET_PREVIOUS_PERSONALITY_DISC" );
        aTestResult = SetPreviousPersonalityTest( newPersonalityId, EFalse );
        }
    else if ( !testOptionName.Compare( _L( "SET_PREVIOUS_PERSONALITY_ON_DISC_DISC" ) ) )
        {
        TRACE( "Test option: SET_PREVIOUS_PERSONALITY_ON_DISC_DISC" );
        aTestResult = SetPreviousPersonalityOnDiscTest( newPersonalityId, EFalse );
        }
    else if ( !testOptionName.Compare( _L( "SET_PREVIOUS_PERSONALITY_SYNC_DISC" ) ) )
        {
        TRACE( "Test option: SET_PREVIOUS_PERSONALITY_SYNC_DISC" );
        aTestResult = SetPreviousPersonalitySyncTest( newPersonalityId, EFalse );
        }
    else if ( !testOptionName.Compare( _L( "CANCEL_SET_PREVIOUS_PERSONALITY_DISC" ) ) )
        {
        TRACE( "Test option: CANCEL_SET_PREVIOUS_PERSONALITY_DISC" );
        aTestResult = CancelSetPreviousPersonalityTest( newPersonalityId, EFalse );
        }
    else if ( !testOptionName.Compare( _L( "SET_UNKNOWN_PERSONALITY" ) ) )
        {
        TRACE( "Test option: SET_UNKNOWN_PERSONALITY" );
        aTestResult = SetUnknownPersonalityTest();
        }
    else if ( !testOptionName.Compare( _L( "INTERRUPT_PERSONALITY_SET" ) ) )
        {
        TRACE( "Test option: INTERRUPT_PERSONALITY_SET" );
        aTestResult = InterruptPersonalitySetTest( newPersonalityId );
        }
    else
        {
        TRACE( "Test option: not supported" );
        User::Leave(KErrNotFound);
        }
    }

TUsbWatcherTestResult CUsbWatcherTest::SetPersonalityTest( TInt aNewPersonalityId, TBool aCableConnected )
    {
    TRequestStatus stat;
    TInt currentPersonalityId;
    TInt res;
    
    TRACE( "Checking test results:" );
    
    if ( aCableConnected )
        {
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
        }
    
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( stat, aNewPersonalityId, EFalse, EFalse );   
    User::WaitForRequest( stat );
    TRACE( "  <SetPersonality" );
    TRACE_INFO("Set personality request status: %d, expected %d", stat.Int(), KErrNone );
    
    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, aNewPersonalityId );
    
    if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != aNewPersonalityId )
        return ETestCaseFailed;
    
    return ETestCasePassed;    
    }

TUsbWatcherTestResult CUsbWatcherTest::CancelSetPersonalityTest( TInt aNewPersonalityId, TBool aCableConnected )
    {
    TRequestStatus stat;
    TInt previousPersonalityId;
    TInt currentPersonalityId;
    TInt res;
    
    res = iCenRep -> Get( KUsbWatcherPersonality, previousPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Original personality ID: %d", previousPersonalityId );
    if ( res != KErrNone )
        return ETestCaseFailed;
    
    TRACE( "Checking test results:" );
    
    if ( aCableConnected )
        {
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EDeviceStateNotification, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EDeviceStateNotification, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetPersonalityProperty, 0, 0, _L8( "stub" ) ) );
        }
    
    
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( stat, aNewPersonalityId, EFalse, EFalse );
    TRACE( "  >CancelSetPersonality" );
    iWatcher -> CancelSetPersonality();
    User::WaitForRequest( stat );
    TRACE( "  <CancelSetPersonality" );
    TRACE_INFO("Cancel set personality request status: %d", stat.Int() );
     
    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d", currentPersonalityId );
    
    if ( aCableConnected )
        {
        if ( stat.Int() != KErrCancel || res != KErrNone || currentPersonalityId != previousPersonalityId )
            return ETestCaseFailed;
        }
    else
        {
        if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != aNewPersonalityId )
            return ETestCaseFailed;
        }
    
    return ETestCasePassed; 
    }

TUsbWatcherTestResult CUsbWatcherTest::SetPreviousPersonalityTest( TInt aNewPersonalityId, TBool aCableConnected )
    {
    TRequestStatus stat;
    TInt previousPersonalityId;
    TInt currentPersonalityId;
    TInt res;
    
    res = iCenRep -> Get( KUsbWatcherPersonality, previousPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Original personality ID: %d", previousPersonalityId );
    if ( res != KErrNone )
        return ETestCaseFailed;
    
    TRACE( "  >SetPersonality" );
    
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EDeviceStateNotification, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
    
    iWatcher -> SetPersonality( stat, aNewPersonalityId, EFalse, EFalse );
    User::WaitForRequest( stat );
    TRACE( "  <SetPersonality" );
    TRACE_INFO("Set personality request status: %d, expected %d", stat.Int(), KErrNone );
    
    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, aNewPersonalityId );
    
    if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != aNewPersonalityId )
        return ETestCaseFailed;
    
    TRACE( "Checking test results:" );
    
    if ( aCableConnected )
        {
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EDeviceStateNotification, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
        }
    
    TRACE( "  >SetPreviousPersonality" );
    iWatcher -> SetPreviousPersonality( stat );
    User::WaitForRequest( stat );
    TRACE( "  <SetPreviousPersonality" );
    TRACE_INFO("Set previous personality request status: %d, expected %d", stat.Int(), KErrNone );
    
    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, previousPersonalityId );
    
    if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != previousPersonalityId )
        return ETestCaseFailed;
    
    return ETestCasePassed; 
    }

TUsbWatcherTestResult CUsbWatcherTest::SetPreviousPersonalityOnDiscTest( TInt aNewPersonalityId, TBool aCableConnected )
    {
    TRequestStatus stat;
    TInt previousPersonalityId;
    TInt currentPersonalityId;
    TInt res;
    
    res = iCenRep -> Get( KUsbWatcherPersonality, previousPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Original personality ID: %d", previousPersonalityId );
    if ( res != KErrNone )
        return ETestCaseFailed;
    
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EDeviceStateNotification, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
    
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( stat, aNewPersonalityId, EFalse, EFalse );
    User::WaitForRequest( stat );
    TRACE( "  <SetPersonality" );
    TRACE_INFO("Set personality request status: %d, expected %d", stat.Int(), KErrNone );
    
    if ( aCableConnected )    
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
    
    TRACE( "  >SetPreviousPersonalityOnDisconnect" );   
    iWatcher -> SetPreviousPersonalityOnDisconnect();
    TRACE( "  <SetPreviousPersonalityOnDisconnect" );
    
    // disconnect cable here
    
    TRACE( "Checking test results:" );
    
    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, aNewPersonalityId );
    
    if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != aNewPersonalityId )
        return ETestCaseFailed;
    
    return ETestCasePassed; 
    }

TUsbWatcherTestResult CUsbWatcherTest::SetPreviousPersonalitySyncTest( TInt aNewPersonalityId, TBool aCableConnected )
    {
    TRequestStatus stat;
    TInt previousPersonalityId;
    TInt currentPersonalityId;
    TInt res;
    
    res = iCenRep -> Get( KUsbWatcherPersonality, previousPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Original personality ID: %d", previousPersonalityId );
    if ( res != KErrNone )
        return ETestCaseFailed;
    
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
    
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( stat, aNewPersonalityId, EFalse, EFalse );
    User::WaitForRequest( stat );
    TRACE( "  <SetPersonality" );
    TRACE_INFO("Set personality request status: %d, expected %d", stat.Int(), KErrNone );
    
    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, aNewPersonalityId );
    
    if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != aNewPersonalityId )
        return ETestCaseFailed;
    
    TRACE( "Checking test results:" );
    
    if ( aCableConnected )
        {
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
        }
    
    TRACE( "  >SetPreviousPersonality" );
    iWatcher -> SetPreviousPersonality();
    TRACE( "  <SetPreviousPersonality" );
    TRACE("Set previous personality sync invoked"  );
    
    User::After( 10*1000*1000 );
    
    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, previousPersonalityId );
    
    if ( res != KErrNone || currentPersonalityId != previousPersonalityId )
        return ETestCaseFailed;
    
    return ETestCasePassed; 
    }

TUsbWatcherTestResult CUsbWatcherTest::CancelSetPreviousPersonalityTest( TInt aNewPersonalityId, TBool aCableConnected )
    {
    TRequestStatus stat;
    TInt previousPersonalityId;
    TInt currentPersonalityId;
    TInt res;
    
    res = iCenRep -> Get( KUsbWatcherPersonality, previousPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Original personality ID: %d", previousPersonalityId );
    if ( res != KErrNone )
        return ETestCaseFailed;
    
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
    
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( stat, aNewPersonalityId, EFalse, EFalse );
    User::WaitForRequest( stat );
    TRACE( "  <SetPersonality" );
    TRACE_INFO("Set personality request status: %d, expected %d", stat.Int(), KErrNone );
    
    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, aNewPersonalityId );
    
    if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != aNewPersonalityId )
        return ETestCaseFailed;
    
    TRACE( "Checking test results:" );
    
    if ( aCableConnected )
        {
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
        }
    
    TRACE( "  >SetPreviousPersonality" );
    iWatcher -> SetPreviousPersonality( stat );
    TRACE( "  >CancelSetPreviousPersonality" );
    iWatcher -> CancelSetPreviousPersonality();
    User::WaitForRequest( stat );
    TRACE( "  <CancelSetPreviousPersonality" );
    TRACE_INFO("Cancel set previous personality request status: %d", stat.Int() );
    
    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d", currentPersonalityId );
    
    if ( aCableConnected )
        {
        if ( stat.Int() != KErrCancel || res != KErrNone || currentPersonalityId != aNewPersonalityId )
            return ETestCaseFailed;
        }
    else
        {
        if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != previousPersonalityId )
            return ETestCaseFailed;
        }
    
    
    return ETestCasePassed; 
    }

TUsbWatcherTestResult CUsbWatcherTest::SetUnknownPersonalityTest()
    {
    TRequestStatus stat;
    TInt unknownPersonalityId = 0xFFFF;
    
    TRACE( "Checking test results:" );
    
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
    
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( stat, unknownPersonalityId, EFalse, EFalse );   
    User::WaitForRequest( stat );
    TRACE( "  <SetPersonality" );
    TRACE_INFO("Set personality request status: %d, expected %d", stat.Int(), KErrNotFound );
    
    if ( stat.Int() != KErrNotFound )
        return ETestCaseFailed;
    
    return ETestCasePassed; 
    }

TUsbWatcherTestResult CUsbWatcherTest::InterruptPersonalitySetTest( TInt aNewPersonalityId )
    {
    TRequestStatus stat;
    TRequestStatus secondStat;
    TInt previousPersonalityId;
    TInt currentPersonalityId;
    TInt res;
    
    res = iCenRep -> Get( KUsbWatcherPersonality, previousPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Original personality ID: %d", previousPersonalityId );
    if ( res != KErrNone )
        return ETestCaseFailed;
    
    TRACE( "Checking test results:" );
    
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
    
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( stat, aNewPersonalityId, EFalse, EFalse );
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( secondStat, previousPersonalityId, EFalse, EFalse );
    User::WaitForRequest( stat );
    TRACE( "  <SetPersonality" );
    User::WaitForRequest( secondStat );
    TRACE( "  <SetPersonality" );
    
    TRACE_INFO("Set personality request status: %d, expected %d", stat.Int(), KErrNone );
    TRACE_INFO("Interrupt set personality request status: %d, expected %d", secondStat.Int(), KErrNone );

    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, previousPersonalityId );
    
    if ( stat.Int() != KErrNone || secondStat.Int() != KErrNone || res != KErrNone || currentPersonalityId != previousPersonalityId )
        return ETestCaseFailed;
    
    return ETestCasePassed; 
    }

TInt CUsbWatcherTest::GetPersonalityIdFromString( TPtrC& aUsbPersonalityName, TInt& aUsbPersonalityId )
    {
    if ( !aUsbPersonalityName.Compare( _L( "PCSUITE" ) ) )
        {
        TRACE( "Test personality ID setting: PCSUITE" );
        aUsbPersonalityId = KUsbPersonalityIdPCSuiteMTP;
        }
    else if ( !aUsbPersonalityName.Compare( _L( "MS" ) ) )
        {
        TRACE( "Test personality ID setting: MS" );
        aUsbPersonalityId = KUsbPersonalityIdMS;
        }
    else if ( !aUsbPersonalityName.Compare( _L( "PTP" ) ) )
        {
        TRACE( "Test personality ID setting: PTP" );
        aUsbPersonalityId = KUsbPersonalityIdPTP;
        }
    else if ( !aUsbPersonalityName.Compare( _L( "MTP" ) ) )
        {
        TRACE( "Test personality ID setting: MTP" );
        aUsbPersonalityId = KUsbPersonalityIdMTP;
        }
    else if ( !aUsbPersonalityName.Compare( _L( "Modem" ) ) )
        {
        TRACE( "Test personality ID setting: Modem" );
        aUsbPersonalityId = KUsbPersonalityIdModemInst;
        }
    else
        {
        TRACE( "Test personality ID setting: not supported" );
        return KErrNotFound;
        }
 
    return KErrNone;   
    }

// -----------------------------------------------------------------------------
// CUSBWatcherTest::PersonalitySwitchTest
// -----------------------------------------------------------------------------

void CUsbWatcherTest::PersonalitySwitchTestL( TUsbWatcherTestBlockParams& aParams, 
                                                TUsbWatcherTestResult& aTestResult )
    {
    TInt res;
    TRequestStatus stat;
    TPtrC testOptionName( aParams.iTestOption1 );
    TInt currentPersonalityId;
    
    res = iUsbMan -> GetCurrentPersonalityId( currentPersonalityId );
    if ( res != KErrNone )
        {
        TRACE_INFO( "GetCurrentPersonalityId failed with value: %d" , res );
        User::Leave( res );
        }
    
    TRACE( "  >Connect" );
    res = iWatcher -> Connect();
    TRACE( "  <Connect" );
    TRACE_INFO( "USB watcher connection value: %d, expected: %d" , res, KErrNone );
    User::LeaveIfError( res );
    
    if ( currentPersonalityId != KUsbPersonalityIdPCSuiteMTP )
        {
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) ); 
        }   
    
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( stat, KUsbPersonalityIdPCSuiteMTP, EFalse, EFalse );
    User::WaitForRequest( stat );
    TRACE( "  <SetPersonality" );
    
    TRACE_INFO( "Set initial test personality value: %d" , stat.Int() );
    User::LeaveIfError( stat.Int() ); 
        
    if ( !testOptionName.Compare( _L( "FAST_SWITCH" ) ) )
        {
        TRACE( "Test option: FAST_SWITCH" );
        aTestResult = FastPersonalitySwitchTest();
        }
    else if ( !testOptionName.Compare( _L( "DELAYED_SWITCH" ) ) )
        {
        TRACE( "Test option: DELAYED_SWITCH" );
        aTestResult = DelayedPersonalitySwitchTest();
        }
    else
        {
        TRACE( "Test option: not supported" );
        User::Leave(KErrNotFound);
        }
    
    }

TUsbWatcherTestResult CUsbWatcherTest::FastPersonalitySwitchTest()
    {
    TInt res;
    TInt testLoopCounter = 3;
    TInt numberOfPersonalities = 4;
    TInt currentPersonalityId;
    TInt setPersonalityId;
    TRequestStatus stat;
    
    TRACE( "Checking test results:" );
    
    for ( int i = 0; i< testLoopCounter; i++ )
        {
        currentPersonalityId = KUsbPersonalityIdMS;
        for ( int j = 0; j< numberOfPersonalities; j++ )
            {
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
            iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) ); 
            
            TRACE( "  >SetPersonality" );
            iWatcher -> SetPersonality( stat, currentPersonalityId, EFalse, EFalse );
            User::WaitForRequest( stat );
            TRACE( "  <SetPersonality" );
            
            res = iCenRep -> Get( KUsbWatcherPersonality, setPersonalityId );
            TRACE_INFO("Set personality request status: %d, expected %d", stat.Int(), KErrNone );
            TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
            TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, setPersonalityId );
            if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != setPersonalityId )
                return ETestCaseFailed;
            currentPersonalityId++;
            }
        }
    return ETestCasePassed;
    }

TUsbWatcherTestResult CUsbWatcherTest::DelayedPersonalitySwitchTest()
    {
    TInt res;
    TInt numberOfPersonalities = 4;
    TInt currentPersonalityId;
    TInt setPersonalityId;
    TRequestStatus stat;
    
    TRACE( "Checking test results:" );
    currentPersonalityId = KUsbPersonalityIdMS;
    
    for ( int j = 0; j< numberOfPersonalities; j++ )
        {
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) ); 
        
        TRACE( "  >SetPersonality" );
        iWatcher -> SetPersonality( stat, currentPersonalityId, EFalse, EFalse );
        User::WaitForRequest( stat );
        TRACE( "  <SetPersonality" );
        
        res = iCenRep -> Get( KUsbWatcherPersonality, setPersonalityId );
        TRACE_INFO("Set personality request status: %d, expected %d", stat.Int(), KErrNone );
        TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
        TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, setPersonalityId );
        if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != setPersonalityId )
            return ETestCaseFailed;
        currentPersonalityId++;
        User::After( 7000*1000 );
        }
    return ETestCasePassed;
    }

// -----------------------------------------------------------------------------
// CUSBWatcherTest::BranchTest
// -----------------------------------------------------------------------------

void CUsbWatcherTest::BranchTestL( TUsbWatcherTestBlockParams& aParams, 
                                    TUsbWatcherTestResult& aTestResult )
    {
    TInt res;
    TRequestStatus stat;
    TPtrC testOptionName( aParams.iTestOption1 );
    TInt currentPersonalityId;
    
    res = iUsbMan -> GetCurrentPersonalityId( currentPersonalityId );
    if ( res != KErrNone )
        {
        TRACE_INFO( "GetCurrentPersonalityId failed with value: %d", res );
        User::Leave( res );
        }
        
    TRACE( "  >Connect" );
    res = iWatcher -> Connect();
    TRACE( "  <Connect" );
    TRACE_INFO( "USB watcher connection value: %d, expected: %d", res, KErrNone );
    User::LeaveIfError( res );
    
    if ( currentPersonalityId != KUsbPersonalityIdPCSuiteMTP )
        {
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) ); 
        }   
    
    TRACE( "Initial SetPersonality" );
    
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( stat, KUsbPersonalityIdPCSuiteMTP, EFalse, EFalse );
    User::WaitForRequest( stat );
    TRACE( "  <SetPersonality" );
    
    TRACE_INFO( "Set initial test personality value: %d", stat.Int() );
    User::LeaveIfError( stat.Int() ); 
        
    if ( !testOptionName.Compare( _L( "SAME_PERSONALITY_SET" ) ) )
        {
        TRACE( "Test option: SAME_PERSONALITY_SET" );
        aTestResult = SetSamePersonalityTest();
        }
    else if ( !testOptionName.Compare( _L( "NON_BLOCKING" ) ) )
        {
        TRACE( "Test option: NON_BLOCKING" );
        aTestResult = SetPersonalityBlockingOn();
        }
    else if ( !testOptionName.Compare( _L( "FORCE_SWITCH" ) ) )
        {
        TRACE( "Test option: FORCE_SWITCH" );
        aTestResult = SetPersonalityAskOnConnectionOff();
        }
    else
        {
        TRACE( "Test option: not supported" );
        User::Leave( KErrNotFound );
        }
    }

TUsbWatcherTestResult CUsbWatcherTest::SetSamePersonalityTest()
    {
    TRequestStatus stat;
    TInt currentPersonalityId;
    TInt res;
    
    TRACE( "Checking test results:" );
    
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( stat, KUsbPersonalityIdPCSuiteMTP, EFalse, EFalse );   
    User::WaitForRequest( stat );
    TRACE( "  <SetPersonality" );
    TRACE_INFO("Set personality request status: %d, expected %d", stat.Int(), KErrNone );
    
    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, KUsbPersonalityIdPCSuiteMTP );
    
    if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != KUsbPersonalityIdPCSuiteMTP )
        return ETestCaseFailed;
    
    return ETestCasePassed;  
    }

TUsbWatcherTestResult CUsbWatcherTest::SetPersonalityBlockingOn()
    {
    TRequestStatus stat;
    TInt currentPersonalityId;
    TInt res;
    
    TRACE( "Checking test results:" );
    
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) ); 
    
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( stat, KUsbPersonalityIdMS, EFalse, ETrue );   
    User::WaitForRequest( stat );
    TRACE( "  <SetPersonality" );
    TRACE_INFO("Set personality request status: %d, expected %d", stat.Int(), KErrNone );
    
    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, KUsbPersonalityIdMS );
    
    if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != KUsbPersonalityIdMS )
        return ETestCaseFailed;
    
    return ETestCasePassed; 
    }

TUsbWatcherTestResult CUsbWatcherTest::SetPersonalityAskOnConnectionOff()
    {
    TRequestStatus stat;
    TInt currentPersonalityId;
    TInt res;
    
    TRACE( "Checking test results:" );
    
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
    iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) ); 
    
    TRACE( "  >SetPersonality" );
    iWatcher -> SetPersonality( stat, KUsbPersonalityIdMS, ETrue, EFalse );   
    User::WaitForRequest( stat );
    TRACE( "  <SetPersonality" );
    TRACE_INFO("Set personality request status: %d, expected %d", stat.Int(), KErrNone );
    
    res = iCenRep -> Get( KUsbWatcherPersonality, currentPersonalityId );
    TRACE_INFO("GetCurrentPersonalityId value: %d, expected %d", res, KErrNone );
    TRACE_INFO("Current personality ID: %d, expected: %d", currentPersonalityId, KUsbPersonalityIdMS );
    
    if ( stat.Int() != KErrNone || res != KErrNone || currentPersonalityId != KUsbPersonalityIdMS )
        return ETestCaseFailed;
    
    return ETestCasePassed; 
    }

// -----------------------------------------------------------------------------
// CUSBWatcherTest:: Other functions
// -----------------------------------------------------------------------------

TInt CUsbWatcherTest::BackupPersonalitySetting()
    {
    TRACE( "><Backup personality setting" )
    return iUsbMan -> GetCurrentPersonalityId( iPersonalityIdBackup );
    }

TInt CUsbWatcherTest::RestorePersonalitySetting()
    {
    TRequestStatus stat;
    TInt currentPersonalityId;
    
    TRACE( ">Restore personality setting" )
    TRACE( "  >Close" );
    iWatcher -> Close();
    TRACE( "  <Close" );
    
    iUsbMan -> GetCurrentPersonalityId( currentPersonalityId );
    if ( currentPersonalityId != iPersonalityIdBackup )
        {   
        TRACE( "  >Connect" );
        iWatcher -> Connect();
        TRACE( "  <Connect" );
        
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, EGetDeviceState, 0, 0, _L8( "EUsbDeviceStateConfigured" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStopAsync, 0, 0, _L8( "stub" ) ) );
        iStubber -> EnqueBehavior( TApiBehavior( KUsbManStubAgentDll, ETryStartAsync, 0, 0, _L8( "stub" ) ) );
        
        TRACE( "  >SetPersonality" );
        iWatcher -> SetPersonality( stat, iPersonalityIdBackup, EFalse, EFalse );
        User::WaitForRequest( stat );
        TRACE( "  <SetPersonality" );
        
        TRACE( "  >Close" );
        iWatcher -> Close();
        TRACE( "  <Close" );
        }
    
    TRACE( "<Restore personality setting" )
    return stat.Int();
    }




// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
