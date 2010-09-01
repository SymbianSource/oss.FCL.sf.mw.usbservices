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

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "UsbMscPersonalityTest.h"
#include "testdebug.h"
#include <ecom/ecom.h>
// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUsbMscPersonalityTest::TestConstructL
// Construct here all test resources for test methods. 
// Called from ConstructL. 
// -----------------------------------------------------------------------------

void CUsbMscPersonalityTest::TestConstructL()
    {
    TRACE_FUNC_ENTRY

    if (!CActiveScheduler::Current())
        {
        iScheduler = new (ELeave) CActiveScheduler;
        CActiveScheduler::Install(iScheduler);
        }

    iUsb = new (ELeave) RUsb;
    iNotifier = CUsbPersonalityNotifier::NewL();
    iParams = new (ELeave) TUsbPersonalityParams(*iUsb, *iNotifier);
    //    iParams->SetPersonalityId(2);
    iPlugin = CUsbPersonalityPlugin::NewL(*iParams, TUid::Uid(0x1020DF7B));
    
    TRACE_FUNC_EXIT
    }

// -----------------------------------------------------------------------------
// CUsbMscPersonalityTest::TestDelete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------

void CUsbMscPersonalityTest::TestDelete()
    {
    TRACE_FUNC_ENTRY

    if (iScheduler)
        {
        iScheduler->Stop();
        delete iScheduler;
        iScheduler = NULL;
        }

    if (iUsb)
        {
        delete iUsb;
        iUsb = NULL;
        }

    if (iNotifier)
        {
        delete iNotifier;
        }

    if (iParams)
        {
        delete iParams;
        iParams = NULL;
        }

    if (iPlugin)
        {
        iPlugin->Cancel();
        delete iPlugin;
        iPlugin = NULL;
        }

    if (iActive)
        {
        delete iActive;
        iActive = NULL;
        }
    
    if (iDismountClient)
        {
        delete iDismountClient;
        iDismountClient = NULL;
        }
    
    REComSession::FinalClose();

    TRACE_FUNC_EXIT
    }

// -----------------------------------------------------------------------------
// CUsbMscPersonalityTest::TestBlocksInfoL
// Add new test block parameter entries below
// -----------------------------------------------------------------------------

void CUsbMscPersonalityTest::TestBlocksInfoL()
    {

    TESTENTRY( "ExampleTestL", CUsbMscPersonalityTest::ExampleTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        TESTPARAM( eGetString, eTestOption2 )
        TESTPARAM( eGetInt, eTestIntOption1 )
        TESTPARAM( eGetChar, eTestCharOption1 )
        }
    TESTENTRY( "PreparePersonalityStartTestL", CUsbMscPersonalityTest::PreparePersonalityStartTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "FinishPersonalityStartTestL", CUsbMscPersonalityTest::FinishPersonalityStartTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "PreparePersonalityStopTestL", CUsbMscPersonalityTest::PreparePersonalityStopTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "FinishPersonalityStopTestL", CUsbMscPersonalityTest::FinishPersonalityStopTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "InitializationTestL", CUsbMscPersonalityTest::InitializationTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "StateChangeNotifyTestL", CUsbMscPersonalityTest::StateChangeNotifyTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "StateChangeNotifyUndefinedTestL", CUsbMscPersonalityTest::StateChangeNotifyUndefinedTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "StateChangeNotifyStateAddressTestL", CUsbMscPersonalityTest::StateChangeNotifyStateAddressTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "ConfirmPersonalityUnloadTestL", CUsbMscPersonalityTest::ConfirmPersonalityUnloadTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "AllowDismountAfter2SecTestL", CUsbMscPersonalityTest::AllowDismountAfter2SecTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "AllowDismountTestL", CUsbMscPersonalityTest::AllowDismountTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "NotAllowDismountTestL", CUsbMscPersonalityTest::NotAllowDismountTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "AllowDismountTooLateTestL", CUsbMscPersonalityTest::AllowDismountTooLateTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }

    }

// Add test block methods implementation here
// -----------------------------------------------------------------------------
// CUsbMscPersonalityTest::ExampleTestL
// -----------------------------------------------------------------------------

