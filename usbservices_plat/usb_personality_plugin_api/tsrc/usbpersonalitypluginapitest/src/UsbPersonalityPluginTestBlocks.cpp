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
#include "UsbPersonalityPluginTest.h"
#include "testdebug.h"
#include <ecom/ecom.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

const TInt KIdPCSuite = 5;
const TInt KIdMediaTransfer = 4;
const TInt KIdMassStorage = 2;

const TUid KUidPCSuite =
    {
    0x102068DF
    };
const TUid KUidMediaTransfer =
    {
    0x10282C70
    };
//const TUid KUidPCSuite2 =
//    {
//    0x2000B5D2
//    };
//const TUid KUidPhoneAsModem =
//    {
//    0x20029E41
//    };
const TUid KUidMassStorage =
    {
    0x1020DF7B
    };

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
// CUsbPersonalityPluginTest::TestConstructL
// Construct here all test resources for test methods. 
// Called from ConstructL. 
// -----------------------------------------------------------------------------

void CUsbPersonalityPluginTest::TestConstructL()
    {
    TRACE_FUNC_ENTRY

    iUsbMan = new (ELeave) RUsb;
    iUsbMan->Connect();
    iNotifier = CUsbPersonalityNotifier::NewL();
    iParams = new (ELeave) TUsbPersonalityParams(*iUsbMan, *iNotifier);

    TRACE_FUNC_EXIT
    }

// -----------------------------------------------------------------------------
// CUsbPersonalityPluginTest::TestDelete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------

void CUsbPersonalityPluginTest::TestDelete()
    {
    TRACE_FUNC_ENTRY

    if (iUsbMan)
        {
        iUsbMan->Close();
        delete iUsbMan;
        iUsbMan = NULL;
        }
    if (iNotifier)
        {
        delete iNotifier;
        iNotifier = NULL;
        }
    if (iParams)
        {
        delete iParams;
        iParams = NULL;
        }
    if (iPlugin)
        {
        delete iPlugin;
        iPlugin = NULL;
        }

    REComSession::FinalClose();

    TRACE_FUNC_EXIT
    }

// -----------------------------------------------------------------------------
// CUsbPersonalityPluginTest::TestBlocksInfoL
// Add new test block parameter entries below
// -----------------------------------------------------------------------------

void CUsbPersonalityPluginTest::TestBlocksInfoL()
    {

    TESTENTRY( "InitializationTestL", CUsbPersonalityPluginTest::InitializationTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "SetPersonalityTestL", CUsbPersonalityPluginTest::SetPersonalityTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        TESTPARAM( eGetString, eTestOption2 )
        }
    TESTENTRY( "ShowQueryTestL", CUsbPersonalityPluginTest::ShowQueryTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        }
    TESTENTRY( "ShowConnectionNoteTestL", CUsbPersonalityPluginTest::ShowConnectionNoteTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        TESTPARAM( eGetString, eTestOption2 )
        }
    TESTENTRY( "CreatePersonalityPluginTestL", CUsbPersonalityPluginTest::CreatePersonalityPluginTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        TESTPARAM( eGetString, eTestOption2 )
        }
    TESTENTRY( "PreparePersonalityStartTestL", CUsbPersonalityPluginTest::PreparePersonalityStartTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        TESTPARAM( eGetString, eTestOption2 )
        }
    TESTENTRY( "FinishPersonalityStartTestL", CUsbPersonalityPluginTest::FinishPersonalityStartTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        TESTPARAM( eGetString, eTestOption2 )
        }
    TESTENTRY( "PreparePersonalityStopTestL", CUsbPersonalityPluginTest::PreparePersonalityStopTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        TESTPARAM( eGetString, eTestOption2 )
        }
    TESTENTRY( "FinishPersonalityStopTestL", CUsbPersonalityPluginTest::FinishPersonalityStopTestL )
        {
        TESTPARAM( eGetString, eTestOption1 )
        TESTPARAM( eGetString, eTestOption2 )
        }
    }

// Add test block methods implementation here
// -----------------------------------------------------------------------------
// CUsbPersonalityPluginTest::ExampleTestL
// -----------------------------------------------------------------------------

void CUsbPersonalityPluginTest::ExampleTestL(
        TUsbPersonalityPluginTestBlockParams& aParams,
        TUsbPersonalityPluginTestResult& aTestResult)
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

