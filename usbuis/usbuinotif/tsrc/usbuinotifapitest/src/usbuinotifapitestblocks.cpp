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
* Description:  
*
*/




// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <ecom/ecom.h>
#include <Stiftestinterface.h>
#include "UsbUiNotifApiTest.h"

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
// CUsbUiNotifApiTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CUsbUiNotifApiTest::Delete() 
	{
    iNotifier -> Close();
    delete iNotifier;
    iNotifier = NULL;
    iUsbWatcher -> Close();
    delete iUsbWatcher;
    iUsbWatcher = NULL;
    delete iRepository;
    iRepository = NULL;
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CUsbUiNotifApiTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove
		ENTRY( "ExecuteApiTestBlock", CUsbUiNotifApiTest::ExecuteApiTestBlock ),
        ENTRY( "ExecuteModuleTestBlock", CUsbUiNotifApiTest::ExecuteModuleTestBlock ),
        ENTRY( "ExecuteBranchTestBlock", CUsbUiNotifApiTest::ExecuteBranchTestBlock ),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::GetTestBlockParamsL
// -----------------------------------------------------------------------------

void CUsbUiNotifApiTest::GetTestBlockParamsL( CStifItemParser& aItem )
    {
    TRACE_INFO( _L(">>> GetTestBlockParamsL") );
    
    // Add new test block branches below, get all required test parameters    
    if ( !iTestBlockParams.iTestBlockName.Compare( _L( "ExampleTestL" ) ) )
        {              
        User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption1 ) );        
        User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption2 ) );
        User::LeaveIfError( aItem.GetNextInt( iTestBlockParams.iTestIntOption1 ) );        
        User::LeaveIfError( aItem.GetNextChar( iTestBlockParams.iTestCharOption1 ) );        
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "CableConnectedNotifierTest" ) ) )
		{                      
		User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption2 ) );       
		}
	else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UsbQueriesNotifierTest" ) ) )
		{                      
		User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption2 ) );       
		}
	else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "FinishQuery" ) ) )
		{                      
		User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption2 ) );       
		}
	else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UsbOTGErrorNotifierTests" ) ) )
		{                      
		User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption2 ) );       
		}
	else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UsbOTGWarningNotifierTests" ) ) )
		{                      
		User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption2 ) );       
		}
	else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "LoadNotifiers" ) ) )
		{                           
		}
	else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UnLoadNotifiers" ) ) )
		{                      
		}
	else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UsbMSMMNotifierTests" ) ) )
		{                      
		User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption2 ) );       
		}
    else
        {
        TRACE_INFO( _L("GetTestBlockParamsL() Test type: not found") );
        User::Leave( KErrNotFound );
        }
    TRACE_INFO( _L("<<< GetTestBlockParamsL") );
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::ExecuteApiTestBlock
// -----------------------------------------------------------------------------

TInt CUsbUiNotifApiTest::ExecuteApiTestBlock( CStifItemParser& aItem )
    {
    TRACE_INFO( _L(">>> ExecuteApiTestBlock") );
	
	TInt res;
    TUsbUiNotifApiTestResult testResult = ETestCaseFailed;
	
    TRAP( res, DoExecuteApiTestBlockL( aItem, testResult ) );
    if ( res != KErrNone )
        {
        TRACE_INFO( (_L("DoExecuteApiTestBlockL error: %d"), res) );
        return res;
        }
    
    STIF_ASSERT_EQUALS( ETestCasePassed, testResult );
    TRACE_INFO( _L("Test case passed") );
    
    TRACE_INFO( _L("<<< ExecuteApiTestBlock") );
	
    return KErrNone;
    }
	
	
