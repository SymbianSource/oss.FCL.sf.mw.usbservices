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
#include "UsbBasicPersonalityTest.h"
#include <usbpersonalityids.h>

#include <ecom/ecom.h>


// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CUsbBasicPersonalityTest::Delete() 
    {
    iUsb.Close();	
    
    delete iScheduler;
	iScheduler = NULL; 
	
	delete iPlugin;
	iPlugin = NULL;
	REComSession::FinalClose();	
	
	delete iNotifier;
	iNotifier = NULL;	
	
	delete iParams;
	iParams = NULL;	
    }

// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove
		ENTRY( "ExecuteApiTest", CUsbBasicPersonalityTest::ExecuteApiTest ),
        ENTRY( "ExecuteModuleTest", CUsbBasicPersonalityTest::ExecuteModuleTest ),
        ENTRY( "ExecuteBranchTest", CUsbBasicPersonalityTest::ExecuteBranchTest ),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::ExecuteApiTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::ExecuteApiTest( CStifItemParser& aItem )
    {
	STIF_LOG( "[STIF_LOG] >>>ExecuteApiTest" );
	
	TInt res;
    TUsbBasicPersonalityTestResult testResult;
    TPtrC apiTestName( KNullDesC );
	
	res = aItem.GetString( _L( "ExecuteApiTest" ), apiTestName );   
    if ( res != KErrNone )
        {
        iLog -> Log( _L( "GetString failed with value: %d" ), res );
        return res;
        }
		
	TRAP( res, DoExecuteApiTestL( apiTestName, testResult ) );
    if ( res != KErrNone )
        {
        iLog -> Log( _L( "DoExecuteApiTestL error: %d"), res );
        return res;
        }
    
    STIF_ASSERT_EQUALS( ETestCasePassed, testResult );
    STIF_LOG( "[STIF_LOG] Test case passed" );
	STIF_LOG( "[STIF_LOG] <<<ExecuteApiTest" );
    return KErrNone;
    }
	

