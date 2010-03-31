/*
 * Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  USB UI queries notifier class.
 *
 */

// INCLUDE FILES
#include <eikenv.h>          // Eikon environment
#include <bautils.h>         // BAFL utils (for language file)
#include <utf.h>             // Unicode character conversion utilities
#include <StringLoader.h>    // Localisation stringloader
#include <AknQueryDialog.h> 
#include <aknnotewrappers.h>

#include <usbuinotif.h>                     // pck
#include <usbuinotif.rsg>                   // Own resources
#include "usbuinqueriesnotifiermdrv.h"      // Own class definition
#include "usbuinotifdebug.h"                // Debugging macros
#include <SecondaryDisplay/usbuinotifsecondarydisplay.h>     // Dialog index for cover UI

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CUSBUIQueriesNotifier::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CUSBUIQueriesNotifier* CUSBUIQueriesNotifier::NewL()
    {
        FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::NewL"));
    CUSBUIQueriesNotifier* self = new (ELeave) CUSBUIQueriesNotifier();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CUSBUIQueriesNotifier::CUSBUIQueriesNotifier
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CUSBUIQueriesNotifier::CUSBUIQueriesNotifier()
    {
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CUSBUIQueriesNotifier::~CUSBUIQueriesNotifier()
    {
    //Make sure that the request is completed. Note that inside the destructor,
    //this virtual function call is to local CUSBUIQueriesNotifier::Cancel, 
    //not to any possibly derived class implementation. 
    Cancel();
    }

// ----------------------------------------------------------------------------
// CUSBUIQueriesNotifier::RegisterL
// Register notifier.
// ----------------------------------------------------------------------------
//
CUSBUIQueriesNotifier::TNotifierInfo CUSBUIQueriesNotifier::RegisterL()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::RegisterL"));
    iInfo.iUid = KQueriesNotifier;
    iInfo.iChannel = KQueriesNotifier;
    iInfo.iPriority = ENotifierPriorityVHigh; // must be higher than the one 
        // in cableconnected notifier
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::RegisterL completed"));
    return iInfo;
    }

// ----------------------------------------------------------------------------
// CUSBUIQueriesNotifier::StartL
// Synchronic notifier launch. 
// ----------------------------------------------------------------------------
//
TPtrC8 CUSBUIQueriesNotifier::StartL(const TDesC8& aBuffer)
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::StartL()"));

    TUSBQueriesNotiferParams params; //stores parameters from aBuffef
    TPckgC<TUSBQueriesNotiferParams> pckg( params );
    pckg.Set( aBuffer );
    // Save the type of the query for later use (dialog selection)
    //

    if (pckg().iQuery == EUSBNoMemoryCard)
        {
        TRAPD( err, GetParamsL( aBuffer, 0, iMessage ));
        if (err)
            {
            iNeedToCompleteMessage = EFalse;
            User::Leave( err );
            }
        }

    TPtrC8 ret( KNullDesC8 );
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::StartL() completed"));
    return (ret);
    }

// ----------------------------------------------------------------------------
// CUSBUIQueriesNotifier::GetParamsL
//  Jump to RunL as soon as possible.
// ----------------------------------------------------------------------------
//
void CUSBUIQueriesNotifier::GetParamsL(const TDesC8& aBuffer,
        TInt aReplySlot, const RMessagePtr2& aMessage)
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::GetParamsL"));
    if (iUSBQueryDlg || iReplySlot != 0 || iNeedToCompleteMessage)
        {
        User::Leave( KErrInUse );
        }

    iMessage = aMessage;
    iNeedToCompleteMessage = ETrue;
    iReplySlot = aReplySlot;

    // Get parameters 
    //
    TUSBQueriesNotiferParams params; //stores parameters from aBuffer
    TPckgC<TUSBQueriesNotiferParams> pckg( params );
    pckg.Set( aBuffer );
    // Save the type of the query for later use (dialog selection)
    //
    iQueryType = pckg().iQuery;
    if (iQueryType == EUSBNoMemoryCard)
        {
        iNeedToCompleteMessage = EFalse;
        }
    // Call SetActive() so RunL() will be called by the active scheduler
    //
    SetActive();
    iStatus = KRequestPending;
    TRequestStatus* stat = &iStatus;
    User::RequestComplete( stat, KErrNone );
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::GetParamsL() completed"));
    }

// ----------------------------------------------------------------------------
// CUSBUIQueriesNotifier::RunL
// Ask user response and return it to caller.
// ----------------------------------------------------------------------------
//
void CUSBUIQueriesNotifier::RunL()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::RunL"));

    TBool isCancelKey = EFalse;
    TBool isErrorQuery = EFalse;
    TInt returnValue = KErrNone;
    // for cover display support
    TInt coverDialogId = EUSBCoverInvalidDialogId;

    // Choose text and other query attributes
    //
    HBufC* stringHolder = GetQueryAttributesLC( coverDialogId, isCancelKey, isErrorQuery );

    //check if query text string loading was successful
    if (NULL != stringHolder)
        {
        DisableKeylock();
        SuppressAppSwitching( ETrue );
        returnValue = QueryUserResponseL( *stringHolder, coverDialogId,
                isCancelKey, isErrorQuery );
        SuppressAppSwitching( EFalse );
        RestoreKeylock();
        CleanupStack::PopAndDestroy( stringHolder );
        }
    else
        {
        returnValue = KErrUnknown;
        }

    CompleteMessage( returnValue );
    // cancelling the notifier so that next one on the queue can be displayed.
    // it may be that the client calls cancel too, but it is ok
    iManager->CancelNotifier( iInfo.iUid );
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::RunL() completed"));
    }

