/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/




#ifndef USBUINOTIFAPITEST_H
#define USBUINOTIFAPITEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

#include <usbuinotif.h>
#include <usbpersonalityids.h>
#include <centralrepository.h>
#include <UsbWatcherInternalCRKeys.h>
#include <usbman.h>
#include <usbwatcher.h>
#include <usb/hostms/srverr.h> 
// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
//_LIT( KUsbUiNotifApiTestLogPath, "\\logs\\testframework\\UsbUiNotifApiTest\\" );

// Logging path for ATS - for phone builds comment this line
_LIT( KUsbUiNotifApiTestLogPath, "e:\\testing\\stiflogs\\" ); 

// Log file
_LIT( KUsbUiNotifApiTestLogFile, "UsbUiNotifApiTest.txt" ); 
_LIT( KUsbUiNotifApiTestLogFileWithTitle, "UsbUiNotifApiTest_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CUsbUiNotifApiTest;

// DATA TYPES
//enum ?declaration

enum TUsbUiNotifApiTestResult
    {
    ETestCasePassed,
    ETestCaseFailed
    };

enum TTestOption
    {
    EQueryDiscarded = 0,
    EQueryAccepted,
    EQueryCanceled
    };
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

NONSHARABLE_CLASS( TUsbUiNotifApiTestBlockParams )
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
*  CUsbUiNotifApiTest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS( CUsbUiNotifApiTest ) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CUsbUiNotifApiTest* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CUsbUiNotifApiTest();

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
        CUsbUiNotifApiTest( CTestModuleIf& aTestModuleIf );

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
        
    	void DoExecuteApiTestBlockL( CStifItemParser& aItem, TUsbUiNotifApiTestResult& aTestResult );    	
    	void DoExecuteModuleTestBlockL( CStifItemParser& aItem, TUsbUiNotifApiTestResult& aTestResult );    
    	void DoExecuteBranchTestBlockL( CStifItemParser& aItem, TUsbUiNotifApiTestResult& aTestResult );
    	
        void ExampleTestL( TPtrC aTestOption, TPtrC aTestSubOption, 
                 TInt aTestIntOption, TInt aTestCharOption, TUsbUiNotifApiTestResult& aTestResult );

        void CableConnectedNotifierTest( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult );
        void ConnectionNotifierTest( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult );
        void UsbQueriesNotifierTest( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult );
        void UsbOTGErrorNotifierTests( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult );
        void UsbOTGWarningNotifierTests( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult );
        TInt FinishQuery( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult );
        void LoadNotifiersL( TUsbUiNotifApiTestResult& aTestResult );
        void UnLoadNotifiers ( TUsbUiNotifApiTestResult& aTestResult );
        void Update( TUsbUiNotifApiTestResult& aTestResult );
        //void Cancelnotifier( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult ); 
        void Notifierstart( TUsbUiNotifApiTestResult& aTestResult );
        //void AsyncConnectionNotifier ( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult );
        void UsbMSMMNotifierTests ( TPtrC aTestSubOption, TUsbUiNotifApiTestResult& aTestResult );

        TInt GetTestOption( TPtrC aOptionString, TTestOption& aOption );
        TInt GetPersonalityIdFromString( TPtrC aOptionString, TInt& aPersonalityId );
        TInt GetQueryType( TPtrC aTypeString, TUSBUIQueries& aQueryType );        
        TInt GetOTGErrorType( TPtrC aTypeString, TUsbUiNotifOtgError& aQueryType );
        TInt GetOTGWarningType( TPtrC aTypeString, TUsbUiNotifOtgWarning& aQueryType );
        TInt GetMSMMrrorType( TPtrC aTypeString, THostMsErrCode& aQueryType );

        inline void Trace(TRefByValue<const TDesC8> aFmt, ...);
        inline void Trace(TRefByValue<const TDesC16> aFmt, ...);
	
    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        TUsbUiNotifApiTestBlockParams iTestBlockParams;

        RNotifier *iNotifier;
        RUsbWatcher* iUsbWatcher;
 
        CRepository* iRepository;
        
        TUSBConnectionNotifierParamsPckg iConnectionNotifierResponseParamsPckg;
        TRequestStatus iReqStatus;
        TInt iPersonalityIdBackup;
        TBool iCompleteQuery;        
        TPckg<TInt> iRes;
        CArrayPtr<MEikSrvNotifierBase2>* iNotifierArray;
        TUid iInstanceUid;
        THostMsErrData iErrData;
        THostMsErrorDataPckg iErrPckg ;

        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;
        
    };

#include "UsbUiNotifApiTestDebug.inl"

#endif      // USBUINOTIFAPITEST_H

// End of File