void CUsbUiNotifApiTest::DoExecuteApiTestBlockL( CStifItemParser& aItem, TUsbUiNotifApiTestResult& aTestResult )
    {
    TRACE_INFO( _L(">>>DoExecuteApiTestBlockL") );

	User::LeaveIfError( aItem.GetString( _L( "ExecuteApiTestBlock" ), iTestBlockParams.iTestBlockName ) );
        TRACE_INFO( (_L("Api test type: %S"), &iTestBlockParams.iTestBlockName) );
	
	GetTestBlockParamsL( aItem );
	
	// Add new API test block branches with optional test parameters here	
    if ( !iTestBlockParams.iTestBlockName.Compare( _L( "ExampleTestL" ) ) )
        {      
        ExampleTestL( iTestBlockParams.iTestOption1, iTestBlockParams.iTestOption2, 
                iTestBlockParams.iTestIntOption1, iTestBlockParams.iTestCharOption1, aTestResult );
        }	
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "CableConnectedNotifierTest" ) ) )
        {      
        CableConnectedNotifierTest( iTestBlockParams.iTestOption2, aTestResult );
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UsbQueriesNotifierTest" ) ) )
        {      
        UsbQueriesNotifierTest( iTestBlockParams.iTestOption2, aTestResult );
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "FinishQuery" ) ) )
        {      
        FinishQuery( iTestBlockParams.iTestOption2, aTestResult );
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UsbQueriesNotifierTest" ) ) )
        {      
        UsbQueriesNotifierTest( iTestBlockParams.iTestOption2, aTestResult );
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UsbQueriesNotifierTest" ) ) )
        {      
        UsbQueriesNotifierTest( iTestBlockParams.iTestOption2, aTestResult );
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UsbQueriesNotifierTest" ) ) )
        {      
        UsbQueriesNotifierTest( iTestBlockParams.iTestOption2, aTestResult );
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UsbOTGErrorNotifierTests" ) ) )
        {      
        UsbOTGErrorNotifierTests( iTestBlockParams.iTestOption2, aTestResult );
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UsbOTGWarningNotifierTests" ) ) )
        {      
        UsbOTGWarningNotifierTests( iTestBlockParams.iTestOption2, aTestResult );
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "LoadNotifiers" ) ) )
        {      
        LoadNotifiersL( aTestResult );
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UnLoadNotifiers" ) ) )
        {      
        UnLoadNotifiers( aTestResult );
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "UsbMSMMNotifierTests" ) ) )
        {      
        UsbMSMMNotifierTests( iTestBlockParams.iTestOption2, aTestResult );
        }
    else
        {
    TRACE_INFO( _L("DoExecuteApiTestBlockL() Test type: not found") );
        User::Leave( KErrNotFound );
        }
	
    TRACE_INFO( _L("<<<DoExecuteApiTestBlockL") );
    }
	
// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::ExecuteModuleTestBlock
// -----------------------------------------------------------------------------	

TInt CUsbUiNotifApiTest::ExecuteModuleTestBlock( CStifItemParser& aItem )
    {
    TRACE_INFO( _L(">>>ExecuteModuleTestBlock") );
	
    TInt res;
    TUsbUiNotifApiTestResult testResult;
    
    TRAP( res, DoExecuteModuleTestBlockL( aItem, testResult ) );
    if ( res != KErrNone )
        {
        TRACE_INFO( (_L("DoExecuteModuleTestBlockL error: %d"), res) );
        return res;
        }
    
    STIF_ASSERT_EQUALS( ETestCasePassed, testResult );
    TRACE_INFO( _L("Test case passed") );
    TRACE_INFO( _L("<<<ExecuteModuleTestBlock") );
    return KErrNone;
    }	
	
	
void CUsbUiNotifApiTest::DoExecuteModuleTestBlockL( CStifItemParser& aItem, TUsbUiNotifApiTestResult& aTestResult )
    {
    TRACE_INFO( _L(">>>DoExecuteModuleTestBlockL") );
	
    User::LeaveIfError( aItem.GetString( _L( "ExecuteModuleTestBlock" ), iTestBlockParams.iTestBlockName ) );
    TRACE_INFO( (_L("Module test type: %S"), &iTestBlockParams.iTestBlockName) );
    
    GetTestBlockParamsL( aItem );
    
    // Add new module test block branches with optional test parameters here   
    if ( !iTestBlockParams.iTestBlockName.Compare( _L( "ExampleTestL" ) ) )
        {      
        ExampleTestL( iTestBlockParams.iTestOption1, iTestBlockParams.iTestOption2, 
                iTestBlockParams.iTestIntOption1, iTestBlockParams.iTestCharOption1, aTestResult );
        }
    else
        {
    TRACE_INFO( _L("DoExecuteModuleTestBlockL() Test type: not found") );
        User::Leave( KErrNotFound );
        }
    
    TRACE_INFO( _L("<<<DoExecuteModuleTestBlockL") );
    }
	
// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::ExecuteBranchTestBlock
// -----------------------------------------------------------------------------
	
TInt CUsbUiNotifApiTest::ExecuteBranchTestBlock( CStifItemParser& aItem )
    {
    TRACE_INFO( _L(">>>ExecuteBranchTestBlock") );
	
    TInt res;
    TUsbUiNotifApiTestResult testResult;
    
    TRAP( res, DoExecuteBranchTestBlockL( aItem, testResult ) );
    if ( res != KErrNone )
        {
        TRACE_INFO( (_L("DoExecuteBranchTestBlockL error: %d"), res) );
        return res;
        }   
    
    STIF_ASSERT_EQUALS( ETestCasePassed, testResult );
    TRACE_INFO( _L("Test case passed") );
    TRACE_INFO( _L("<<<ExecuteBranchTestBlock") );
    return KErrNone;
    }

	
void CUsbUiNotifApiTest::DoExecuteBranchTestBlockL( CStifItemParser& aItem, TUsbUiNotifApiTestResult& aTestResult )
    {
    TRACE_INFO( _L(">>>DoExecuteBranchTestBlockL") );
	
    User::LeaveIfError( aItem.GetString( _L( "ExecuteBranchTestBlock" ), iTestBlockParams.iTestBlockName ) );
    TRACE_INFO( (_L("Branch test type: %S"), &iTestBlockParams.iTestBlockName) );
    
    GetTestBlockParamsL( aItem );
    
    // Add new branch test block branches with optional test parameters here   
    if ( !iTestBlockParams.iTestBlockName.Compare( _L( "ExampleTestL" ) ) )
        {      
        ExampleTestL( iTestBlockParams.iTestOption1, iTestBlockParams.iTestOption2, 
                iTestBlockParams.iTestIntOption1, iTestBlockParams.iTestCharOption1, aTestResult );
        }
    else
        {
    TRACE_INFO( _L("DoExecuteBranchTestBlockL() Test type: not found") );
        User::Leave( KErrNotFound );
        }
    
    TRACE_INFO( _L("<<<DoExecuteBranchTestBlockL") );
    }

// Add test block methods implementation here
// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::ExampleTestL
// -----------------------------------------------------------------------------

void CUsbUiNotifApiTest::ExampleTestL( TPtrC aTestOption, TPtrC aTestSubOption, 
        TInt aTestIntOption, TInt aTestCharOption, TUsbUiNotifApiTestResult& aTestResult )
    {
    TRACE_INFO( _L(">>>ExampleTestL") );
    
    if ( !aTestOption.Compare( _L( "API" ) ) )
        {
    TRACE_INFO( (_L("Api test option: %S"), &aTestOption) );
    TRACE_INFO( (_L("Api test sub-option: %S"), &aTestSubOption) );
    TRACE_INFO( (_L("Api test int option: %d"), aTestIntOption) );
    TRACE_INFO( (_L("Api test char option: %c"), aTestCharOption) );
        }
    else if ( !aTestOption.Compare( _L( "MODULE" ) ) )
        {
    TRACE_INFO( (_L("Module test option: %S"), &aTestOption) );
    TRACE_INFO( (_L("Module test sub-option: %S"), &aTestSubOption) );
    TRACE_INFO( (_L("Module test int option: %d"), aTestIntOption) );
    TRACE_INFO( (_L("Module test char option: %c"), aTestCharOption) );
        }
    else if ( !aTestOption.Compare( _L( "BRANCH" ) ) )
        {
    TRACE_INFO( (_L("Branch test option: %S"), &aTestOption) );
    TRACE_INFO( (_L("Branch test sub-option: %S"), &aTestSubOption) );
    TRACE_INFO( (_L("Branch test int option: %d"), aTestIntOption) );
    TRACE_INFO( (_L("Branch test char option: %c"), aTestCharOption) );
        }
    else
        {
        TRACE_INFO( _L("Invalid test parameter") );
        User::Leave( KErrNotFound );
        }
    
    aTestResult = ETestCasePassed;
    
    TRACE_INFO( _L("<<<ExampleTestL") );
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::CableConnectedNotifierTest
// -----------------------------------------------------------------------------

void CUsbUiNotifApiTest::CableConnectedNotifierTest( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult )
    {
    TRACE_INFO(  _L(">>>CableConnectedNotifierTest") );
    
    TInt res;
    TUSBConnectionNotifierParamsPckg emptyNotifierInputPckg;
    TPtrC displayedUsbPersonalityName( KNullDesC );
    TInt displayedUsbPersonalityId;
    
    res = GetPersonalityIdFromString( aTestSubOption, displayedUsbPersonalityId );
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("GetPersonalityIdFromString failed with value: %d"), res) );
        return;
        }
    
    res = iRepository -> Get( KUsbWatcherPersonality, iPersonalityIdBackup );
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("Personality backup failed with value (cenrep): %d"), res) );
        return;
        }
    
    res = iUsbWatcher -> Connect();
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("Failed to connect UsbWatcher with value: %d"), res) );
        return;
        }   
    TRACE_INFO(  _L("UsbWatcher connected") );
        
    iUsbWatcher -> SetPersonality( iReqStatus, displayedUsbPersonalityId, ETrue, ETrue );
    User::WaitForRequest( iReqStatus );
    if ( iReqStatus.Int() != KErrNone )
        {
        TRACE_INFO(  (_L("Failed to set usb personality with value: %d"), iReqStatus.Int()) );
        iUsbWatcher -> Close();
        return;
        }   
    
    res = iNotifier -> Connect();
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("Failed to connect RNotifier with value: %d"), res) );
        iUsbWatcher -> Close();
        return;
        }   
    TRACE_INFO(  _L("RNotifier connected") );

    iNotifier -> StartNotifierAndGetResponse( iReqStatus, KCableConnectedNotifierUid, emptyNotifierInputPckg, iConnectionNotifierResponseParamsPckg );    
    User::WaitForRequest( iReqStatus );
    
    TRACE_INFO((_L("StartNotifierAndGetResponse Status: %d, expected: %d"), iReqStatus.Int(), KErrCancel));
    if(iReqStatus.Int() == KErrCancel)
    	aTestResult = ETestCasePassed;
    
    TRACE_INFO(  _L("<<<CableConnectedNotifierTest") );
    }


// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::UsbQueriesNotifierTest
// -----------------------------------------------------------------------------

void CUsbUiNotifApiTest::UsbQueriesNotifierTest( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult )
    {
    TRACE_INFO(  _L(">>>UsbQueriesNotifierTest") );
    TInt res;    
    TUSBUIQueries usbQueryType;
    
    
    res = GetQueryType( aTestSubOption, usbQueryType );
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("GetQueryType failed with value: %d"), res) );
        return;
        }
    
    TUSBQueriesNotiferParams notifierParams;
    notifierParams.iDrive = 'c';
    notifierParams.iQuery = usbQueryType;
    TUSBQueriesNotifierParamsPckg notifierParamsPckg( notifierParams );
    TPckgBuf<TBool> output;
    
    res = iNotifier -> Connect();
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("Failed to connect RNotifier with value: %d"), res) );
        return;
        }   
    TRACE_INFO(  _L("RNotifier connected") );
    
    switch( usbQueryType )
        {
        case EUSBNoMemoryCard:
            iCompleteQuery = ETrue;
            break;
        case EUSBStorageMediaFailure:
        case EUSBChangeFromMassStorage:
            iCompleteQuery = EFalse;
            break;
        default:
            break; 
        }
    
    iNotifier -> StartNotifierAndGetResponse( iReqStatus, KQueriesNotifier, notifierParamsPckg, output ); 

    aTestResult = ETestCasePassed;
    
    TRACE_INFO(  _L("<<<UsbQueriesNotifierTest") );
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::UsbOTGErrorNotifierTests
// -----------------------------------------------------------------------------

