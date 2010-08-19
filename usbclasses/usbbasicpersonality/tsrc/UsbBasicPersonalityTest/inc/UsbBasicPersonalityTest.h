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




#ifndef USBBASICPERSONALITYTEST_H
#define USBBASICPERSONALITYTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>             
#include <TestclassAssert.h>
#include <cusbpersonalityplugin.h>
#include "TUsbPersonalityParams.h"

class RUsb;
class CUsbPersonalityNotifier;


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
//_LIT( KUsbBasicPersonalityTestLogPath, "\\logs\\testframework\\UsbBasicPersonalityTest\\" );

// Logging path for ATS - for phone builds comment this line
_LIT( KUsbBasicPersonalityTestLogPath, "e:\\testing\\stiflogs\\" ); 

// Log file
_LIT( KUsbBasicPersonalityTestLogFile, "UsbBasicPersonalityTest.txt" ); 
_LIT( KUsbBasicPersonalityTestLogFileWithTitle, "UsbBasicPersonalityTest_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CUsbBasicPersonalityTest;

// DATA TYPES
//enum ?declaration

enum TUsbBasicPersonalityTestResult
    {
    ETestCasePassed,
    ETestCaseFailed
    };

//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CUsbBasicPersonalityTest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CUsbBasicPersonalityTest) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CUsbBasicPersonalityTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CUsbBasicPersonalityTest();

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
        CUsbBasicPersonalityTest( CTestModuleIf& aTestModuleIf );

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

        virtual TInt ExecuteApiTest( CStifItemParser& aItem );
        virtual TInt ExecuteModuleTest( CStifItemParser& aItem );
        virtual TInt ExecuteBranchTest( CStifItemParser& aItem );
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

	void DoExecuteApiTestL( TPtrC aApiTestName, TUsbBasicPersonalityTestResult& aTestResult );
	void ExampleApiTestL( TUsbBasicPersonalityTestResult& aTestResult );
	
	void DoExecuteModuleTestL( TPtrC aModuleTestName, TUsbBasicPersonalityTestResult& aTestResult );
	void ExampleModuleTestL( TUsbBasicPersonalityTestResult& aTestResult );

	void DoExecuteBranchTestL( TPtrC aBranchTestName, TUsbBasicPersonalityTestResult& aTestResult );
	void ExampleBranchTestL( TUsbBasicPersonalityTestResult& aTestResult );	

    TInt CreateAndDeleteTest( TUsbBasicPersonalityTestResult& aTestResult );
    TInt StateChangeNotifyTest( TUsbBasicPersonalityTestResult& aTestResult );
    TInt IsActiveTest( TUsbBasicPersonalityTestResult& aTestResult );
    TInt IsAddedTest( TUsbBasicPersonalityTestResult& aTestResult );
    TInt PriorityTest( TUsbBasicPersonalityTestResult& aTestResult );
    TInt ConfirmPersonalityUnloadTest( TUsbBasicPersonalityTestResult& aTestResult );
    TInt PreparePersonalityStartTest( TUsbBasicPersonalityTestResult& aTestResult );
    TInt ShowUsbConnectionNoteTest( TUsbBasicPersonalityTestResult& aTestResult ); //TODO Check
    TInt FinishPersonalityStartTest( TUsbBasicPersonalityTestResult& aTestResult );
    TInt FinishPersonalityStopTest( TUsbBasicPersonalityTestResult& aTestResult );
    TInt PreparePersonalityStopTest( TUsbBasicPersonalityTestResult& aTestResult );
    TInt SetPriorityTest( TUsbBasicPersonalityTestResult& aTestResult );


    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        // ?one_line_short_description_of_data
        //?data_declaration;

		CActiveScheduler* iScheduler;
		TRequestStatus iStatus;
		
		RUsb iUsb;
		CUsbPersonalityNotifier *iNotifier;
		TUsbPersonalityParams *iParams;
		CUsbPersonalityPlugin *iPlugin;
		
        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // USBBASICPERSONALITYTEST_H

// End of File