// Add other member functions implementation here
void CUsbPersonalityPluginTest::InitializeTestObjectsL()
    {
    iActive = CUsbPersonalityPluginActiveObject::NewL(iPlugin, iLog, iParams);
    TRACE ( " test objects initialized " );
    }

void CUsbPersonalityPluginTest::DeleteTestObjects()
    {
    delete iActive;
    iActive = NULL;
    }

void CUsbPersonalityPluginTest::InitializationTestL(
        TUsbPersonalityPluginTestBlockParams& aParams,
        TUsbPersonalityPluginTestResult& aTestResult)
    {
    TRACE_FUNC_ENTRY
    aTestResult = ETestCaseFailed;

    if (iNotifier)
        {
        aTestResult = ETestCasePassed;
        }

    TRACE_FUNC_EXIT
    }

void CUsbPersonalityPluginTest::SetPersonalityTestL(
        TUsbPersonalityPluginTestBlockParams& aParams,
        TUsbPersonalityPluginTestResult& aTestResult)
    {
    TRACE_FUNC_ENTRY
    aTestResult = ETestCaseFailed;
    TInt id = GetIdFromString(aParams.iTestOption2);
    TRACE_INFO( " Set personality ID: %d", id );

    iParams->SetPersonalityId(id);
    if (iParams->PersonalityId() == id)
        {
        aTestResult = ETestCasePassed;
        }

    TRACE_FUNC_EXIT
    }

void CUsbPersonalityPluginTest::ShowQueryTestL(
        TUsbPersonalityPluginTestBlockParams& aParams,
        TUsbPersonalityPluginTestResult& aTestResult)
    {
    TRACE_FUNC_ENTRY
    aTestResult = ETestCaseFailed;

    TBuf8<8> iDummyBuf;

    TUSBQueriesNotifierParamsPckg queryParams;
    queryParams().iQuery = EUSBNotEnoughRam;

    TInt res = iNotifier->ShowQuery(KQueriesNotifier, queryParams, iDummyBuf);
    User::After(3000000);
    TRACE_INFO( " Query show ret %d ", res )
    if (res == KErrNone)
        {
        aTestResult = ETestCasePassed;
        }

    TRACE_FUNC_EXIT
    }

void CUsbPersonalityPluginTest::ShowConnectionNoteTestL(
        TUsbPersonalityPluginTestBlockParams& aParams,
        TUsbPersonalityPluginTestResult& aTestResult)
    {
    TRACE_FUNC_ENTRY
    aTestResult = ETestCaseFailed;

    TBuf8<8> iDummyBuf;

    TUSBConnectionNotifierParamsPckg noteParams;
//    noteParams().iPersonalityId = personalityId;

    TInt res = iNotifier->ShowQuery(KCableConnectedNotifierUid, noteParams,
            iDummyBuf);
    User::After(3000000);
    TRACE_INFO( " Connection note show ret %d ", res )
    if (res == KErrNone)
        {
        aTestResult = ETestCasePassed;
        }

    TRACE_FUNC_EXIT
    }

TInt CUsbPersonalityPluginTest::GetIdFromString(TPtrC aBuffer)
    {

    if (!aBuffer.Compare(_L("MS")))
        {
        TRACE( " mass storage " );
        return KIdMassStorage;
        }
    else if (!aBuffer.Compare(_L("PCS")))
        {
        TRACE( " pc suite " );
        return KIdPCSuite;
        }
    else if (!aBuffer.Compare(_L("MTP")))
        {
        TRACE( " media transfer " );
        return KIdMediaTransfer;
        }
    else
        {
        TRACE( " not found " );
        return 0;
        }
    }

TUid CUsbPersonalityPluginTest::GetUidFromId(TInt aId)
    {
    switch (aId)
        {
        case KIdMassStorage:
            return KUidMassStorage;
        case KIdPCSuite:
            return KUidPCSuite;
        case KIdMediaTransfer:
            return KUidMediaTransfer;
        default:
            return TUid::Uid(0x0);
        }
    }

