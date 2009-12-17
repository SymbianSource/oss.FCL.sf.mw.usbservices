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
* Description:  ?Description
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
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "LaunchUsbUiApp", CUSBUiApiTest::LaunchUsbUiApp ),
        ENTRY( "FinishTest", CUSBUiApiTest::FinishTest ),
        ENTRY( "TurnLightsOn", CUSBUiApiTest::TurnLightsOn ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::LaunchUsbUiApp
// -----------------------------------------------------------------------------

TInt CUSBUiApiTest::LaunchUsbUiApp( CStifItemParser& aItem )
    {
    TInt res;
    TPtrC testAskOnConnectionSettingName( KNullDesC );
    TPtrC testUsbPersonalityName( KNullDesC );
    TInt testAskOnConnectionSetting;
    TInt testUsbPersonalityId;
    
    res = aItem.GetString( _L( "LaunchUsbUiApp" ), testAskOnConnectionSettingName );   
    if ( res != KErrNone )
        {
        iLog -> Log( _L("GetString failed with value: %d"), res );
        iApaLsSession.Close();
        return res;
        }
    //STIF_LOG("aItem.GetString testAskOnConnectionSettingName OK");
    
    res = GetAskOnConnectionOptionFromString( testAskOnConnectionSettingName, testAskOnConnectionSetting );   
    if ( res != KErrNone )
        {
        iLog -> Log( _L("GetAskOnConnectionOptionFromString failed with value: %d"), res );
        iApaLsSession.Close();
        return res;
        }
    //STIF_LOG1("GetAskOnConnectionOptionFromString OK:  %d", testAskOnConnectionSetting);
    
    res = aItem.GetNextString( testUsbPersonalityName );   
    if ( res != KErrNone )
        {
        iLog -> Log( _L("GetNextString failed with value: %d"), res );
        iApaLsSession.Close();
        return res;
        }
    //STIF_LOG("aItem.GetNextString testUsbPersonalityName OK");
    
    res = GetUsbPersonalityOptionFromString( testUsbPersonalityName, testUsbPersonalityId );   
    if ( res != KErrNone )
        {
        iLog -> Log( _L("GetUsbPersonalityOptionFromString failed with value: %d"), res );
        iApaLsSession.Close();
        return res;
        }
    //STIF_LOG1("GetUsbPersonalityOptionFromString OK: %d",testUsbPersonalityId);
    
    res = GetUsbSettings( iUsbPersonalityBackup, iAskOnConnectionSettingBackup );
    if ( res != KErrNone )
        {
        iLog -> Log( _L("Failed to backup usb settings with value: %d"), res );
        return res;
        }
    STIF_LOG2("GetUsbSettings OK: %d, %d", iUsbPersonalityBackup, iAskOnConnectionSettingBackup);
    
    res = SetUsbSettings( testUsbPersonalityId, testAskOnConnectionSetting );
    if ( res != KErrNone )
        {
        iLog -> Log( _L("Changing usb settings failed with value (cenrep): %d"), res );
        return res;
        }
    STIF_LOG2("SetUsbSettings OK: %d, %d", testUsbPersonalityId, testAskOnConnectionSetting);
  
    iCommandLine->SetCommandL( EApaCommandRun );
    iCommandLine->SetExecutableNameL( KUsbAppFileName );

    res = iApaLsSession.Connect();
    if ( res != KErrNone )
        {
        iLog->Log( _L("Connecting to application server failed with value: %d"), res );
        return res;
        }
    STIF_LOG("iApaLsSession.Connect OK");

    res = iApaLsSession.StartApp( *iCommandLine );
    if ( res != KErrNone )
        {
        iLog->Log( _L("Starting application failed with value: %d"), res );
        iApaLsSession.Close();
        return res;
        }
    STIF_LOG("iApaLsSession.StartApp OK");
    
    if ( !IsUsbAppRunning() )
        {
        iLog->Log( _L("Usb application launch error") );
        iApaLsSession.Close();
        return KErrGeneral;
        }
    STIF_LOG("IsUsbAppRunning OK");

    iLog->Log( _L("Usb application started") );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::FinishTest
// -----------------------------------------------------------------------------

TInt CUSBUiApiTest::FinishTest( CStifItemParser& aItem )
    {
    TInt res;
    TPtrC optionName( KNullDesC );
    TUSBAppTestOption option;
    TBool isAppRunning;
    TPtrC expectedAskOnConnectionSettingName( KNullDesC );
    TPtrC expectedPersonalityIdSettingName( KNullDesC );
    TInt actualAskOnConnectionSetting;
    TInt expectedAskOnConnectionSetting;
    TInt actualPersonalityIdSetting;
    TInt expectedPersonalityIdSetting;
    
    res = aItem.GetString( _L( "FinishTest" ), optionName );   
    if ( res != KErrNone )
        {
        iLog -> Log( _L("GetString failed with value: %d"), res );
        iApaLsSession.Close();
        return res;
        }
    STIF_LOG("aItem.GetString optionName OK");
    
    res = GetTestOption( optionName, option );
    if ( res != KErrNone )
        {
        iLog -> Log( _L("GetTestOption failed with value: %d"), res );
        iApaLsSession.Close();
        return res;
        }
    STIF_LOG1("GetTestOption OK: %d",option);
    
    if ( option == EAskOnConnectionSettingChanged || option == EUsbConnectionModeSettingChanged )
        {
        res = aItem.GetNextString( expectedAskOnConnectionSettingName );   
        if ( res != KErrNone )
            {
            iLog -> Log( _L("GetNextString failed with value: %d"), res );
            iApaLsSession.Close();
            return res;
            }
        STIF_LOG("aItem.GetNextString expectedAskOnConnectionSettingName OK");
        
        res = GetAskOnConnectionOptionFromString( expectedAskOnConnectionSettingName, expectedAskOnConnectionSetting );   
        if ( res != KErrNone )
            {
            iLog -> Log( _L("GetAskOnConnectionOptionFromString failed with value: %d"), res );
            iApaLsSession.Close();
            return res;
            }
        STIF_LOG1("GetAskOnConnectionOptionFromString OK: %d", expectedAskOnConnectionSetting);
        
        res = aItem.GetNextString( expectedPersonalityIdSettingName );   
        if ( res != KErrNone )
            {
            iLog -> Log( _L("GetNextString failed with value: %d"), res );
            iApaLsSession.Close();
            return res;
            }
        STIF_LOG("aItem.GetNextString expectedPersonalityIdSettingName OK");
        
        res = GetUsbPersonalityOptionFromString( expectedPersonalityIdSettingName, expectedPersonalityIdSetting );   
        if ( res != KErrNone )
            {
            iLog -> Log( _L("GetUsbPersonalityOptionFromString failed with value: %d"), res );
            iApaLsSession.Close();
            return res;
            }
        STIF_LOG1("GetUsbPersonalityOptionFromString OK %d", expectedPersonalityIdSetting);
        }
    
    isAppRunning = IsUsbAppRunning();

    res = GetUsbSettings( actualPersonalityIdSetting, actualAskOnConnectionSetting );
    if ( res != KErrNone )
        {
        iLog -> Log( _L("Getting usb settings failed with value (cenrep): %d"), res );
        iApaLsSession.Close();
        return res;
        }
    STIF_LOG2("GetUsbSettings OK: %d, %d", actualPersonalityIdSetting, actualAskOnConnectionSetting);
    
    res = SetUsbSettings( iUsbPersonalityBackup, iAskOnConnectionSettingBackup );
    if ( res != KErrNone )
        iLog -> Log( _L("Failed to restore original usb settings!") );
    STIF_LOG2("SetUsbSettings OK: %d, %d", iUsbPersonalityBackup, iAskOnConnectionSettingBackup);
    
    iApaLsSession.Close();
    
    switch( option )
        {
        case EAskOnConnectionSettingChanged:       
        case EUsbConnectionModeSettingChanged:
            iLog -> Log( _L("Actual USB ask on connection setting value: %d, expected: %d"), actualAskOnConnectionSetting, expectedAskOnConnectionSetting );
            STIF_ASSERT_EQUALS( expectedAskOnConnectionSetting, actualAskOnConnectionSetting );
            iLog -> Log( _L("Actual USB personality setting value: %d, expected: %d"), actualPersonalityIdSetting, expectedPersonalityIdSetting );
            STIF_ASSERT_EQUALS( expectedPersonalityIdSetting, actualPersonalityIdSetting );   
            iLog -> Log( _L("USB app is running: %d, expected: %d"), isAppRunning, EFalse );
            STIF_ASSERT_FALSE( isAppRunning );
            break;
        case EAppCancelled:
            iLog -> Log( _L("USB app is running: %d, expected: %d"), isAppRunning, EFalse );
            STIF_ASSERT_FALSE( isAppRunning );
            break;
        default:
            return KErrNotFound;
        }
      
    iLog -> Log( _L("Test case passed!") );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CUSBUiApiTest::FinishTest
// -----------------------------------------------------------------------------

TInt CUSBUiApiTest::GetTestOption( TPtrC aOptionString, TUSBAppTestOption& aOption )
    {    
    if ( !aOptionString.Compare( _L( "USB_CONN_MODE" ) ) )
        {
        aOption = EUsbConnectionModeSettingChanged;
        iLog -> Log( _L("Test type: EUsbConnectionModeSettingChanged") );
        }
    else if ( !aOptionString.Compare( _L( "ASK_ON_CONN" ) ) )
        {
        aOption = EAskOnConnectionSettingChanged;
        iLog -> Log( _L("Test type: EAskOnConnectionSettingChanged") );
        }
    else if ( !aOptionString.Compare( _L( "CANCELLED" ) ) )
        {
        aOption = EAppCancelled;
        iLog -> Log( _L("Test type: EAppCancelled") );
        }
    else
        {
        iLog -> Log( _L("Test type: not supported") );
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
        iLog -> Log( _L("Ask on connection option: ON") );
        }
    else if ( !aOptionString.Compare( _L( "OFF" ) ) )
        {
        aOption = KUsbWatcherChangeOnConnectionOff;
        iLog -> Log( _L("Ask on connection option: OFF") );
        }
    else
        {
        iLog -> Log( _L("Ask on connection option: not supported") );
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
        iLog -> Log( _L("Usb personality option: KUsbPersonalityIdPCSuiteMTP") );
        }
    else if ( !aOptionString.Compare( _L( "MS" ) ) )
        {
        aOption = KUsbPersonalityIdMS;
        iLog -> Log( _L("Usb personality option: KUsbPersonalityIdMS") );
        }
    else if ( !aOptionString.Compare( _L( "PTP" ) ) )
        {
        aOption = KUsbPersonalityIdPTP;
        iLog -> Log( _L("Usb personality option: KUsbPersonalityIdPTP") );
        }
    else if ( !aOptionString.Compare( _L( "MTP" ) ) )
        {
        aOption = KUsbPersonalityIdMTP;
        iLog -> Log( _L("Usb personality option: KUsbPersonalityIdMTP") );
        }
    else
        {
        iLog -> Log( _L("Usb personality option: not supported") );
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

TInt CUSBUiApiTest::TurnLightsOn( CStifItemParser& aItem )
    {
    TInt res;
    res = RProperty::Set(KPSUidCoreApplicationUIs, KLightsControl, ELightsOn);
    return res;
    }
// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