void CUsbMscPersonalityTest::ExampleTestL(
        TUsbMscPersonalityTestBlockParams& aParams,
        TUsbMscPersonalityTestResult& aTestResult)
    {
    TRACE_FUNC_ENTRY

    if (!aParams.iTestOption1.Compare(_L( "API" )))
        {
        TRACE_INFO( "Api test option: %S", &aParams.iTestOption1 )
        TRACE_INFO( "Api test sub-option: %S", &aParams.iTestOption2 )
        TRACE_INFO( "Api test int option: %d", aParams.iTestIntOption1 )
        TRACE_INFO( "Api test char option: %c", TText( aParams.iTestCharOption1 ) )
        }
    else if (!aParams.iTestOption1.Compare(_L( "MODULE" )))
        {
        TRACE_INFO( "Module test option: %S", &aParams.iTestOption1 )
        TRACE_INFO( "Module test sub-option: %S", &aParams.iTestOption2 )
        TRACE_INFO( "Module test int option: %d", aParams.iTestIntOption1 )
        TRACE_INFO( "Module test char option: %c", TText( aParams.iTestCharOption1 ) )
        }
    else if (!aParams.iTestOption1.Compare(_L( "BRANCH" )))
        {
        TRACE_INFO( "Branch test option: %S", &aParams.iTestOption1 )
        TRACE_INFO( "Branch test sub-option: %S", &aParams.iTestOption2 )
        TRACE_INFO( "Branch test int option: %d", aParams.iTestIntOption1 )
        TRACE_INFO( "Branch test char option: %c", TText( aParams.iTestCharOption1 ) )
        }
    else
        {
        TRACE( "Invalid test parameter" )
        TRACE_FUNC_EXIT
        User::Leave(KErrNotFound);
        }

    aTestResult = ETestCasePassed;
    TRACE_FUNC_EXIT
    }

void CUsbMscPersonalityTest::InitializeTestObjectsL()
    {
    iActive
            = CUsbMscPersonalityTestActiveObject::NewL(iPlugin, iLog, iParams);
    iDismountClient = CDismountClient::NewL(iLog);
    TRACE ( " test objects initialized " );
    }

void CUsbMscPersonalityTest::DeleteTestObjects()
    {
    delete iActive;
    iActive = NULL;
    delete iDismountClient;
    iDismountClient = NULL;
    TRACE ( " test objects deleted " );
    }

void CUsbMscPersonalityTest::InitializationTestL(
        TUsbMscPersonalityTestBlockParams& aParams,
        TUsbMscPersonalityTestResult& aTestResult)
    {
    InitializeTestObjectsL();
    TRACE( "checking iPlugin")
    if (iPlugin)
        {
        aTestResult = ETestCasePassed;
        }
    else
        {
        TRACE( "no iplugin instance")
        }

    DeleteTestObjects();
    }

void CUsbMscPersonalityTest::ExecuteDismounting()
    {
    iActive->PreparePersonalityStart(iRequestStatus);
    iActive->FinishPersonalityStart(iRequestStatus);
    iActive->PreparePersonalityStop(iRequestStatus);
    iActive->FinishPersonalityStop(iRequestStatus);
    }

