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
* Description:   ?Description
*
*/




// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "UsbObexClassControllerTest.h"

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;
const TUid KCSrcsInterfaceUid = {0x101F7C8C};
const TInt KObexNumInterfaces = 2;

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
// CUsbObexClassControllerTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CUsbObexClassControllerTest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove
		ENTRY( "ExecuteApiTest", CUsbObexClassControllerTest::ExecuteApiTest ),
        ENTRY( "ExecuteModuleTest", CUsbObexClassControllerTest::ExecuteModuleTest ),
        ENTRY( "ExecuteBranchTest", CUsbObexClassControllerTest::ExecuteBranchTest ),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::ExecuteApiTest
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::ExecuteApiTest( CStifItemParser& aItem )
    {
	STIF_LOG( "[STIF_LOG] >>>ExecuteApiTest" );
	
	TInt res;
    TUsbObexClassControllerTestResult testResult = ETestCaseFailed;
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
// CUsbObexClassControllerTest::DoExecuteApiTestL
// -----------------------------------------------------------------------------
//
void CUsbObexClassControllerTest::DoExecuteApiTestL( TPtrC aApiTestName, TUsbObexClassControllerTestResult& aTestResult )
    {
	STIF_LOG( "[STIF_LOG] >>>DoExecuteApiTestL" );
    if ( !aApiTestName.Compare( _L( "ExampleApiTest" ) ) )
        {
        STIF_LOG( "[STIF_LOG] Api test type: ExampleApiTest" );
        ExampleApiTestL( aTestResult );
        }
    else if ( !aApiTestName.Compare( _L( "InitializationTest" ) ) )
    	{
        STIF_LOG( "[STIF_LOG] Api test type: ExampleApiTest" );
        InitializationTestL( aTestResult );    	
    	}
    else if ( !aApiTestName.Compare( _L( "StartTest" ) ) )
    	{
        STIF_LOG( "[STIF_LOG] Api test type: ExampleApiTest" );
        StartTestL( aTestResult );    	
    	}
    else if ( !aApiTestName.Compare( _L( "StopTest" ) ) )
    	{
        STIF_LOG( "[STIF_LOG] Api test type: ExampleApiTest" );
        StopTestL( aTestResult );    	
    	}
    else if ( !aApiTestName.Compare( _L( "InterfaceInfoTest" ) ) )
    	{
        STIF_LOG( "[STIF_LOG] Api test type: ExampleApiTest" );
        InterfaceInfoTestL( aTestResult );    	
    	}
    else if ( !aApiTestName.Compare( _L( "InterfaceLengthInfoTest" ) ) )
    	{
        STIF_LOG( "[STIF_LOG] Api test type: ExampleApiTest" );
        InterfaceLengthInfoTestL( aTestResult );    	
    	}
    else if ( !aApiTestName.Compare( _L( "NumberOfInterfacesInfoTest" ) ) )
    	{
        STIF_LOG( "[STIF_LOG] Api test type: ExampleApiTest" );
        NumberOfInterfacesInfoTestL( aTestResult );    	
    	}
    else if ( !aApiTestName.Compare( _L( "StartupPriorityInfoTest" ) ) )
    	{
        STIF_LOG( "[STIF_LOG] Api test type: ExampleApiTest" );
        StartupPriorityInfoTestL( aTestResult );    	
    	}
    else if ( !aApiTestName.Compare( _L( "StateInfoTest" ) ) )
    	{
        STIF_LOG( "[STIF_LOG] Api test type: ExampleApiTest" );
        StateInfoTestL( aTestResult );    	
    	}
    else if ( !aApiTestName.Compare( _L( "OwnerTest" ) ) )
    	{
        STIF_LOG( "[STIF_LOG] Api test type: ExampleApiTest" );
        OwnerTestL( aTestResult );    	
    	}
    else
        {
        STIF_LOG( "[STIF_LOG] Api test type: not found" );
        User::Leave( KErrNotFound );
        }
	STIF_LOG( "[STIF_LOG] <<<DoExecuteApiTestL" );
    }


// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::ExecuteApiTest
// -----------------------------------------------------------------------------
//
void CUsbObexClassControllerTest::ExampleApiTestL( TUsbObexClassControllerTestResult& aTestResult )
    {
	STIF_LOG( "[STIF_LOG] >>>ExampleApiTestL" );
	
    aTestResult = ETestCasePassed;
	
	STIF_LOG( "[STIF_LOG] <<<ExampleApiTestL" );
    }
	
// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::ExecuteModuleTest
// -----------------------------------------------------------------------------	
//
TInt CUsbObexClassControllerTest::ExecuteModuleTest( CStifItemParser& aItem )
    {
	STIF_LOG( "[STIF_LOG] >>>ExecuteModuleTest" );
	
    TInt res;
    TUsbObexClassControllerTestResult testResult;
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
// CUsbObexClassControllerTest::DoExecuteModuleTestL
// -----------------------------------------------------------------------------
//
void CUsbObexClassControllerTest::DoExecuteModuleTestL( TPtrC aModuleTestName, TUsbObexClassControllerTestResult& aTestResult )
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
// CUsbObexClassControllerTest::ExampleModuleTestL
// -----------------------------------------------------------------------------
//
void CUsbObexClassControllerTest::ExampleModuleTestL( TUsbObexClassControllerTestResult& aTestResult )
    {
	STIF_LOG( "[STIF_LOG] >>>ExampleModuleTestL" );
	
    aTestResult = ETestCasePassed;
	
	STIF_LOG( "[STIF_LOG] <<<ExampleModuleTestL" );
    }
	
// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::ExecuteBranchTest
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::ExecuteBranchTest( CStifItemParser& aItem )
    {
	STIF_LOG( "[STIF_LOG] >>>ExecuteBranchTest" );
	
    TInt res;
    TUsbObexClassControllerTestResult testResult;
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
// CUsbObexClassControllerTest::DoExecuteBranchTestL
// -----------------------------------------------------------------------------
//
void CUsbObexClassControllerTest::DoExecuteBranchTestL( TPtrC aBranchTestName, TUsbObexClassControllerTestResult& aTestResult )
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
// CUsbObexClassControllerTest::ExampleBranchTestL
// -----------------------------------------------------------------------------
//
void CUsbObexClassControllerTest::ExampleBranchTestL( TUsbObexClassControllerTestResult& aTestResult )
    {
	STIF_LOG( "[STIF_LOG] >>>ExampleBranchTestL" );
	
    aTestResult = ETestCasePassed;
    
	STIF_LOG( "[STIF_LOG] <<<ExampleBranchTestL" );
	}
	

// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::InitializationTestL
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::InitializationTestL( TUsbObexClassControllerTestResult& aTestResult )
    {    
    InitializeTestObjectL();
    STIF_ASSERT_NOT_NULL( iUsbClassControllerPlugIn );
    DeleteTestObject();    
    
    // Case was executed
    iLog->Log( _L("Test Case Passed"));
    aTestResult = ETestCasePassed;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::StartTestL
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::StartTestL( TUsbObexClassControllerTestResult& aTestResult )
    {
    iUsbServiceState = EUsbServiceStarted;
    InitializeTestObjectL();
    CUSBObexApiTestActiveObject* activeTestObject = 
            CUSBObexApiTestActiveObject::NewL( iLog, iUsbClassControllerPlugIn );
    activeTestObject -> Start();
    CActiveScheduler::Start();
    iLog->Log( _L("UsbServiceState = %d, expected %d"), 
                        iUsbServiceState, iUsbClassControllerPlugIn -> State());
    STIF_ASSERT_EQUALS( iUsbServiceState, iUsbClassControllerPlugIn -> State() );    
    DeleteTestObject();
    activeTestObject -> Cancel();
    
    // Case was executed
    iLog->Log( _L("Test Case Passed"));
    aTestResult = ETestCasePassed;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::StopTestL
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::StopTestL( TUsbObexClassControllerTestResult& aTestResult )
    {
    iUsbServiceState = EUsbServiceIdle;
    InitializeTestObjectL();
    CUSBObexApiTestActiveObject* activeTestObject = 
            CUSBObexApiTestActiveObject::NewL( iLog, iUsbClassControllerPlugIn );
    activeTestObject -> Start();
    CActiveScheduler::Start();
    activeTestObject -> Stop();
    CActiveScheduler::Start();
    iLog->Log( _L("UsbServiceState = %d, expected %d"), 
                           iUsbServiceState, iUsbClassControllerPlugIn -> State());
    STIF_ASSERT_EQUALS( iUsbServiceState, iUsbClassControllerPlugIn -> State() );    
    DeleteTestObject();
    activeTestObject -> Cancel();
    
    // Case was executed
    iLog->Log( _L("Test Case Passed"));
    aTestResult = ETestCasePassed;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::InterfaceInfoTestL
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::InterfaceInfoTestL( TUsbObexClassControllerTestResult& aTestResult )
    {
    TUsbDescriptor aDescriptorInfo;
    RImplInfoPtrArray implInfoArray;
    CleanupClosePushL(implInfoArray);
    TEComResolverParams resolverParams;
    TInt numberOfInterfaces;
    
    InitializeTestObjectL();
    CUSBObexApiTestActiveObject* activeTestObject = 
            CUSBObexApiTestActiveObject::NewL( iLog, iUsbClassControllerPlugIn );
    activeTestObject -> Start();
    CActiveScheduler::Start();
    iUsbClassControllerPlugIn -> GetDescriptorInfo( aDescriptorInfo ); 
    resolverParams.SetDataType( KSrcsTransportUSB );
    resolverParams.SetWildcardMatch( EFalse );
    REComSession::ListImplementationsL( KCSrcsInterfaceUid, resolverParams, implInfoArray );
    numberOfInterfaces = implInfoArray.Count() * KObexNumInterfaces;
    
    iLog->Log( _L("Number of interfaces = %d, expected %d"), 
                        numberOfInterfaces, aDescriptorInfo.iNumInterfaces);
    STIF_ASSERT_EQUALS( numberOfInterfaces, aDescriptorInfo.iNumInterfaces );
    
    DeleteTestObject();
    activeTestObject -> Cancel();
    CleanupStack::PopAndDestroy(&implInfoArray);    
    
    // Case was executed
    iLog->Log( _L("Test Case Passed"));
    aTestResult = ETestCasePassed;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::InterfaceLengthInfoTestL
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::InterfaceLengthInfoTestL( TUsbObexClassControllerTestResult& aTestResult )
    {
    TUsbDescriptor aDescriptorInfo;
    RImplInfoPtrArray implInfoArray;
    CleanupClosePushL(implInfoArray);
    TEComResolverParams resolverParams;
    TInt interfaceLength = 0;
    
    InitializeTestObjectL();
    CUSBObexApiTestActiveObject* activeTestObject = 
            CUSBObexApiTestActiveObject::NewL( iLog, iUsbClassControllerPlugIn );
    activeTestObject -> Start();
    CActiveScheduler::Start();
    iUsbClassControllerPlugIn -> GetDescriptorInfo( aDescriptorInfo );
    resolverParams.SetDataType( KSrcsTransportUSB );
    resolverParams.SetWildcardMatch( EFalse );
    REComSession::ListImplementationsL( KCSrcsInterfaceUid, resolverParams, implInfoArray );
    
    iLog->Log( _L("Interface Length = %d, expected %d"), 
                                    interfaceLength, aDescriptorInfo.iLength);
    STIF_ASSERT_EQUALS( interfaceLength, aDescriptorInfo.iLength );
    
    DeleteTestObject();
    activeTestObject -> Cancel();
    CleanupStack::PopAndDestroy(&implInfoArray);    
    
    // Case was executed
    iLog->Log( _L("Test Case Passed"));
    aTestResult = ETestCasePassed;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::NumberOfInterfacesInfoTestL
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::NumberOfInterfacesInfoTestL( TUsbObexClassControllerTestResult& aTestResult )
    {
    TUsbDescriptor aDescriptorInfo;
    RImplInfoPtrArray implInfoArray;
    CleanupClosePushL(implInfoArray);
    TEComResolverParams resolverParams;
    TInt interfaceLength = 0;
    
    InitializeTestObjectL();
    CUSBObexApiTestActiveObject* activeTestObject =
									CUSBObexApiTestActiveObject::NewL( iLog, iUsbClassControllerPlugIn );
    activeTestObject -> Start();
    CActiveScheduler::Start();
    iUsbClassControllerPlugIn -> GetDescriptorInfo( aDescriptorInfo );
    resolverParams.SetDataType( KSrcsTransportUSB );
    resolverParams.SetWildcardMatch( EFalse );
    REComSession::ListImplementationsL( KCSrcsInterfaceUid, resolverParams, implInfoArray );
    
    iLog->Log( _L("Number of Interfaces this class controller is responsible for  = %d, expected %d"), 
                                    interfaceLength, aDescriptorInfo.iNumInterfaces);
    STIF_ASSERT_EQUALS( interfaceLength, aDescriptorInfo.iLength );
    
    DeleteTestObject();
    activeTestObject -> Cancel();
    CleanupStack::PopAndDestroy(&implInfoArray);    
    
    // Case was executed
    iLog->Log( _L("Test Case Passed"));
    aTestResult = ETestCasePassed;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::StartupPriorityInfoTestL
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::StartupPriorityInfoTestL( TUsbObexClassControllerTestResult& aTestResult )
    {
    TInt priority = 0;
    
    InitializeTestObjectL();
    CUSBObexApiTestActiveObject* activeTestObject =
									CUSBObexApiTestActiveObject::NewL( iLog, iUsbClassControllerPlugIn );
    activeTestObject -> Start();
    CActiveScheduler::Start();
    priority = iUsbClassControllerPlugIn -> StartupPriority();

    iLog->Log( _L("StartupPriorityInfoTestL:> Priority = %d "),priority );                                
    
    DeleteTestObject();
    activeTestObject -> Cancel();    
    
    // Case was executed
    iLog->Log( _L("StartupPriorityInfoTestL:> Test Case Passed"));
    aTestResult = ETestCasePassed;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::StateInfoTestL
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::StateInfoTestL( TUsbObexClassControllerTestResult& aTestResult )
    {
    TUsbServiceState state;
    
    InitializeTestObjectL();
    CUSBObexApiTestActiveObject* activeTestObject =
									CUSBObexApiTestActiveObject::NewL( iLog, iUsbClassControllerPlugIn );
    activeTestObject -> Start();
    CActiveScheduler::Start();
    state = iUsbClassControllerPlugIn -> State();

    iLog->Log( _L("StateTestL:> State = 0x%x "), state );
    
    DeleteTestObject();
    activeTestObject -> Cancel();    
    
    // Case was executed
    iLog->Log( _L("StateInfoTestL:> Test Case Passed"));
    aTestResult = ETestCasePassed;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::OwnerTestL
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::OwnerTestL( TUsbObexClassControllerTestResult& aTestResult )
    {   
    InitializeTestObjectL();
    CUSBObexApiTestActiveObject* activeTestObject =
									CUSBObexApiTestActiveObject::NewL( iLog, iUsbClassControllerPlugIn );
    activeTestObject -> Start();
    CActiveScheduler::Start();
    (iUsbClassControllerPlugIn -> Owner()).UccnGetClassControllerIteratorL();

    (iUsbClassControllerPlugIn -> Owner()).UccnError( KErrNone );
    
    DeleteTestObject();
    activeTestObject -> Cancel();    
    
    // Case was executed
    iLog->Log( _L("OwnerTestL:> Test Case Passed"));
    aTestResult = ETestCasePassed;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::CompareTestL
// -----------------------------------------------------------------------------
//
TInt CUsbObexClassControllerTest::CompareTestL( TUsbObexClassControllerTestResult& aTestResult )
    {
    InitializeTestObjectL();   
    
    CUSBObexApiTestActiveObject* activeTestObject =
									CUSBObexApiTestActiveObject::NewL( iLog, iUsbClassControllerPlugIn );
    activeTestObject -> Start();
    CActiveScheduler::Start();
    
    //result = iUsbClassControllerPlugIn -> Compare( 	this, iUsbClassControllerPlugIn );
    //iLog->Log( _L("CompareTestL:> Comparition Result = %d "),result );
    
    DeleteTestObject();
    activeTestObject -> Cancel();    
    
    // Case was executed
    iLog->Log( _L("CompareTestL:> Test Case Passed"));
    aTestResult = ETestCasePassed;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::InitializeTestObjectL
// -----------------------------------------------------------------------------
//
void CUsbObexClassControllerTest::InitializeTestObjectL()
    {
    iUsbClassControllerPlugIn = CUsbClassControllerPlugIn::NewL( TUid::Uid( 0x10281F2A ), *this);
    }

// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::DeleteTestObject
// -----------------------------------------------------------------------------
//
void CUsbObexClassControllerTest::DeleteTestObject()
    {
    delete iUsbClassControllerPlugIn;
    iUsbClassControllerPlugIn = NULL;
    REComSession::FinalClose();
    }

// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::UccnGetClassControllerIteratorL
// -----------------------------------------------------------------------------
//
CUsbClassControllerIterator* CUsbObexClassControllerTest::UccnGetClassControllerIteratorL()
    {
    iLog->Log( _L("get class controller iterator"));
	return (CUsbClassControllerIterator *)NULL;
    }

// -----------------------------------------------------------------------------
// CUsbObexClassControllerTest::UccnError
// -----------------------------------------------------------------------------
//
void CUsbObexClassControllerTest::UccnError( TInt aError )
    {
    iLog->Log( _L("class controller iterator error: %d"), aError );
    }
// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
