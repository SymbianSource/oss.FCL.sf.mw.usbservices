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




#ifndef USBOBEXCLASSCONTROLLERTEST_H
#define USBOBEXCLASSCONTROLLERTEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

#include "USBObexApiTestActiveObject.h"
#include <musbclasscontrollernotify.h>
#include <cusbclasscontrollerplugin.h>
#include <usbobex.h>
#include <usb_std.h>
#include <ecom/ecom.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KUsbObexClassControllerTestLogPath, "\\logs\\testframework\\UsbObexClassControllerTest\\" );

// Logging path for ATS - for phone builds comment this line
//_LIT( KUsbObexClassControllerTestLogPath, "e:\\testing\\stiflogs\\" ); 

// Log file
_LIT( KUsbObexClassControllerTestLogFile, "UsbObexClassControllerTest.txt" ); 
_LIT( KUsbObexClassControllerTestLogFileWithTitle, "UsbObexClassControllerTest_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CUsbObexClassControllerTest;
class CUsbClassControllerIterator;              

// DATA TYPES
//enum ?declaration

enum TUsbObexClassControllerTestResult
    {
    ETestCasePassed,
    ETestCaseFailed
    };

//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CUsbObexClassControllerTest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CUsbObexClassControllerTest) : 
										public CScriptBase,
										public MUsbClassControllerNotify
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CUsbObexClassControllerTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CUsbObexClassControllerTest();

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
        
        /**
        * overrider of pure virtual functions from MUsbClassControllerNotify class
        */ 
        CUsbClassControllerIterator* UccnGetClassControllerIteratorL();
        void UccnError(TInt aError);

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
        CUsbObexClassControllerTest( CTestModuleIf& aTestModuleIf );

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

		void DoExecuteApiTestL( TPtrC aApiTestName, TUsbObexClassControllerTestResult& aTestResult );
	
		void ExampleApiTestL( TUsbObexClassControllerTestResult& aTestResult );
		
		void DoExecuteModuleTestL( TPtrC aModuleTestName, TUsbObexClassControllerTestResult& aTestResult );
	
		void ExampleModuleTestL( TUsbObexClassControllerTestResult& aTestResult );
	
		void DoExecuteBranchTestL( TPtrC aBranchTestName, TUsbObexClassControllerTestResult& aTestResult );
	
		void ExampleBranchTestL( TUsbObexClassControllerTestResult& aTestResult );
		
		void InitializeTestObjectL();
		void DeleteTestObject();
		TInt InitializationTestL( TUsbObexClassControllerTestResult& aTestResult );
		TInt StartTestL( TUsbObexClassControllerTestResult& aTestResult );
		TInt StopTestL( TUsbObexClassControllerTestResult& aTestResult );
		TInt InterfaceInfoTestL( TUsbObexClassControllerTestResult& aTestResult );
		TInt InterfaceLengthInfoTestL( TUsbObexClassControllerTestResult& aTestResult );
		TInt NumberOfInterfacesInfoTestL( TUsbObexClassControllerTestResult& aTestResult );
		TInt StartupPriorityInfoTestL( TUsbObexClassControllerTestResult& aTestResult );
		TInt StateInfoTestL( TUsbObexClassControllerTestResult& aTestResult );
		TInt OwnerTestL( TUsbObexClassControllerTestResult& aTestResult );
		TInt CompareTestL( TUsbObexClassControllerTestResult& aTestResult );

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        // ?one_line_short_description_of_data
        //?data_declaration;

        CUsbClassControllerPlugIn* iUsbClassControllerPlugIn;
        CActiveScheduler* iScheduler;
        TRequestStatus iRequestStatus;
        TUsbServiceState iUsbServiceState;

        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // USBOBEXCLASSCONTROLLERTEST_H

// End of File