// -----------------------------------------------------------------------------
// CusbBasicPersonalityApiTest::DoExecuteApiTestL
// -----------------------------------------------------------------------------
//
void CUsbBasicPersonalityTest::DoExecuteApiTestL( TPtrC aApiTestName, TUsbBasicPersonalityTestResult& aTestResult )
    {
	STIF_LOG( "[STIF_LOG] >>>DoExecuteApiTestL" );
    if ( !aApiTestName.Compare( _L( "ExampleApiTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: ExampleApiTest" );
        ExampleApiTestL( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "CreateAndDeleteTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: CreateAndDeleteTest" );
        CreateAndDeleteTest ( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "IsActiveTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: IsActiveTest" );
        IsActiveTest ( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "IsAddedTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: IsAddedTest" );
        IsAddedTest ( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "PriorityTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: PriorityTest" );
        PriorityTest ( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "PreparePersonalityStartTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: PreparePersonalityStartTest" );
        PreparePersonalityStartTest ( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "ConfirmPersonalityUnloadTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: ConfirmPersonalityUnloadTest" );
        ConfirmPersonalityUnloadTest ( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "StateChangeNotifyTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: StateChangeNotifyTest" );
        StateChangeNotifyTest ( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "FinishPersonalityStartTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: FinishPersonalityStartTest" );
        FinishPersonalityStartTest ( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "FinishPersonalityStopTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: FinishPersonalityStopTest" );
        FinishPersonalityStopTest ( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "PreparePersonalityStopTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: PreparePersonalityStopTest" );
        PreparePersonalityStopTest ( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "SetPriorityTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: SetPriorityTest" );
        SetPriorityTest ( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "ShowUsbConnectionNoteTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: ShowUsbConnectionNoteTest" );
        ShowUsbConnectionNoteTest ( aTestResult );
        }
    else
        {
        STIF_LOG( "[STIF_LOG] Api test type: not found" );
        User::Leave( KErrNotFound );
        }
	STIF_LOG( "[STIF_LOG] <<<DoExecuteApiTestL" );
    }


// -----------------------------------------------------------------------------
// CusbBasicPersonalityApiTest::ExampleApiTestL
// -----------------------------------------------------------------------------
//
void CUsbBasicPersonalityTest::ExampleApiTestL( TUsbBasicPersonalityTestResult& aTestResult )
    {
	STIF_LOG( "[STIF_LOG] >>>ExampleApiTestL" );
	
    aTestResult = ETestCasePassed;
	
	STIF_LOG( "[STIF_LOG] <<<ExampleApiTestL" );
    }
	
// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::ExecuteModuleTest
// -----------------------------------------------------------------------------	
//
TInt CUsbBasicPersonalityTest::ExecuteModuleTest( CStifItemParser& aItem )
    {
	STIF_LOG( "[STIF_LOG] >>>ExecuteModuleTest" );
	
    TInt res;
    TUsbBasicPersonalityTestResult testResult;
    TPtrC moduleTestName( KNullDesC );

    res = aItem.GetString( _L( "ExecuteModuleTest" ), moduleTestName );   
    if ( res != KErrNone )
        {
        iLog -> Log( _L( "GetString failed with value: %d" ), res );
        return res;
        }

    TRAP( res, DoExecuteModuleTestL( moduleTestName, testResult ) );
    if ( res != KErrNone )
        {
        iLog -> Log( _L( "DoExecuteModuleTestL error: %d"), res );
        return res;
        }
    
    STIF_ASSERT_EQUALS( ETestCasePassed, testResult );
    STIF_LOG( "[STIF_LOG] Test case passed" );
	STIF_LOG( "[STIF_LOG] <<<ExecuteModuleTest" );
    return KErrNone;
    }	
	

// -----------------------------------------------------------------------------
// CusbBasicPersonalityApiTest::DoExecuteModuleTestL
// -----------------------------------------------------------------------------
//
void CUsbBasicPersonalityTest::DoExecuteModuleTestL( TPtrC aModuleTestName, TUsbBasicPersonalityTestResult& aTestResult )
    {
	STIF_LOG( "[STIF_LOG] >>>DoExecuteModuleTestL" );
    if ( !aModuleTestName.Compare( _L( "ExampleModuleTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Module test type: ExampleModuleTest" );
        ExampleModuleTestL( aTestResult );
        }
    else
        {
        STIF_LOG( "[STIF_LOG] Module test type: not found" );
        User::Leave( KErrNotFound );
        }
	STIF_LOG( "[STIF_LOG] <<<DoExecuteModuleTestL" );
    }
	

// -----------------------------------------------------------------------------
// CusbBasicPersonalityApiTest::ExampleModuleTestL
// -----------------------------------------------------------------------------
//
void CUsbBasicPersonalityTest::ExampleModuleTestL( TUsbBasicPersonalityTestResult& aTestResult )
    {
	STIF_LOG( "[STIF_LOG] >>>ExampleModuleTestL" );
	
    aTestResult = ETestCasePassed;
	
	STIF_LOG( "[STIF_LOG] <<<ExampleModuleTestL" );
    }
	
// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::ExecuteBranchTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::ExecuteBranchTest( CStifItemParser& aItem )
    {
	STIF_LOG( "[STIF_LOG] >>>ExecuteBranchTest" );
	
    TInt res;
    TUsbBasicPersonalityTestResult testResult;
    TPtrC branchTestName( KNullDesC );

    res = aItem.GetString( _L( "ExecuteBranchTest" ), branchTestName );   
    if ( res != KErrNone )
        {
        iLog -> Log( _L( "GetString failed with value: %d" ), res );
        return res;
        }

    TRAP( res, DoExecuteBranchTestL( branchTestName, testResult ) );
    if ( res != KErrNone )
        {
        iLog -> Log( _L( "DoExecuteBranchTestL error: %d"), res );
        return res;
        }
    
    STIF_ASSERT_EQUALS( ETestCasePassed, testResult );
    STIF_LOG( "[STIF_LOG] Test case passed" );
	STIF_LOG( "[STIF_LOG] <<<ExecuteBranchTest" );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CusbBasicPersonalityApiTest::DoExecuteBranchTestL
// -----------------------------------------------------------------------------
//
void CUsbBasicPersonalityTest::DoExecuteBranchTestL( TPtrC aBranchTestName, TUsbBasicPersonalityTestResult& aTestResult )
    {
	STIF_LOG( "[STIF_LOG] >>>DoExecuteBranchTestL" );
    if ( !aBranchTestName.Compare( _L( "ExampleBranchTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Branch test type: ExampleBranchTest" );
        ExampleBranchTestL( aTestResult );
        }
    else
        {
        STIF_LOG( "[STIF_LOG] Branch test type: not found" );
        User::Leave( KErrNotFound );
        }
	STIF_LOG( "[STIF_LOG] <<<DoExecuteBranchTestL" );
    }
	

// -----------------------------------------------------------------------------
// CusbBasicPersonalityApiTest::ExampleBranchTestL
// -----------------------------------------------------------------------------
//
void CUsbBasicPersonalityTest::ExampleBranchTestL( TUsbBasicPersonalityTestResult& aTestResult )
    {
	STIF_LOG( "[STIF_LOG] >>>ExampleBranchTestL" );
	
    aTestResult = ETestCasePassed;
    
	STIF_LOG( "[STIF_LOG] <<<ExampleBranchTestL" );
	}

// -----------------------------------------------------------------------------
// CusbBasicPersonalityApiTest::CreateAndDeleteTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::CreateAndDeleteTest( TUsbBasicPersonalityTestResult& aTestResult )
	{
	STIF_LOG("CreateAndDeleteTest: Started");
	
	//PC Suite
	CUsbPersonalityPlugin* plugin1 = CUsbPersonalityPlugin::NewL( *iParams, TUid::Uid(0x102068DF) );
	CleanupStack::PushL( plugin1 );
	
	//Media Transfer
	CUsbPersonalityPlugin* plugin3 = CUsbPersonalityPlugin::NewL( *iParams, TUid::Uid(0x10282C70) );
	CleanupStack::PushL( plugin3 );
	
	//PC Suite
	CUsbPersonalityPlugin* plugin4 = CUsbPersonalityPlugin::NewL( *iParams, TUid::Uid(0x2000B5D2) );
	CleanupStack::PushL( plugin4 );
	
	CleanupStack::PopAndDestroy( 3 );

    aTestResult = ETestCasePassed;
    
    STIF_LOG("CreateAndDeleteTest: End ");
    
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::ShowUsbConnectionNoteTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::ShowUsbConnectionNoteTest( TUsbBasicPersonalityTestResult& aTestResult )
	{
		STIF_LOG("ShowUsbConnectionNoteTest: Started");
		
		iPlugin->ShowUsbConnectionNote();
		
	    aTestResult = ETestCasePassed;

	    STIF_LOG("ShowUsbConnectionNoteTest: End ");
	    
	    return KErrNone;
	}

// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::SetPriorityTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::SetPriorityTest( TUsbBasicPersonalityTestResult& aTestResult )
	{
	STIF_LOG("SetPriorityTest:> Started");
	
	TRequestStatus status;	

	iPlugin->SetPriority( CActive::EPriorityStandard );
	
    aTestResult = ETestCasePassed;

    STIF_LOG("SetPriorityTest:> End ");
    
    return KErrNone;
	}


// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::PreparePersonalityStopTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::PreparePersonalityStopTest( TUsbBasicPersonalityTestResult& aTestResult )
	{
	STIF_LOG("PreparePersonalityStopTest:> Started");
	
	TRequestStatus status;	

	iPlugin->PreparePersonalityStop( status );
	User::WaitForRequest( iStatus );
	
	REComSession::FinalClose();
	
    aTestResult = ETestCasePassed;

    STIF_LOG("PreparePersonalityStopTest:> End ");
    
    return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::FinishPersonalityStopTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::FinishPersonalityStopTest( TUsbBasicPersonalityTestResult& aTestResult )
	{
	STIF_LOG("FinishPersonalityStopTest:> Started");
	
	TRequestStatus status;	

	iPlugin->FinishPersonalityStop( status );
	User::WaitForRequest( iStatus );
	
	REComSession::FinalClose();
	
    aTestResult = ETestCasePassed;

    STIF_LOG("FinishPersonalityStopTets:> End ");
    
    return KErrNone;	
	}

// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::FinishPersonalityStartTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::FinishPersonalityStartTest( TUsbBasicPersonalityTestResult& aTestResult )
	{
	STIF_LOG("FinishPersonalityStartTest:> Started");
	
	TRequestStatus status;	

	iPlugin->FinishPersonalityStart( status );
	User::WaitForRequest( iStatus );
	
	REComSession::FinalClose();
	
    aTestResult = ETestCasePassed;

    STIF_LOG("FinishPersonalityStartTest:> End ");
    
    return KErrNone;
	
	}

// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::ConfirmPersonalityUnloadTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::ConfirmPersonalityUnloadTest( TUsbBasicPersonalityTestResult& aTestResult )
	{
		STIF_LOG("ConfirmPersonalityUnloadTest: Started");
		
		iPlugin->ConfirmPersonalityUnload( iStatus );
		User::WaitForRequest( iStatus );
	
		REComSession::FinalClose();
		
	    aTestResult = ETestCasePassed;

	    STIF_LOG("ConfirmPersonalityUnloadTest: End ");
	    return KErrNone;
	}

// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::PreparePersonalityStartTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::PreparePersonalityStartTest( TUsbBasicPersonalityTestResult& aTestResult )
	{
		STIF_LOG("PreparePersonalityStartTest: Started");
		
		iPlugin->PreparePersonalityStart( iStatus );
		User::WaitForRequest( iStatus );
		
		REComSession::FinalClose();
		
	    aTestResult = ETestCasePassed;

	    STIF_LOG("PreparePersonalityStartTest: End ");
	    
	    return KErrNone;
	}


// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::StateChangeNotifyTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::StateChangeNotifyTest( TUsbBasicPersonalityTestResult& aTestResult )
	{
	STIF_LOG("StateChangeNotifyTest:> Started");
	
	TUsbDeviceState state = EUsbDeviceStateDefault;
	iPlugin->StateChangeNotify( state );
	
    aTestResult = ETestCasePassed;

    STIF_LOG("StateChangeNotifyTest:> End ");
    return KErrNone;	
	
	}


// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::IsActiveTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::IsActiveTest( TUsbBasicPersonalityTestResult& aTestResult )
	{
	STIF_LOG("IsActiveTest:> Started");
	
	_LIT(KIsActive,"IsActiveTest:> Is Active? %d");
	iLog->Log(KIsActive, iPlugin->IsActive() );
		
    aTestResult = ETestCasePassed;

    STIF_LOG("IsActiveTest:> End ");
    
    return KErrNone;	
	
	}

// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::IsAddedTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::IsAddedTest( TUsbBasicPersonalityTestResult& aTestResult )
	{
	STIF_LOG("IsAddedTest:> Started");

	_LIT(KIsActive,"IsAddedTest:> Is Added? %d");
	iLog->Log(KIsActive, iPlugin->IsAdded() );

	aTestResult = ETestCasePassed;
		
    STIF_LOG("IsAddedTest:> End ");
    
    return KErrNone;
	
	}

// -----------------------------------------------------------------------------
// CUsbBasicPersonalityTest::PriorityTest
// -----------------------------------------------------------------------------
//
TInt CUsbBasicPersonalityTest::PriorityTest( TUsbBasicPersonalityTestResult& aTestResult )
	{
	STIF_LOG("PriorityTest:> Started");

	iPlugin->SetPriority( CActive::EPriorityLow );
	TInt priority;
	priority = iPlugin->Priority();	
	
	priority == CActive::EPriorityLow ?
			aTestResult = ETestCasePassed :
			aTestResult = ETestCaseFailed;

    STIF_LOG("PriorityTest:> End ");
    
    return KErrNone;
	
	}
	
// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