void CUsbMscPersonalityTest::PreparePersonalityStartTestL(
        TUsbMscPersonalityTestBlockParams& aParams,
        TUsbMscPersonalityTestResult& aTestResult)
    {
    InitializeTestObjectsL();

    iActive->PreparePersonalityStart(iRequestStatus);

    TRACE_INFO( "prepare personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError(iRequestStatus.Int());
    
    aTestResult = ETestCasePassed;
    DeleteTestObjects();
    }

void CUsbMscPersonalityTest::FinishPersonalityStartTestL(
        TUsbMscPersonalityTestBlockParams& aParams,
        TUsbMscPersonalityTestResult& aTestResult)
    {
    InitializeTestObjectsL();

    iActive->PreparePersonalityStart(iRequestStatus);
    TRACE_INFO( "prepare personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError(iRequestStatus.Int());

    iActive->FinishPersonalityStart(iRequestStatus);
    TRACE_INFO( "finish personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );

    iActive->PreparePersonalityStop(iRequestStatus);
    iActive->FinishPersonalityStart(iRequestStatus);
    User::LeaveIfError(iRequestStatus.Int());

    aTestResult = ETestCasePassed;

    DeleteTestObjects();
    }

void CUsbMscPersonalityTest::PreparePersonalityStopTestL(
        TUsbMscPersonalityTestBlockParams& aParams,
        TUsbMscPersonalityTestResult& aTestResult)
    {
    InitializeTestObjectsL();

    iActive->PreparePersonalityStart(iRequestStatus);
    TRACE_INFO( "prepare personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError( iRequestStatus.Int() );

    iActive->FinishPersonalityStart(iRequestStatus);
    TRACE_INFO( "finish personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError( iRequestStatus.Int() );

    iActive->PreparePersonalityStop(iRequestStatus);
    TRACE_INFO( "prepare personality stop status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError( iRequestStatus.Int() );
        
    aTestResult = ETestCasePassed;

    DeleteTestObjects();
    }

void CUsbMscPersonalityTest::FinishPersonalityStopTestL(
        TUsbMscPersonalityTestBlockParams& aParams,
        TUsbMscPersonalityTestResult& aTestResult)
    {
    InitializeTestObjectsL();

    iActive->PreparePersonalityStart(iRequestStatus);
    TRACE_INFO( "prepare personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError( iRequestStatus.Int() );

    iActive->FinishPersonalityStart(iRequestStatus);
    TRACE_INFO( "finish personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError( iRequestStatus.Int() );

    iActive->PreparePersonalityStop(iRequestStatus);
    TRACE_INFO( "prepare personality stop status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError( iRequestStatus.Int() );

    iActive->FinishPersonalityStop(iRequestStatus);
    TRACE_INFO( "finish personality stop status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError( iRequestStatus.Int() );
        
    aTestResult = ETestCasePassed;

    DeleteTestObjects();
    }

void CUsbMscPersonalityTest::ConfirmPersonalityUnloadTestL(
        TUsbMscPersonalityTestBlockParams& aParams,
        TUsbMscPersonalityTestResult& aTestResult)
    {
    InitializeTestObjectsL();

    TRACE_INFO( "prepare personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError( iRequestStatus.Int() );

    iActive->FinishPersonalityStart(iRequestStatus);
    TRACE_INFO( "finish personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError( iRequestStatus.Int() );

    iActive->ConfirmPersonalityUnload(iRequestStatus);
    User::After(5000);
    TRACE_INFO( "confirm personality unload status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError( iRequestStatus.Int() );
        
    aTestResult = ETestCasePassed;

    DeleteTestObjects();
    }

void CUsbMscPersonalityTest::StateChangeNotifyTestL(
        TUsbMscPersonalityTestBlockParams& aParams,
        TUsbMscPersonalityTestResult& aTestResult)
    {
    InitializeTestObjectsL();

    TUsbDeviceState state = EUsbDeviceStateDefault;
    iPlugin->StateChangeNotify(state);

    aTestResult = ETestCasePassed;

    DeleteTestObjects();
    }

void CUsbMscPersonalityTest::StateChangeNotifyUndefinedTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult )
    {
    InitializeTestObjectsL();

    TUsbDeviceState state = EUsbDeviceStateUndefined;
    iPlugin->StateChangeNotify(state);

    aTestResult = ETestCasePassed;

    DeleteTestObjects();
    }

void CUsbMscPersonalityTest::StateChangeNotifyStateAddressTestL( TUsbMscPersonalityTestBlockParams& aParams, 
                TUsbMscPersonalityTestResult& aTestResult )
    {
    InitializeTestObjectsL();

    TUsbDeviceState state = EUsbDeviceStateAddress;
    iPlugin->StateChangeNotify(state);

    User::After( 2000000 );
    iPlugin->Cancel();
    
    aTestResult = ETestCasePassed;

    DeleteTestObjects();
    }

void CUsbMscPersonalityTest::AllowDismountAfter2SecTestL(
        TUsbMscPersonalityTestBlockParams& aParams,
        TUsbMscPersonalityTestResult& aTestResult)
    {
    InitializeTestObjectsL();
    iDismountClient->StartL(EAllowDismountAfter2Sec);
    iActive->PreparePersonalityStart(iRequestStatus);
    ExecuteDismounting();

    aTestResult = ETestCasePassed;
    DeleteTestObjects();
    }

void CUsbMscPersonalityTest::AllowDismountTestL(
        TUsbMscPersonalityTestBlockParams& aParams,
        TUsbMscPersonalityTestResult& aTestResult)
    {
    InitializeTestObjectsL();
    iDismountClient->StartL(EAllowDismount);
    iActive->PreparePersonalityStart(iRequestStatus);
    ExecuteDismounting();

    aTestResult = ETestCasePassed;
    DeleteTestObjects();
    }

void CUsbMscPersonalityTest::NotAllowDismountTestL(
        TUsbMscPersonalityTestBlockParams& aParams,
        TUsbMscPersonalityTestResult& aTestResult)
    {
    InitializeTestObjectsL();
    iDismountClient->StartL(ENotAllowDismount);
    iActive->PreparePersonalityStart(iRequestStatus);
    ExecuteDismounting();

    aTestResult = ETestCasePassed;
    DeleteTestObjects();
    }

void CUsbMscPersonalityTest::AllowDismountTooLateTestL(
        TUsbMscPersonalityTestBlockParams& aParams,
        TUsbMscPersonalityTestResult& aTestResult)
    {
    InitializeTestObjectsL();
    iDismountClient->StartL(EAllowDismountTooLate);
    iActive->PreparePersonalityStart(iRequestStatus);
    ExecuteDismounting();

    aTestResult = ETestCasePassed;
    DeleteTestObjects();
    }
// Add other member functions implementation here


// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