void CUsbUiNotifApiTest::UsbOTGErrorNotifierTests( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult )
    {
    TRACE_INFO(  _L(">>>UsbOTGErrorNotifierTests") );
    
    TInt res;    
    TUsbUiNotifOtgError usbOTGErrorType;
        
    res = GetOTGErrorType( aTestSubOption, usbOTGErrorType );
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("usbOTGErrorType failed with value: %d"), res) );
        return;
        }    
    
    res = iNotifier -> Connect();
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("Failed to connect RNotifier with value: %d"), res) );
        return;
        }   
    TRACE_INFO(  _L("RNotifier connected") );
    
    TPckgBuf<TInt> notifierParamsPckg;
    notifierParamsPckg() = usbOTGErrorType;
    iCompleteQuery = EFalse;

    iNotifier -> StartNotifierAndGetResponse( iReqStatus, KUsbUiNotifOtgError, notifierParamsPckg, iRes ); 

    aTestResult = ETestCasePassed;
    
    TRACE_INFO(  _L("<<<UsbOTGErrorNotifierTests") );
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::UsbOTGWarningNotifierTests
// -----------------------------------------------------------------------------

void CUsbUiNotifApiTest::UsbOTGWarningNotifierTests( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult )
    {        
    TRACE_INFO(  _L(">>>UsbOTGWarningNotifierTests") );
    
    TInt res;    
    TUsbUiNotifOtgWarning usbOTGWarningType;
    
    
    res = GetOTGWarningType( aTestSubOption, usbOTGWarningType );
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("usbOTGErrorType failed with value: %d"), res) );
        return;
        }    
    
    res = iNotifier -> Connect();
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("Failed to connect RNotifier with value: %d"), res) );
        return;
        }   
    TRACE_INFO(  _L("RNotifier connected") );
    
    TPckgBuf<TInt> notifierParamsPckg;
    notifierParamsPckg() = usbOTGWarningType;
    iCompleteQuery = EFalse;

    iNotifier -> StartNotifierAndGetResponse( iReqStatus, KUsbUiNotifOtgWarning, notifierParamsPckg, iRes ); 

    aTestResult = ETestCasePassed;
    
    TRACE_INFO(  _L("<<<UsbOTGWarningNotifierTests") ); 
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::UsbMSMMNotifierTests
// -----------------------------------------------------------------------------

