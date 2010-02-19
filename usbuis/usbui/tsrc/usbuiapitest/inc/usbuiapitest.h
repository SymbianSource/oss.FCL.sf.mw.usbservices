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




#ifndef USBUIAPITEST_H
#define USBUIAPITEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

#include "coreapplicationuisdomainpskeys.h"
#include <SettingsInternalCRKeys.h>
#include <centralrepository.h>
#include <apgcli.h>
#include <apacmdln.h>
#include <UsbWatcherInternalCRKeys.h>
#include <usbpersonalityids.h>

#include "USBClassChangeUI.h"
// CONSTANTS
_LIT( KUsbAppFileName, "usbclasschangeui" );

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
//_LIT( KUSBUiApiTestLogPath, "\\logs\\testframework\\USBUiApiTest\\" );

// Logging path for ATS - for phone builds comment this line
_LIT( KUSBUiApiTestLogPath, "e:\\testing\\stiflogs\\" ); 

// Log file
_LIT( KUSBUiApiTestLogFile, "USBUiApiTest.txt" ); 
_LIT( KUSBUiApiTestLogFileWithTitle, "USBUiApiTest_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CUSBUiApiTest;

// DATA TYPES
//enum ?declaration

enum TUSBUiApiTestResult
    {
    ETestCasePassed,
    ETestCaseFailed
    };

enum TUSBAppTestOption
    {
    EAppCancelled = 0,
    EAskOnConnectionSettingChanged,//1
    EUsbConnectionModeSettingChanged//2
    };

//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

NONSHARABLE_CLASS( TUSBUiApiTestBlockParams )
    {
    public:
        TPtrC iTestBlockName;
        
        TPtrC iTestOption1;
        TPtrC iTestOption2;
        TPtrC iTestOption3;
        
        TInt iTestIntOption1;
        TInt iTestIntOption2;
        
        TChar iTestCharOption1;
        TChar iTestCharOption2;
    };

/**
*  CUSBUiApiTest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS( CUSBUiApiTest ) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CUSBUiApiTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CUSBUiApiTest();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CUSBUiApiTest( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        virtual TInt ExecuteApiTestBlock( CStifItemParser& aItem );
        virtual TInt ExecuteModuleTestBlock( CStifItemParser& aItem );
        virtual TInt ExecuteBranchTestBlock( CStifItemParser& aItem );
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

        void GetTestBlockParamsL( CStifItemParser& aItem );
        
    	void DoExecuteApiTestBlockL( CStifItemParser& aItem, TUSBUiApiTestResult& aTestResult );    	
    	void DoExecuteModuleTestBlockL( CStifItemParser& aItem, TUSBUiApiTestResult& aTestResult );    
    	void DoExecuteBranchTestBlockL( CStifItemParser& aItem, TUSBUiApiTestResult& aTestResult );
    	
        void ExampleTestL( TPtrC aTestOption, TPtrC aTestSubOption, 
                 TInt aTestIntOption, TInt aTestCharOption, TUSBUiApiTestResult& aTestResult );

        virtual TInt LaunchUsbUiApp( TPtrC aTestOption, TPtrC aTestSubOption, TUSBUiApiTestResult& aTestResult );
        virtual TInt FinishTest( TPtrC aTestOption, TPtrC aTestSubOption, TPtrC aTestSubOption2, TUSBUiApiTestResult& aTestResult );
        TInt GetTestOption( TPtrC aOptionString, TUSBAppTestOption& aOption );
        TInt GetAskOnConnectionOptionFromString( TPtrC aOptionString, TInt& aOption );
        TInt GetUsbPersonalityOptionFromString( TPtrC aOptionString, TInt& aOption );
        TBool IsUsbAppRunning();
        TInt GetUsbSettings( TInt& aUsbPersinalityId, TInt& aAskOnConnectionSetting );
        TInt SetUsbSettings( TInt aUsbPersinalityId, TInt aAskOnConnectionSetting );

        TInt TurnLightsOn( );   



        inline void Trace(TRefByValue<const TDesC8> aFmt, ...);
        inline void Trace(TRefByValue<const TDesC16> aFmt, ...);
	
    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        TUSBUiApiTestBlockParams iTestBlockParams;
        
        RApaLsSession iApaLsSession;
        
        CApaCommandLine *iCommandLine;
        CRepository *iRepository;
        
        TSecureId iUsbAppSecureId;
        TInt iAskOnConnectionSettingBackup;
        TInt iUsbPersonalityBackup;
        TInt iTestAskOnConnectionSetting;
        TInt iTestUsbPersonality;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;
        
    };

#include "USBUiApiTestDebug.inl"

#endif      // USBUIAPITEST_H

// End of File
