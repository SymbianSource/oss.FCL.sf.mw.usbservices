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


#ifndef USBMSCPERSONALITYTEST_H
#define USBMSCPERSONALITYTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

#include <cusbpersonalityplugin.h>
#include <cusbpersonalitynotifier.h>

#include "UsbMscPersonalityTestActiveObject.h"
#include <tusbpersonalityparams.h>
#include "DismountClient.h"

// CONSTANTS
// Logging path
_LIT( KUsbMscPersonalityTestLogPath, "\\logs\\testframework\\UsbMscPersonalityTest\\" );

// Logging path for ATS - for phone builds comment this line
//_LIT( KUsbMscPersonalityTestLogPath, "e:\\testing\\stiflogs\\" ); 

// Log file
_LIT( KUsbMscPersonalityTestLogFile, "UsbMscPersonalityTest.txt" ); 
_LIT( KUsbMscPersonalityTestLogFileWithTitle, "UsbMscPersonalityTest_[%S].txt" );

// MACROS
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Macros used to extract test block parameters
#define TESTENTRY( A, B )\
    if ( !iTestBlockParams.iTestBlockName.Compare( TPtrC( ( TText* ) L ##A ) ) )\
        { iTestBlockFunction = &B; iTestBlockFound = ETrue; }\
    if ( !iTestBlockParams.iTestBlockName.Compare( TPtrC( ( TText* ) L ##A ) ) )

#define TESTPARAM( A, B )\
    GetTestBlockParamL( ( TGetTestBlockParamOperation ) A, ( TTestBlockParamName ) B );

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// DATA TYPES

// Enum type used to determine test block execution result
enum TUsbMscPersonalityTestResult
    {
    ETestCasePassed,
    ETestCaseFailed
    };

// Enum type indicating test block parameter which should be used to store
// data extracted from test script
enum TTestBlockParamName
    {
    eTestOption1,
    eTestOption2,
    eTestOption3,    
    eTestIntOption1,
    eTestIntOption2,
    eTestIntOption3,   
    eTestCharOption1,
    eTestCharOption2,
    eTestCharOption3
    };

// Enum type used to indicate which get parameter function should be used
enum TGetTestBlockParamOperation
    {
    eGetString,
    eGetInt,
    eGetChar
    };

// CLASS DECLARATION
/**
*  TUsbMscPersonalityTestBlockParams test class for storing test block parameters.
*/
NONSHARABLE_CLASS( TUsbMscPersonalityTestBlockParams )
    {
    public:
        TPtrC iTestBlockName;
        
        TPtrC iTestOption1;
        TPtrC iTestOption2;
        TPtrC iTestOption3;
        
        TInt iTestIntOption1;
        TInt iTestIntOption2;
        TInt iTestIntOption3;
        
        TChar iTestCharOption1;
        TChar iTestCharOption2;
        TChar iTestCharOption3;
    };

/**
*  CUsbMscPersonalityTest test class for STIF Test Framework TestScripter.
*/
NONSHARABLE_CLASS( CUsbMscPersonalityTest ) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CUsbMscPersonalityTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CUsbMscPersonalityTest();

    public: // New functions
    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions
    protected:  // Functions from base classes
    private:

        /**
        * C++ default constructor.
        */
        CUsbMscPersonalityTest( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void TestDelete();
        
        /**
        * Constructs all test resources for test methods.
        */
        void TestConstructL();
        
        /**
        * Extracts test block parameters from script file
        */
        void TestBlocksInfoL();
        
        /**
        * Executes test block
        */
        TInt ExecuteTestBlock( CStifItemParser& aItem );
        
        /**
        * Executes proper get parameter function indicated by its arguments
        */
        void GetTestBlockParamL( TGetTestBlockParamOperation aOperation,
                                TTestBlockParamName aParamName );
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        // ADD NEW METHOD DEC HERE
        // [TestMethods] - Do not remove 
        void ExampleTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                            TUsbMscPersonalityTestResult& aTestResult );
	
        void ExecuteDismounting();
        
        void PreparePersonalityStartTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult );
        void FinishPersonalityStartTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult );
        void PreparePersonalityStopTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult );
        void FinishPersonalityStopTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult );
        void InitializationTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult );
        void StateChangeNotifyTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult );
        void StateChangeNotifyUndefinedTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                        TUsbMscPersonalityTestResult& aTestResult );
        void StateChangeNotifyStateAddressTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                        TUsbMscPersonalityTestResult& aTestResult );
        void ConfirmPersonalityUnloadTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult );
        void AllowDismountAfter2SecTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult );
        void AllowDismountTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult );
        void NotAllowDismountTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult );
        void AllowDismountTooLateTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult );
                
    
        void InitializeTestObjectsL();
        void DeleteTestObjects();
        
        
    public:     // Data
    protected:  // Data
    private:    // Data
        
        // Member object used to store test block parameters
        TUsbMscPersonalityTestBlockParams iTestBlockParams;
        
        // Used to indicate if test block with specified parameters exist
        TBool iTestBlockFound;
        
        // Function pointer used to call proper test block methods
        void ( CUsbMscPersonalityTest::*iTestBlockFunction )
            ( TUsbMscPersonalityTestBlockParams&, TUsbMscPersonalityTestResult& );
        
        // Not own
        CStifItemParser* iItem;

        // Reserved pointer for future extension
        //TAny* iReserved;

        // ADD NEW DATA DEC HERE
        
        RUsb* iUsb;
        CUsbPersonalityNotifier* iNotifier;
        TRequestStatus iRequestStatus;
        CActiveScheduler* iScheduler;
        CUsbMscPersonalityTestActiveObject* iActive;
        CUsbPersonalityPlugin* iPlugin;
        TUsbPersonalityParams* iParams;
        CDismountClient* iDismountClient;
        
    public:     // Friend classes
    protected:  // Friend classes
    private:    // Friend classes
        
    };
#endif      // USBMSCPERSONALITYTEST_H

// End of File
