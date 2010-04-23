/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
#include "USBUiApiTest.h"

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
// CUSBUiApiTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CUSBUiApiTest::Delete() 
    {
    iApaLsSession.Close();
    delete iCommandLine;
    iCommandLine = NULL;
    delete iRepository;
    iRepository = NULL;
    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CUSBUiApiTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove
	    ENTRY( "ExecuteApiTestBlock", CUSBUiApiTest::ExecuteApiTestBlock ),
        ENTRY( "ExecuteModuleTestBlock", CUSBUiApiTest::ExecuteModuleTestBlock ),
        ENTRY( "ExecuteBranchTestBlock", CUSBUiApiTest::ExecuteBranchTestBlock ),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::GetTestBlockParamsL
// -----------------------------------------------------------------------------

void CUSBUiApiTest::GetTestBlockParamsL( CStifItemParser& aItem )
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
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "LaunchUsbUiApp" ) ) )
        {       
        User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption1 ) );        
        User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption2 ) );      
        }
	else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "TurnLightsOn" ) ) )
		{         
		}
	else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "FinishTest" ) ) )
		{       
		User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption1 ) ); 
		if( iTestBlockParams.iTestOption1.Compare(_L("CANCELLED")) == KErrNotFound )
			{
			User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption2 ) );        
			User::LeaveIfError( aItem.GetNextString( iTestBlockParams.iTestOption3 ) );
			}
		}
    else
        {
        TRACE_INFO( _L("GetTestBlockParamsL() Test type: not found") );
        User::Leave( KErrNotFound );
        }
    TRACE_INFO( _L("<<< GetTestBlockParamsL") );
    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::ExecuteApiTestBlock
// -----------------------------------------------------------------------------

TInt CUSBUiApiTest::ExecuteApiTestBlock( CStifItemParser& aItem )
    {
    TRACE_INFO( _L(">>> ExecuteApiTestBlock") );
	
	TInt res;
    TUSBUiApiTestResult testResult;
	
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
	
	
void CUSBUiApiTest::DoExecuteApiTestBlockL( CStifItemParser& aItem, TUSBUiApiTestResult& aTestResult )
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
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "LaunchUsbUiApp" ) ) )
        {      
        LaunchUsbUiApp( iTestBlockParams.iTestOption1, iTestBlockParams.iTestOption2, aTestResult );
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "FinishTest" ) ) )
        {      
        FinishTest( iTestBlockParams.iTestOption1, iTestBlockParams.iTestOption2, iTestBlockParams.iTestOption3, aTestResult );
        }
    else if ( !iTestBlockParams.iTestBlockName.Compare( _L( "TurnLightsOn" ) ) )
        {      
        TurnLightsOn(  );
        }
    else
        {
    TRACE_INFO( _L("DoExecuteApiTestBlockL() Test type: not found") );
        User::Leave( KErrNotFound );
        }
	
    TRACE_INFO( _L("<<<DoExecuteApiTestBlockL") );
    }
	
// -----------------------------------------------------------------------------
// CUSBUiApiTest::ExecuteModuleTestBlock
// -----------------------------------------------------------------------------	

