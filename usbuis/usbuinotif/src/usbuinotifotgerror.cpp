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

#include <usbuinotif.h>                     // pck
#include <usbuinotif.rsg>                   // Own resources
#include "usbuinotifotgerror.h"             // Own class definition
#include "usbuinotifdebug.h"                // Debugging macros
#include "usbuinotifsecondarydisplay.h"     // Dialog index for cover UI
// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgError::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CUsbUiNotifOtgError* CUsbUiNotifOtgError::NewL()
    {
    CUsbUiNotifOtgError* self = new (ELeave) CUsbUiNotifOtgError();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgError::CUsbUiNotifOtgError
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CUsbUiNotifOtgError::CUsbUiNotifOtgError() :
    iStringIds( KUsbUiNotifOtgGeneralQueryGranularity)
    {
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CUsbUiNotifOtgError::~CUsbUiNotifOtgError()
    {
    //Make sure that the request is completed. Note that inside the destructor,
    //this virtual function call is to local CUsbUiNotifOtgError::Cancel, 
    //not to any possibly derived class implementation. 
    Cancel();
    }

void CUsbUiNotifOtgError::ConstructL()
    {
    CUSBUINotifierBase::ConstructL();
    iStringIds.AppendL( R_USB_OTG_ERROR_CURRENT_LIMIT);
    iStringIds.AppendL( R_USB_OTG_ERROR_TOO_MUCH_CURRENT_REQUIRED);
    iStringIds.AppendL( R_USB_OTG_ERROR_UNSUPPORTED);
    iStringIds.AppendL( R_USB_OTG_ERROR_HUB_UNSUPPORTED);
    iStringIds.AppendL( R_USB_OTG_ERROR_UNRECOVERABLE);
    iStringIds.AppendL( R_USB_OTG_ERROR_ATTACH_TIMEDOUT);
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgError::RegisterL
// Register notifier.
// ----------------------------------------------------------------------------
//
CUsbUiNotifOtgError::TNotifierInfo CUsbUiNotifOtgError::RegisterL()
    {
    iInfo.iUid = KUsbUiNotifOtgError;
    iInfo.iChannel = KUsbUiNotifOtgError;
    iInfo.iPriority = ENotifierPriorityVHigh;
    return iInfo;
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgError::GetParamsL
//  Jump to RunL as soon as possible.
// ----------------------------------------------------------------------------
//
void CUsbUiNotifOtgError::GetParamsL(const TDesC8& aBuffer, TInt aReplySlot,
        const RMessagePtr2& aMessage)
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgError::GetParamsL"));
    if (iQuery || iReplySlot != 0 || iNeedToCompleteMessage)
        {
        User::Leave( KErrInUse );
        }

    iMessage = aMessage;
    iNeedToCompleteMessage = ETrue;
    iReplySlot = aReplySlot;

    // Get parameters 
    //
    iErrorId = 0;
    TPckgC<TInt> pckg( iErrorId );
    pckg.Set( aBuffer );
    iErrorId = pckg();

    SetActive();
    iStatus = KRequestPending;
    TRequestStatus* stat = &iStatus;
    User::RequestComplete( stat, KErrNone );
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgError::GetParamsL() completed"));
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgError::RunL
// Ask user response and return it to caller.
// ----------------------------------------------------------------------------
//
void CUsbUiNotifOtgError::RunL()
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgError::RunL"));
    TInt returnValue = KErrNone;

    DisableKeylock();
    SuppressAppSwitching( ETrue );

    //Excute dialog and check return value
    returnValue = QueryUserResponseL();

    SuppressAppSwitching( EFalse );
    RestoreKeylock();
    CompleteMessage( returnValue );

    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgError::RunL() completed"));
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgError::Cancel
// Release all own resources (member variables)
// ----------------------------------------------------------------------------
//
void CUsbUiNotifOtgError::Cancel()
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgError::Cancel"));
    if (iQuery)
        {
        delete iQuery;
        iQuery = NULL;
        }
    CompleteMessage( KErrCancel );

    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgError::Cancel() completed"));
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgError::QueryUserResponseL
// Show query dialog. 
// ----------------------------------------------------------------------------
//
TInt CUsbUiNotifOtgError::QueryUserResponseL()
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgError::QueryUserResponseL"));
    TInt returnValue = KErrNone;
    TInt resourceId = R_USB_QUERY_OTG_ERROR;

    iQuery = CAknQueryDialog::NewL( CAknQueryDialog::EErrorTone );

    if (iCoverDisplaySupported)
        {
        iQuery->PublishDialogL( iErrorId, KUsbUiNotifOtgError );
        }
    HBufC *stringHolder = StringLoader::LoadLC( iStringIds[iErrorId] );

    TInt keypress = iQuery->ExecuteLD( resourceId, *stringHolder );

    CleanupStack::PopAndDestroy( stringHolder );

    iQuery = NULL; // Dialog destroyed

    returnValue = keypress ? KErrNone : KErrCancel; //OK?

    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgError::QueryUserResponseL completed"));
    return returnValue;
    }

// End of File