void CUsbPersonalityPluginTest::CreatePersonalityPluginTestL(
        TUsbPersonalityPluginTestBlockParams& aParams,
        TUsbPersonalityPluginTestResult& aTestResult)
    {
    TRACE_FUNC_ENTRY
    aTestResult = ETestCaseFailed;

    TInt id = GetIdFromString(aParams.iTestOption2);
    TUid uid = GetUidFromId(id);

    iPlugin = CUsbPersonalityPlugin::NewL(*iParams, uid);

    if (iPlugin)
        {
        TRACE( " plugin created ")
        aTestResult = ETestCasePassed;
        }

    TRACE_FUNC_EXIT
    }

void CUsbPersonalityPluginTest::PreparePersonalityStartTestL(
        TUsbPersonalityPluginTestBlockParams& aParams,
        TUsbPersonalityPluginTestResult& aTestResult)
    {
    TRACE_FUNC_ENTRY
    
    aTestResult = ETestCaseFailed;
    
    TInt id = GetIdFromString(aParams.iTestOption2);
    TUid uid = GetUidFromId(id);
    iPlugin = CUsbPersonalityPlugin::NewL(*iParams, uid);

    InitializeTestObjectsL();

    iActive->PreparePersonalityStart(iRequestStatus);

    TRACE_INFO( "prepare personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError(iRequestStatus.Int());

    aTestResult = ETestCasePassed;
    DeleteTestObjects();
    
    TRACE_FUNC_EXIT
    }

void CUsbPersonalityPluginTest::FinishPersonalityStartTestL(
        TUsbPersonalityPluginTestBlockParams& aParams,
        TUsbPersonalityPluginTestResult& aTestResult)
    {
    TRACE_FUNC_ENTRY
    
    aTestResult = ETestCaseFailed;
    
    TInt id = GetIdFromString(aParams.iTestOption2);
    TUid uid = GetUidFromId(id);
    iPlugin = CUsbPersonalityPlugin::NewL(*iParams, uid);

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
    
    TRACE_FUNC_EXIT
    }

void CUsbPersonalityPluginTest::PreparePersonalityStopTestL(
        TUsbPersonalityPluginTestBlockParams& aParams,
        TUsbPersonalityPluginTestResult& aTestResult)
    {
    TRACE_FUNC_ENTRY
    
    aTestResult = ETestCaseFailed;
    
    TInt id = GetIdFromString(aParams.iTestOption2);
    TUid uid = GetUidFromId(id);
    iPlugin = CUsbPersonalityPlugin::NewL(*iParams, uid);

    InitializeTestObjectsL();

    iActive->PreparePersonalityStart(iRequestStatus);
    TRACE_INFO( "prepare personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError(iRequestStatus.Int());

    iActive->FinishPersonalityStart(iRequestStatus);
    TRACE_INFO( "finish personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError(iRequestStatus.Int());

    iActive->PreparePersonalityStop(iRequestStatus);
    TRACE_INFO( "prepare personality stop status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError(iRequestStatus.Int());

    aTestResult = ETestCasePassed;

    DeleteTestObjects();
    
    TRACE_FUNC_EXIT
    }

void CUsbPersonalityPluginTest::FinishPersonalityStopTestL(
        TUsbPersonalityPluginTestBlockParams& aParams,
        TUsbPersonalityPluginTestResult& aTestResult)
    {
    TRACE_FUNC_ENTRY
    
    aTestResult = ETestCaseFailed;
    
    TInt id = GetIdFromString(aParams.iTestOption2);
    TUid uid = GetUidFromId(id);
    iPlugin = CUsbPersonalityPlugin::NewL(*iParams, uid);

    InitializeTestObjectsL();

    iActive->PreparePersonalityStart(iRequestStatus);
    TRACE_INFO( "prepare personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError(iRequestStatus.Int());

    iActive->FinishPersonalityStart(iRequestStatus);
    TRACE_INFO( "finish personality start status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError(iRequestStatus.Int());

    iActive->PreparePersonalityStop(iRequestStatus);
    TRACE_INFO( "prepare personality stop status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError(iRequestStatus.Int());

    iActive->FinishPersonalityStop(iRequestStatus);
    TRACE_INFO( "finish personality stop status: %d, expected: %d", iRequestStatus.Int(), KErrNone );
    User::LeaveIfError(iRequestStatus.Int());

    aTestResult = ETestCasePassed;

    DeleteTestObjects();
    
    TRACE_FUNC_EXIT
    }
// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