void CUsbUiNotifApiTest::UsbMSMMNotifierTests( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult )
    {        
    TRACE_INFO(  _L(">>>UsbMSMMNotifierTests") );
    
    TInt res;    
    THostMsErrCode usbMSMMNErrorType;
    
    res = GetMSMMrrorType( aTestSubOption, usbMSMMNErrorType );
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("MSMMrrorType failed with value: %d"), res) );
        return;
        }    
    iErrData.iError=usbMSMMNErrorType;
    iErrPckg = iErrData;
    res = iNotifier -> Connect();
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("Failed to connect RNotifier with value: %d"), res) );
        return;
        }   
    TRACE_INFO(  _L("RNotifier connected") );
    
    TPckgBuf<TInt> notifierParamsPckg;
    notifierParamsPckg() = usbMSMMNErrorType;
    iCompleteQuery = EFalse;

    iNotifier -> StartNotifierAndGetResponse( iReqStatus, KUsbUiNotifMsmmError, iErrPckg, iRes ); 

    aTestResult = ETestCasePassed;
    
    TRACE_INFO(  _L("<<<UsbMSMMNotifierTests") ); 
    }


// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::FinishQuery
// -----------------------------------------------------------------------------

TInt CUsbUiNotifApiTest::FinishQuery( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult )
    {
    TRACE_INFO(  _L(">>>FinishQuery") ); 
    
    TInt res;
    TTestOption option;
    
    if ( iCompleteQuery )
        {
        TRequestStatus* statPtr = &iReqStatus;
        User::RequestComplete( statPtr, KErrNone );
        }    
    
    res = GetTestOption( aTestSubOption, option );
    if ( res != KErrNone )
        {
        TRACE_INFO(  (_L("GetTestOption failed with value: %d"), res) );
        iNotifier -> Close();
        return res;
        }
    
    User::WaitForRequest( iReqStatus );   
    iNotifier -> Close();
    
    switch( option )
        {
        case EQueryAccepted:
            TRACE_INFO(  (_L("Request status value: %d, expected: %d"), iReqStatus.Int(), KErrNone) );
            STIF_ASSERT_EQUALS( KErrNone, iReqStatus.Int() );
            break;
        case EQueryCanceled:
            TRACE_INFO(  (_L("Request status value: %d, expected: %d"), iReqStatus.Int(), KErrCancel) );
            STIF_ASSERT_EQUALS( KErrCancel, iReqStatus.Int() );
            break;
        default:
            return KErrNotFound;
        }
    
    aTestResult = ETestCasePassed;
    
    TRACE_INFO(  _L("<<<FinishQuery") ); 
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::LoadNotifiersL
// -----------------------------------------------------------------------------

void CUsbUiNotifApiTest::LoadNotifiersL( TUsbUiNotifApiTestResult& aTestResult )
    {
    TRACE_INFO(  _L(">>>LoadNotifiersL") );
    
    iNotifierArray = reinterpret_cast<CArrayPtr<MEikSrvNotifierBase2>*>(
												REComSession::CreateImplementationL(TUid::Uid(0x10281F23), iInstanceUid));

    aTestResult = ETestCasePassed;
    
    TRACE_INFO(  _L("<<<LoadNotifiersL") ); 	
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::UnLoadNotifiers
// -----------------------------------------------------------------------------

void CUsbUiNotifApiTest::UnLoadNotifiers ( TUsbUiNotifApiTestResult& aTestResult )
    {
    TRACE_INFO(  _L(">>>UnLoadNotifiers") );
    
    TInt count = iNotifierArray->Count();
    TInt i=0;
    for (i=0; i<count; i++)
        {
        iNotifierArray->At(i)->Release();        
        }
    iNotifierArray->Reset();    
    delete iNotifierArray;
    REComSession::DestroyedImplementation(iInstanceUid);    

    aTestResult = ETestCasePassed;
    
    TRACE_INFO(  _L("<<<UnLoadNotifiers") ); 
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::Update
// -----------------------------------------------------------------------------

void CUsbUiNotifApiTest::Update( TUsbUiNotifApiTestResult& aTestResult  )
    {
    TRACE_INFO(  _L(">>>Update") );
    TInt count = iNotifierArray->Count();
    TInt i=0;    
    _LIT8(test, "test");
    TBuf8<32> buf(test);
    for (i=0; i<count; i++)
        {
        iNotifierArray->At(i)->UpdateL(buf);        
        }    
    
    aTestResult = ETestCasePassed;
    
    TRACE_INFO(  _L("<<<Update") );
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::Notifierstart
// -----------------------------------------------------------------------------

void CUsbUiNotifApiTest::Notifierstart( TUsbUiNotifApiTestResult& aTestResult )
    {
    TRACE_INFO(  _L(">>>Notifierstart") );
    TInt count = iNotifierArray->Count();
    TInt i=0;    
    _LIT8(test, "test");
    TBuf8<32> buf(test);
    for (i=0; i<count; i++)
        {
        iNotifierArray->At(i)->StartL(buf);        
        }
    
    aTestResult = ETestCasePassed;
    
    TRACE_INFO(  _L("<<<Notifierstart") );
    }


// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::GetQueryType
// -----------------------------------------------------------------------------

TInt CUsbUiNotifApiTest::GetQueryType( TPtrC aTypeString, TUSBUIQueries& aQueryType )
    {
    TRACE_INFO(  _L(">>>GetQueryType") );
    if ( !aTypeString.Compare( _L( "EUSBNoMemoryCard" ) ) )
        {
        aQueryType = EUSBNoMemoryCard;
        TRACE_INFO(  _L("Query type: EUSBNoMemoryCard") );
        }
    else if ( !aTypeString.Compare( _L( "EUSBStorageMediaFailure" ) ) )
        {
        aQueryType = EUSBStorageMediaFailure;
        TRACE_INFO(  _L("Query type: EUSBStorageMediaFailure") );
        }
    else if ( !aTypeString.Compare( _L( "EUSBChangeFromMassStorage" ) ) )
        {
        aQueryType = EUSBChangeFromMassStorage;
        TRACE_INFO(  _L("Query type: EUSBChangeFromMassStorage") );
        }    
    else
        {
        TRACE_INFO(  _L("Query type: not supported") );
        return KErrNotFound;
        }
    TRACE_INFO(  _L("<<<GetQueryType") );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::GetOTGErrorType
// -----------------------------------------------------------------------------

TInt CUsbUiNotifApiTest::GetOTGErrorType( TPtrC aTypeString, TUsbUiNotifOtgError& aQueryType )
    {
    TRACE_INFO(  _L(">>>GetOTGErrorType") );
    
    if ( !aTypeString.Compare( _L( "EUsbOtgTooMuchPower" ) ) )
        {
        aQueryType = EUsbOtgTooMuchPower;
        TRACE_INFO(  _L("Query type: EUsbOtgTooMuchPower") );
        }    
    else if ( !aTypeString.Compare( _L( "EUsbOtgTooMuchPowerRequired" ) ) )
        {
        aQueryType = EUsbOtgTooMuchPowerRequired;
        TRACE_INFO(  _L("Query type: EUsbOtgTooMuchPowerRequired") );
        }    
    else if ( !aTypeString.Compare( _L( "EUsbOtgUnsupportedDevice" ) ) )
        {
        aQueryType = EUsbOtgUnsupportedDevice;
        TRACE_INFO(  _L("Query type: EUsbOtgUnsupportedDevice") );
        }        
    else if ( !aTypeString.Compare( _L( "EUsbOtgHubUnsupported" ) ) )
        {
        aQueryType = EUsbOtgHubUnsupported;
        TRACE_INFO(  _L("Query type: EUsbOtgHubUnsupported") );
        }            
    else if ( !aTypeString.Compare( _L( "EUsbOtgErrorInConnection" ) ) )
        {
        aQueryType = EUsbOtgErrorInConnection;
        TRACE_INFO(  _L("Query type: EUsbOtgErrorInConnection") );
        }                
    else if ( !aTypeString.Compare( _L( "EUsbOtgErrorAttachTimedOut" ) ) )
        {
        aQueryType = EUsbOtgErrorAttachTimedOut;
        TRACE_INFO(  _L("Query type: EUsbOtgErrorAttachTimedOut") );
        }                    
    else
        {
        TRACE_INFO(  _L("Query type: not supported") );
        return KErrNotFound;
        } 
    
    TRACE_INFO(  _L("<<<GetOTGErrorType") );
    return KErrNone;    
    }


// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::GetOTGWarningType
// -----------------------------------------------------------------------------

TInt CUsbUiNotifApiTest::GetOTGWarningType( TPtrC aTypeString, TUsbUiNotifOtgWarning& aQueryType )
    {        
    TRACE_INFO(  _L(">>>GetOTGWarningType") );
    
    if ( !aTypeString.Compare( _L( "EUsbOtgPartiallySupportedDevice" ) ) )
        {            
        aQueryType = EUsbOtgPartiallySupportedDevice;
        TRACE_INFO(  _L("Query type: EUsbOtgPartiallySupportedDevice;") );
        }    
    else
        {
        TRACE_INFO(  _L("Query type: not supported") );
        return KErrNotFound;
        }

    TRACE_INFO(  _L("<<<GetOTGWarningType") );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::GetMSMMrrorType
// -----------------------------------------------------------------------------

TInt CUsbUiNotifApiTest::GetMSMMrrorType( TPtrC aTypeString, THostMsErrCode& aQueryType )
    {        
    TRACE_INFO(  _L(">>>GetMSMMrrorType") );
    if ( !aTypeString.Compare( _L( "EUsbMSMMGeneralError" ) ) )
        {            
        aQueryType = EHostMsErrGeneral;
        TRACE_INFO(  _L("Query type: EUsbOtgPartiallySupportedDevice;") );
        }    
    else if ( !aTypeString.Compare( _L( "EUsbMSMMUnknownFileSystem" ) ) )
        {
        aQueryType = EHostMsErrUnknownFileSystem;
        TRACE_INFO(  _L("Query type: EUsbMSMMUnknownFileSystem") );
        }         
    else if ( !aTypeString.Compare( _L( "EUsbMSMMOutOfMemory" ) ) )
        {
        aQueryType = EHostMsErrOutOfMemory;
        TRACE_INFO(  _L("Query type: EUsbMSMMOutOfMemory") );
        }     
    else
        {
        TRACE_INFO(  _L("Query type: not supported") );
        return KErrNotFound;
        }

    TRACE_INFO(  _L("<<<GetMSMMrrorType") );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::GetTestOption
// -----------------------------------------------------------------------------

TInt CUsbUiNotifApiTest::GetTestOption( TPtrC aOptionString, TTestOption& aOption )
    {    
    if ( !aOptionString.Compare( _L( "DISCARD" ) ) )
        {
        aOption = EQueryDiscarded;
        TRACE_INFO(  _L("Test type: EQueryDiscarded") );
        }
    else if ( !aOptionString.Compare( _L( "ACCEPT" ) ) )
        {
        aOption = EQueryAccepted;
        TRACE_INFO(  _L("Test type: EQueryAccepted") );
        }
    else if ( !aOptionString.Compare( _L( "CANCEL" ) ) )
        {
        aOption = EQueryCanceled;
        TRACE_INFO(  _L("Test type: EQueryCanceled") );
        }
    else
        {
        TRACE_INFO(  _L("Test type: not supported") );
        return KErrNotFound;
        }
 
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUsbUiNotifApiTest::GetPersonalityIdFromString
// -----------------------------------------------------------------------------

TInt CUsbUiNotifApiTest::GetPersonalityIdFromString( TPtrC aOptionString, TInt& aPersonalityId )
    {    
    if ( !aOptionString.Compare( _L( "MS" ) ) )
        {
        aPersonalityId = KUsbPersonalityIdMS;
        TRACE_INFO(  _L("Usb personality type: KUsbPersonalityIdMS") );
        }
    else if ( !aOptionString.Compare( _L( "PCSUITE" ) ) )
        {
        aPersonalityId = KUsbPersonalityIdPCSuite;
        TRACE_INFO(  _L("Usb personality type: KUsbPersonalityIdPCSuite") );
        }
    else if ( !aOptionString.Compare( _L( "MTP" ) ) )
        {
        aPersonalityId = KUsbPersonalityIdMTP;
        TRACE_INFO(  _L("Usb personality type: KUsbPersonalityIdMTP") );
        }
    else if ( !aOptionString.Compare( _L( "PCSUITEMTP" ) ) )
        {
        aPersonalityId = KUsbPersonalityIdPCSuiteMTP;
        TRACE_INFO(  _L("Usb personality type: KUsbPersonalityIdPCSuiteMTP") );
        }
    else
        {
        TRACE_INFO(  _L("Usb personality type: not found") );
        return KErrNotFound;  
        }
 
    return KErrNone;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