// ----------------------------------------------------------------------------
// CUSBUIQueriesNotifier::Cancel
// Release all own resources (member variables)
// ----------------------------------------------------------------------------
//
void CUSBUIQueriesNotifier::Cancel()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::Cancel"));
    if (iUSBQueryDlg)
        {
        delete iUSBQueryDlg;
        iUSBQueryDlg = NULL;
        }
    CompleteMessage( KErrCancel );

    CUSBUINotifierBase::Cancel();
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::Cancel() completed"));
    }

// ----------------------------------------------------------------------------
// CUSBUIQueriesNotifier::QueryUserResponseL
// Show query dialog. 
// ----------------------------------------------------------------------------
//
TInt CUSBUIQueriesNotifier::QueryUserResponseL(const TDesC& aStringHolder,
        TInt aCoverDialogId, TBool aIsCancelKey, TBool aIsErrorQuery)
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::QueryUserResponseL"));
    TInt returnValue = KErrNone;

    iUSBQueryDlg = CAknQueryDialog::NewL( CAknQueryDialog::EConfirmationTone );

    // Show dialog with or without the Cancel
    //
    if (aIsErrorQuery) 
        {
        // aIsErrorQuery flag is set in GetQueryAttributesLC()
        // there is no defined QueryDialogError in resources so QueryOTGerror is used (contains Stop icon)
        iUSBQueryDlg->PrepareLC( R_USB_QUERY_OTG_ERROR );
        }
    else if (aIsCancelKey)
        {
        iUSBQueryDlg->PrepareLC( R_USB_QUERY_WITH_CANCEL );
        }
    else
        {
        iUSBQueryDlg->PrepareLC( R_USB_QUERY_WITHOUT_CANCEL );
        }

    if (iCoverDisplaySupported)
        {
        iUSBQueryDlg->PublishDialogL( aCoverDialogId, KUSBUINotifCategory );
        }

    iUSBQueryDlg->SetPromptL( aStringHolder );
    iUSBQueryDlg->SetFocus( ETrue );
        FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::QueryUserResponseL calling RunLD"));
    TInt keypress = iUSBQueryDlg->RunLD();

    iUSBQueryDlg = NULL;

    if (keypress) // User has accepted the dialog
        {
        returnValue = KErrNone;
            FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::QueryUserResponseL keypress"));
        }
    else
        {
        returnValue = KErrCancel;
            FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::QueryUserResponseL NO keypress"));
        }

        FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::QueryUserResponseL completed"));
    return returnValue;
    }

// ----------------------------------------------------------------------------
// CUSBUIQueriesNotifier::GetQueryAttributesLC
// Get query text and the other attributes for the query dialog. 
// ----------------------------------------------------------------------------
//
HBufC* CUSBUIQueriesNotifier::GetQueryAttributesLC(TInt& aCoverDialogId,
        TBool& aIsCancelKey, TBool& aIsErrorQuery)
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::GetQueryAttributesLC"));
    HBufC* stringHolder = NULL; // The text for the query
    aIsCancelKey = EFalse;
    aIsErrorQuery = EFalse;
    switch (iQueryType)
        {
        case EUSBStorageMediaFailure:
            {
            FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::EUSBStorageMediaFailure"));
            stringHolder = StringLoader::LoadLC( R_USB_STORAGE_MEDIA_FAILURE );
            aCoverDialogId = EUSBCoverStorageMediaFailure;
            break;
            }
        case EUSBChangeFromMassStorage:
            {
            FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::EUSBChangeFromMassStorage"));
            stringHolder = StringLoader::LoadLC( R_USB_CHANGE_FROM_MASS_STORAGE );
            aIsCancelKey = ETrue;
            aCoverDialogId = EUSBCoverChangeFromMassStorage;
            break;
            }
        case EUSBNoMemoryCard:
            {
            FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::EUSBNoMemoryCard"));
            stringHolder = StringLoader::LoadLC( R_USB_NO_MEMORY_CARD );
            aCoverDialogId = EUSBCoverNoMemoryCard;
            break;
            } 
        case EUSBNotEnoughRam:
          	{
            FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::EUSBNotEnoughRam"));
            stringHolder = StringLoader::LoadLC( R_USB_ERROR_MEMORY_NOT_ENOUGH );
            aCoverDialogId = EUSBCoverNoMemoryCard;
            //set flag to change the icon of querydialog (see QueryUserResponseL())
            aIsErrorQuery = ETrue;
            break;
            }
        case EUSBDiskFull:
          	{
            FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::EUSBDiskFull"));
            stringHolder = StringLoader::LoadLC( R_USB_ERROR_DISK_FULL );
            aCoverDialogId = EUSBCoverNoMemoryCard;
            //set flag to change the icon of querydialog (see QueryUserResponseL())
            aIsErrorQuery = ETrue;
            break;
            }
        default:
            {
            FTRACE( FPrint( _L( "[USBUINOTIF]\t CUSBUIQueriesNotifier::ERROR! Unknown query type: %d" ),
                            iQueryType ) );
            }
        }
    FLOG(_L("[USBUINOTIF]\t CUSBUIQueriesNotifier::GetQueryAttributesLC completed"));
    return stringHolder;
    }

// End of File
