/*
* Copyright (c) 2007, 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <usb/hostms/srverr.h> 

#include <usbuinotif.h>                     // pck
#include <usbuinotif.rsg>                   // Own resources
#include "usbuinotifmsmmerror.h"             // Own class definition
#include "usbuinotifdebug.h"                // Debugging macros
#include "usbuinotifsecondarydisplay.h"     // Dialog index for cover UI
// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgError::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CUsbUiNotifMSMMError* CUsbUiNotifMSMMError::NewL()
    {
    CUsbUiNotifMSMMError* self = new (ELeave) CUsbUiNotifMSMMError();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifMSMMError::CUsbUiNotifMSMMError
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CUsbUiNotifMSMMError::CUsbUiNotifMSMMError() :
    iStringIds( KUsbUiNotifOtgGeneralQueryGranularity)
    {
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CUsbUiNotifMSMMError::~CUsbUiNotifMSMMError()
    {
    //Make sure that the request is completed. Note that inside the destructor,
    //this virtual function call is to local CUsbUiNotifMSMMError::Cancel, 
    //not to any possibly derived class implementation. 
    Cancel();
    }

void CUsbUiNotifMSMMError::ConstructL()
    {
    CUSBUINotifierBase::ConstructL();
    iStringIds.AppendL( R_USB_MSMM_ERROR_GENERAL_MASS_STORAGE_ERROR);
    iStringIds.AppendL( R_USB_MSMM_ERROR_UNKNOWN_FILESYSTEM);
    iStringIds.AppendL( R_USB_MSMM_ERROR_OUT_OF_MEMORY);  
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifMSMMError::RegisterL
// Register notifier.
// ----------------------------------------------------------------------------
//
CUsbUiNotifMSMMError::TNotifierInfo CUsbUiNotifMSMMError::RegisterL()
    {
    iInfo.iUid = KUsbUiNotifMsmmError;
    iInfo.iChannel = KUsbUiNotifMsmmError;
    iInfo.iPriority = ENotifierPriorityVHigh;
    return iInfo;
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifMSMMError::GetParamsL
//  Jump to RunL as soon as possible.
// ----------------------------------------------------------------------------
//
void CUsbUiNotifMSMMError::GetParamsL(const TDesC8& aBuffer, TInt aReplySlot,
        const RMessagePtr2& aMessage)
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifMSMMError::GetParamsL"));
    if (iQuery || iReplySlot != 0 || iNeedToCompleteMessage)
        {
        User::Leave( KErrInUse );
        }

    iMessage = aMessage;
    iNeedToCompleteMessage = ETrue;
    iReplySlot = aReplySlot;

    // Get parameters 
    //    
    THostMsErrData error;
    TPckgC<THostMsErrData> pckg( error);
    pckg.Set( aBuffer );
    switch (pckg().iError)
        {
        case  EHostMsErrUnknownFileSystem:
            iErrorId = EUsbMSMMUnknownFileSystem;
            break;        
        case  EHostMsErrOutOfMemory:
            iErrorId = EUsbMSMMOutOfMemory;
            break;        
        default:
            iErrorId = EUsbMSMMGeneralError;
        }

    SetActive();
    iStatus = KRequestPending;
    TRequestStatus* stat = &iStatus;
    User::RequestComplete( stat, KErrNone );
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifMSMMError::GetParamsL() completed"));
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifMSMMError::RunL
// Ask user response and return it to caller.
// ----------------------------------------------------------------------------
//
void CUsbUiNotifMSMMError::RunL()
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifMSMMError::RunL"));
    TInt returnValue = KErrNone;

    DisableKeylock();
    SuppressAppSwitching( ETrue );

    //Excute dialog and check return value
    returnValue = QueryUserResponseL();

    SuppressAppSwitching( EFalse );
    RestoreKeylock();
    CompleteMessage( returnValue );

    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifMSMMError::RunL() completed"));
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifMSMMError::Cancel
// Release all own resources (member variables)
// ----------------------------------------------------------------------------
//
void CUsbUiNotifMSMMError::Cancel()
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifMSMMError::Cancel"));
    if (iQuery)
        {
        delete iQuery;
        iQuery = NULL;
        }
    CompleteMessage( KErrCancel );

    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifMSMMError::Cancel() completed"));
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifMSMMError::QueryUserResponseL
// Show query dialog. 
// ----------------------------------------------------------------------------
//
TInt CUsbUiNotifMSMMError::QueryUserResponseL()
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifMSMMError::QueryUserResponseL"));
    TInt returnValue = KErrNone;
    TInt resourceId = R_USB_QUERY_OTG_ERROR;

    iQuery = CAknQueryDialog::NewL( CAknQueryDialog::EErrorTone );

    if (iCoverDisplaySupported)
        {
        iQuery->PublishDialogL( iErrorId, KUsbUiNotifMsmmError );
        }
    HBufC *stringHolder = StringLoader::LoadLC( iStringIds[iErrorId] );

    TInt keypress = iQuery->ExecuteLD( resourceId, *stringHolder );

    CleanupStack::PopAndDestroy( stringHolder );

    iQuery = NULL; // Dialog destroyed

    returnValue = keypress ? KErrNone : KErrCancel; //OK?

    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifMSMMError::QueryUserResponseL completed"));
    return returnValue;
    }

// End of File
