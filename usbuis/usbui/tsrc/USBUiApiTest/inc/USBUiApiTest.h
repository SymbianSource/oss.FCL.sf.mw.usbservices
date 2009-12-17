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

#ifdef STIF_LOG
#undef STIF_LOG
#endif

#define STIF_LOG( s )\
    {\
    TBuf<KMaxLogData> traceBuf;\
    traceBuf.Append( _L( "[STIF_LOG] " ) );\
    traceBuf.Append( _L( s ) );\
    iLog->Log( _L( s ) );\
    RDebug::Print( traceBuf );\
    }

#define STIF_LOG1( s, v ) \
    {\
    TBuf<KMaxLogData> traceBuf;\
    traceBuf.Append( _L( "[STIF_LOG] " ) );\
    traceBuf.Append( _L( s ) );\
    iLog->Log( _L( s ), v );\
    RDebug::Print( traceBuf, v );\
    }

#define STIF_LOG2( s, v1, v2 ) \
    {\
    TBuf<KMaxLogData> traceBuf;\
    traceBuf.Append( _L( "[STIF_LOG] " ) );\
    traceBuf.Append( _L( s ) );\
    iLog->Log( _L( s ), v1, v2 );\
    RDebug::Print( traceBuf, v1, v2 );\
    }

#define STIF_LOG3( s, v1, v2, v3 ) \
    {\
    TBuf<KMaxLogData> traceBuf;\
    traceBuf.Append( _L( "[STIF_LOG] " ) );\
    traceBuf.Append( _L( s ) );\
    iLog->Log( _L( s ), v1, v2, v3 );\
    RDebug::Print( traceBuf, v1, v2, v3 );\
    }


// Logging path
#ifndef ATS
_LIT( KUSBUiApiTestLogPath, "\\logs\\testframework\\usb\\" );
#else
_LIT( KUSBUiApiTestLogPath, "e:\\testing\\stiflogs\\" ); 
#endif
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
enum TUSBAppTestOption
    {
    EAppCancelled = 0,
    EAskOnConnectionSettingChanged,//1
    EUsbConnectionModeSettingChanged//2
    };
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CUSBUiApiTest test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CUSBUiApiTest) : public CScriptBase
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

        virtual TInt LaunchUsbUiApp( CStifItemParser& aItem );
        virtual TInt FinishTest( CStifItemParser& aItem );
        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove
        TInt GetTestOption( TPtrC aOptionString, TUSBAppTestOption& aOption );
        TInt GetAskOnConnectionOptionFromString( TPtrC aOptionString, TInt& aOption );
        TInt GetUsbPersonalityOptionFromString( TPtrC aOptionString, TInt& aOption );
        TBool IsUsbAppRunning();
        TInt GetUsbSettings( TInt& aUsbPersinalityId, TInt& aAskOnConnectionSetting );
        TInt SetUsbSettings( TInt aUsbPersinalityId, TInt aAskOnConnectionSetting );

        TInt TurnLightsOn( CStifItemParser& aItem );   

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
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

#endif      // USBUIAPITEST_H

// End of File