TInt CUSBUiApiTest::ExecuteModuleTestBlock( CStifItemParser& aItem )
    {
    TRACE_INFO( _L(">>>ExecuteModuleTestBlock") );
	
    TInt res;
    TUSBUiApiTestResult testResult;
    
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
	
	
void CUSBUiApiTest::DoExecuteModuleTestBlockL( CStifItemParser& aItem, TUSBUiApiTestResult& aTestResult )
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
// CUSBUiApiTest::ExecuteBranchTestBlock
// -----------------------------------------------------------------------------
	
TInt CUSBUiApiTest::ExecuteBranchTestBlock( CStifItemParser& aItem )
    {
    TRACE_INFO( _L(">>>ExecuteBranchTestBlock") );
	
    TInt res;
    TUSBUiApiTestResult testResult;
    
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

	
void CUSBUiApiTest::DoExecuteBranchTestBlockL( CStifItemParser& aItem, TUSBUiApiTestResult& aTestResult )
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
// CUSBUiApiTest::ExampleTestL
// -----------------------------------------------------------------------------

void CUSBUiApiTest::ExampleTestL( TPtrC aTestOption, TPtrC aTestSubOption, 
        TInt aTestIntOption, TInt aTestCharOption, TUSBUiApiTestResult& aTestResult )
    {
    
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
// CUSBUiApiTest::LaunchUsbUiApp
// -----------------------------------------------------------------------------

TInt CUSBUiApiTest::LaunchUsbUiApp( TPtrC aTestOption, TPtrC aTestSubOption, TUSBUiApiTestResult& aTestResult )
    {
    TInt res;
    TInt testAskOnConnectionSetting;
    TInt testUsbPersonalityId;
        
    res = GetAskOnConnectionOptionFromString( aTestOption, testAskOnConnectionSetting );   
    if ( res != KErrNone )
        {
        TRACE_INFO( (_L("GetAskOnConnectionOptionFromString failed with value: %d"), res) );
        iApaLsSession.Close();
        return res;
        }
    TRACE_INFO( (_L("GetAskOnConnectionOptionFromString OK:  %d"), testAskOnConnectionSetting) );
    
    
    res = GetUsbPersonalityOptionFromString( aTestSubOption, testUsbPersonalityId );   
    if ( res != KErrNone )
        {
        TRACE_INFO( (_L("GetUsbPersonalityOptionFromString failed with value: %d"), res) );
        iApaLsSession.Close();
        return res;
        }
    TRACE_INFO( (_L("GetUsbPersonalityOptionFromString OK: %d"),testUsbPersonalityId) );
    
    res = GetUsbSettings( iUsbPersonalityBackup, iAskOnConnectionSettingBackup );
    if ( res != KErrNone )
        {
        TRACE_INFO( (_L("Failed to backup usb settings with value: %d"), res) );
        return res;
        }
    TRACE_INFO( (_L("GetUsbSettings OK: %d, %d"), iUsbPersonalityBackup, iAskOnConnectionSettingBackup) );
    
    res = SetUsbSettings( testUsbPersonalityId, testAskOnConnectionSetting );
    if ( res != KErrNone )
        {
        TRACE_INFO( (_L("Changing usb settings failed with value (cenrep): %d"), res) );
        return res;
        }
    TRACE_INFO( (_L("SetUsbSettings OK: %d, %d"), testUsbPersonalityId, testAskOnConnectionSetting) );
  
    iCommandLine->SetCommandL( EApaCommandRun );
    iCommandLine->SetExecutableNameL( KUsbAppFileName );

    res = iApaLsSession.Connect();
    if ( res != KErrNone )
        {
        TRACE_INFO( (_L("Connecting to application server failed with value: %d"), res) );
        return res;

        }
    STIF_LOG("iApaLsSession.Connect OK");

    res = iApaLsSession.StartApp( *iCommandLine );
    if ( res != KErrNone )
        {
        TRACE_INFO( (_L("Starting application failed with value: %d"), res) );
        iApaLsSession.Close();
        return res;
        }
    STIF_LOG("iApaLsSession.StartApp OK");
    
    if ( !IsUsbAppRunning() )
        {
        TRACE_INFO( _L("Usb application launch error") );
        iApaLsSession.Close();
        return KErrGeneral;
        }
    STIF_LOG("IsUsbAppRunning OK");

    TRACE_INFO( _L("Usb application started") );

    aTestResult = ETestCasePassed;
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::FinishTest
// -----------------------------------------------------------------------------

TInt CUSBUiApiTest::FinishTest( TPtrC aTestOption, TPtrC aTestSubOption, TPtrC aTestSubOption2, TUSBUiApiTestResult& aTestResult )
    {
    TRACE_INFO( _L(">>>FinishTest") );
    
    TInt res;
    TUSBAppTestOption option;
    TBool isAppRunning;
    TInt actualAskOnConnectionSetting;
    TInt expectedAskOnConnectionSetting;
    TInt actualPersonalityIdSetting;
    TInt expectedPersonalityIdSetting;
        
    res = GetTestOption( aTestOption, option );
    if ( res != KErrNone )
        {
        TRACE_INFO( (_L("GetTestOption failed with value: %d"), res) );
        iApaLsSession.Close();
        return res;
        }
    TRACE_INFO( (_L("GetTestOption OK: %d"),option) );
    
    if ( option == EAskOnConnectionSettingChanged || option == EUsbConnectionModeSettingChanged )
        {     
        res = GetAskOnConnectionOptionFromString( aTestSubOption, expectedAskOnConnectionSetting );   
        if ( res != KErrNone )
            {
            TRACE_INFO( (_L("GetAskOnConnectionOptionFromString failed with value: %d"), res) );
            iApaLsSession.Close();
            return res;
            }
        TRACE_INFO( (_L("GetAskOnConnectionOptionFromString OK: %d"), expectedAskOnConnectionSetting) );
                
        res = GetUsbPersonalityOptionFromString( aTestSubOption2, expectedPersonalityIdSetting );   
        if ( res != KErrNone )
            {
            TRACE_INFO( (_L("GetUsbPersonalityOptionFromString failed with value: %d"), res) );
            iApaLsSession.Close();
            return res;
            }
        TRACE_INFO( (_L("GetUsbPersonalityOptionFromString OK %d"), expectedPersonalityIdSetting) );
        }
    
    isAppRunning = IsUsbAppRunning();

    res = GetUsbSettings( actualPersonalityIdSetting, actualAskOnConnectionSetting );
    if ( res != KErrNone )
        {
        TRACE_INFO( (_L("Getting usb settings failed with value (cenrep): %d"), res) );
        iApaLsSession.Close();
        return res;
        }
    TRACE_INFO( (_L("GetUsbSettings OK: %d, %d"), actualPersonalityIdSetting, actualAskOnConnectionSetting) );
    
    res = SetUsbSettings( iUsbPersonalityBackup, iAskOnConnectionSettingBackup );
    if ( res != KErrNone )
        TRACE_INFO( _L("Failed to restore original usb settings!") );
    TRACE_INFO( (_L("SetUsbSettings OK: %d, %d"), iUsbPersonalityBackup, iAskOnConnectionSettingBackup) );
    
    iApaLsSession.Close();
    
    switch( option )
        {
        case EAskOnConnectionSettingChanged:       
        case EUsbConnectionModeSettingChanged:
            TRACE_INFO( (_L("Actual USB ask on connection setting value: %d, expected: %d"), actualAskOnConnectionSetting, expectedAskOnConnectionSetting) );
            STIF_ASSERT_EQUALS( expectedAskOnConnectionSetting, actualAskOnConnectionSetting );
            TRACE_INFO( (_L("Actual USB personality setting value: %d, expected: %d"), actualPersonalityIdSetting, expectedPersonalityIdSetting) );
            STIF_ASSERT_EQUALS( expectedPersonalityIdSetting, actualPersonalityIdSetting );   
            TRACE_INFO( (_L("USB app is running: %d, expected: %d"), isAppRunning, EFalse) );
            STIF_ASSERT_FALSE( isAppRunning );
            break;
        case EAppCancelled:
            TRACE_INFO( (_L("USB app is running: %d, expected: %d"), isAppRunning, EFalse) );
            STIF_ASSERT_FALSE( isAppRunning );
            break;
        default:
            return KErrNotFound;
        }
      
    TRACE_INFO( _L("Test case passed!") );
    aTestResult = ETestCasePassed;
    
    TRACE_INFO( _L("<<<FinishTest") );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::GetTestOption
// -----------------------------------------------------------------------------

TInt CUSBUiApiTest::GetTestOption( TPtrC aOptionString, TUSBAppTestOption& aOption )
    {    
    if ( !aOptionString.Compare( _L( "USB_CONN_MODE" ) ) )
        {
        aOption = EUsbConnectionModeSettingChanged;
        TRACE_INFO( _L("Test type: EUsbConnectionModeSettingChanged") );
        }
    else if ( !aOptionString.Compare( _L( "ASK_ON_CONN" ) ) )
        {
        aOption = EAskOnConnectionSettingChanged;
        TRACE_INFO( _L("Test type: EAskOnConnectionSettingChanged") );
        }
    else if ( !aOptionString.Compare( _L( "CANCELLED" ) ) )
        {
        aOption = EAppCancelled;
        TRACE_INFO( _L("Test type: EAppCancelled") );
        }
    else
        {
        TRACE_INFO( _L("Test type: not supported") );
        return KErrNotFound;
        }
 
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::GetAskOnConnectionOptionFromString
// -----------------------------------------------------------------------------

TInt CUSBUiApiTest::GetAskOnConnectionOptionFromString( TPtrC aOptionString, TInt& aOption )
    {    
    if ( !aOptionString.Compare( _L( "ON" ) ) )
        {
        aOption = KUsbWatcherChangeOnConnectionOn;
        TRACE_INFO( _L("Ask on connection option: ON") );
        }
    else if ( !aOptionString.Compare( _L( "OFF" ) ) )
        {
        aOption = KUsbWatcherChangeOnConnectionOff;
        TRACE_INFO( _L("Ask on connection option: OFF") );
        }
    else
        {
        TRACE_INFO( _L("Ask on connection option: not supported") );
        return KErrNotFound;
        }
 
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::GetUsbPersonalityOptionFromString
// -----------------------------------------------------------------------------

TInt CUSBUiApiTest::GetUsbPersonalityOptionFromString( TPtrC aOptionString, TInt& aOption )
    {    
    if ( !aOptionString.Compare( _L( "PCSUITE" ) ) )
        {
        aOption = KUsbPersonalityIdPCSuiteMTP;
        TRACE_INFO( _L("Usb personality option: KUsbPersonalityIdPCSuiteMTP") );
        }
    else if ( !aOptionString.Compare( _L( "MS" ) ) )
        {
        aOption = KUsbPersonalityIdMS;
        TRACE_INFO( _L("Usb personality option: KUsbPersonalityIdMS") );
        }
    else if ( !aOptionString.Compare( _L( "PTP" ) ) )
        {
        aOption = KUsbPersonalityIdPTP;
        TRACE_INFO( _L("Usb personality option: KUsbPersonalityIdPTP") );
        }
    else if ( !aOptionString.Compare( _L( "MTP" ) ) )
        {
        aOption = KUsbPersonalityIdMTP;
        TRACE_INFO( _L("Usb personality option: KUsbPersonalityIdMTP") );
        }
	else if ( !aOptionString.Compare( _L( "MODEM" ) ) )
		{
		aOption = KUsbPersonalityIdModemInst;
		TRACE_INFO( _L("Usb personality option: KUsbPersonalityIdModemInst") );
		}
    else
        {
        TRACE_INFO( _L("Usb personality option: not supported") );
        return KErrNotFound;
        }
 
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::IsUsbAppRunning
// -----------------------------------------------------------------------------

TBool CUSBUiApiTest::IsUsbAppRunning()
    {
    TFindProcess processFinder;
    TFullName processName;
    TBool isAppRunning = EFalse;
    RProcess matchedProcess;
    
    while ( processFinder.Next( processName ) == KErrNone )
            {
            matchedProcess.Open( processFinder );
            if ( matchedProcess.SecureId() == iUsbAppSecureId )
                isAppRunning = ETrue;
            }
    
    matchedProcess.Close();    
    return isAppRunning;
    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::GetUsbSettings
// -----------------------------------------------------------------------------

TInt CUSBUiApiTest::GetUsbSettings( TInt& aUsbPersonalityId, TInt& aAskOnConnectionSetting )
    {
    TInt res;   
    res = iRepository -> Get( KUsbWatcherChangeOnConnectionSetting, aAskOnConnectionSetting );
    if ( res != KErrNone )
        return res;
    
    res = iRepository -> Get( KUsbWatcherPersonality, aUsbPersonalityId );
    if ( res != KErrNone )
        return res;
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::SetUsbSettings
// -----------------------------------------------------------------------------

TInt CUSBUiApiTest::SetUsbSettings( TInt aUsbPersonalityId, TInt aAskOnConnectionSetting )
    {
    TInt res;   
    res = iRepository -> Set( KUsbWatcherChangeOnConnectionSetting, aAskOnConnectionSetting );
    if ( res != KErrNone )
        return res;
    
    res = iRepository -> Set( KUsbWatcherPersonality, aUsbPersonalityId );
    if ( res != KErrNone )
        return res;
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::TurnLightsOn
// -----------------------------------------------------------------------------

TInt CUSBUiApiTest::TurnLightsOn(  )
    {
    TInt res;
    res = RProperty::Set(KPSUidCoreApplicationUIs, KLightsControl, ELightsOn);
    res = RProperty::Set(KPSUidCoreApplicationUIs, KLightsVTForcedLightsOn, EForcedLightsOn);
    return res;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
