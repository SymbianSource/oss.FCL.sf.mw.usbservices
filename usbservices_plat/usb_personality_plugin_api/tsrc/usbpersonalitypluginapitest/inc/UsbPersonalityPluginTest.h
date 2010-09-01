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
#ifndef USBPERSONALITYPLUGINTEST_H
#define USBPERSONALITYPLUGINTEST_H
//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <usbman.h>
#include <cusbpersonalitynotifier.h>
#include <cusbpersonalityplugin.h>
#include <tusbpersonalityparams.h>
#include <usbuinotif.h>
#include "UsbPersonalityPluginActiveObject.h"

// CONSTANTS
// Logging path
_LIT( KUsbPersonalityPluginTestLogPath, "\\logs\\testframework\\UsbPersonalityPluginTest\\" );
// Logging path for ATS - for phone builds comment this line
//_LIT( KUsbPersonalityPluginTestLogPath, "e:\\testing\\stiflogs\\" ); 
// Log file
_LIT( KUsbPersonalityPluginTestLogFile, "UsbPersonalityPluginTest.txt" );
_LIT( KUsbPersonalityPluginTestLogFileWithTitle, "UsbPersonalityPluginTest_[%S].txt" );

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
enum TUsbPersonalityPluginTestResult
    {
    ETestCasePassed, ETestCaseFailed
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
    eGetString, eGetInt, eGetChar
    };

enum TUsbPersonalityIds
    {
    EMassStorage = 2, EMediaTransfer = 4, EPCSuite = 5
    };
// CLASS DECLARATION
/**
 *  TUsbPersonalityPluginTestBlockParams test class for storing test block parameters.
 */
NONSHARABLE_CLASS( TUsbPersonalityPluginTestBlockParams )
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
 *  CUsbPersonalityPluginTest test class for STIF Test Framework TestScripter.
 */
NONSHARABLE_CLASS( CUsbPersonalityPluginTest ) : public CScriptBase
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CUsbPersonalityPluginTest* NewL(CTestModuleIf& aTestModuleIf);

    /**
     * Destructor.
     */
    virtual ~CUsbPersonalityPluginTest();

public:
    // New functions
public:
    // Functions from base classes

    /**
     * From CScriptBase Runs a script line.
     * @since ?Series60_version
     * @param aItem Script line containing method name and parameters
     * @return Symbian OS error code
     */
    virtual TInt RunMethodL(CStifItemParser& aItem);

protected:
    // New functions
protected:
    // Functions from base classes
private:

    /**
     * C++ default constructor.
     */
    CUsbPersonalityPluginTest(CTestModuleIf& aTestModuleIf);

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
    TInt ExecuteTestBlock(CStifItemParser& aItem);

    /**
     * Executes proper get parameter function indicated by its arguments
     */
    void GetTestBlockParamL(TGetTestBlockParamOperation aOperation,
            TTestBlockParamName aParamName);

    /**
     * Method used to log version of test class
     */
    void SendTestClassVersion();

    // ADD NEW METHOD DEC HERE
    // [TestMethods] - Do not remove 
    void ExampleTestL(TUsbPersonalityPluginTestBlockParams& aParams,
            TUsbPersonalityPluginTestResult& aTestResult);
    void InitializationTestL(TUsbPersonalityPluginTestBlockParams& aParams,
            TUsbPersonalityPluginTestResult& aTestResult);
    void SetPersonalityTestL(TUsbPersonalityPluginTestBlockParams& aParams,
            TUsbPersonalityPluginTestResult& aTestResult);
    void ShowNoteTestL(TUsbPersonalityPluginTestBlockParams& aParams,
            TUsbPersonalityPluginTestResult& aTestResult);
    void ShowQueryTestL(TUsbPersonalityPluginTestBlockParams& aParams,
            TUsbPersonalityPluginTestResult& aTestResult);
    void ShowConnectionNoteTestL(
            TUsbPersonalityPluginTestBlockParams& aParams,
            TUsbPersonalityPluginTestResult& aTestResult);
    void CreatePersonalityPluginTestL(
            TUsbPersonalityPluginTestBlockParams& aParams,
            TUsbPersonalityPluginTestResult& aTestResult);
    void PreparePersonalityStartTestL(
            TUsbPersonalityPluginTestBlockParams& aParams,
            TUsbPersonalityPluginTestResult& aTestResult);
    void FinishPersonalityStartTestL(
            TUsbPersonalityPluginTestBlockParams& aParams,
            TUsbPersonalityPluginTestResult& aTestResult);
    void PreparePersonalityStopTestL(
            TUsbPersonalityPluginTestBlockParams& aParams,
            TUsbPersonalityPluginTestResult& aTestResult);
    void FinishPersonalityStopTestL(
            TUsbPersonalityPluginTestBlockParams& aParams,
            TUsbPersonalityPluginTestResult& aTestResult);
    
    TInt GetIdFromString(TPtrC aBuffer);
    TUid GetUidFromId(TInt aId);
    
    void InitializeTestObjectsL();
    void DeleteTestObjects();  
    
public:
    // Data
protected:
    // Data
private:
    // Data

    // Member object used to store test block parameters
    TUsbPersonalityPluginTestBlockParams iTestBlockParams;

    // Used to indicate if test block with specified parameters exist
    TBool iTestBlockFound;

    // Function pointer used to call proper test block methods
    void (CUsbPersonalityPluginTest::*iTestBlockFunction)(
            TUsbPersonalityPluginTestBlockParams&,
            TUsbPersonalityPluginTestResult&);

    // Not own
    CStifItemParser* iItem;

    RUsb* iUsbMan;
    CUsbPersonalityNotifier* iNotifier;
    TUsbPersonalityParams* iParams;
    CUsbPersonalityPlugin* iPlugin;
    CUsbPersonalityPluginActiveObject* iActive;
    TRequestStatus iRequestStatus;
    // Reserved pointer for future extension
    //TAny* iReserved;

    // ADD NEW DATA DEC HERE


public:
    // Friend classes
protected:
    // Friend classes
private:
    // Friend classes

    };
#endif      // USBPERSONALITYPLUGINTEST_H
// End of File
