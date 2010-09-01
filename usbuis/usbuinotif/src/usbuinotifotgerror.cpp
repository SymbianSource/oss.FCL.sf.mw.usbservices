/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include <featmgr.h>

#include <usbuinotif.h>                     // pck
#include <usbuinotif.rsg>                   // Own resources
#include <secondarydisplay/usbuinotifsecondarydisplay.h> // Dialog index for cover UI

#include "usbuinotifotgerror.h"             // Own class definition
#include "usbuinotifdebug.h"                // Debugging macros


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
    delete iDialerWatcher;    
    delete iQuery;    
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
    iStringIds.AppendL( R_USB_ERROR_MEMORY_NOT_ENOUGH);
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
        
    // Get parameters 
    //
    
    TPckgC<TInt> pckg( iErrorId );
    pckg.Set( aBuffer );
    iErrorId = pckg();
    
    FTRACE(FPrint(_L("[USBUINOTIF]\t CUsbUiNotifOtgError::GetParamsL iErrorId: %d"), iErrorId ));  
    if ( iErrorId < 0 || iErrorId >= iStringIds.Count() )
        {        
        User::Leave( KErrArgument);        
        }    
        
    iMessage = aMessage;
    iNeedToCompleteMessage = ETrue;
    iReplySlot = aReplySlot;  
	  
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
    FeatureManager::InitializeLibL();
    if ( FeatureManager::FeatureSupported( KFeatureIdFfKeypadNoSendKey ) )
        {    
        if (!iDialerWatcher)
            {
            iDialerWatcher = CUsbuinotifDialerWatcher::NewL(this);
            }
        }        
    FeatureManager::UnInitializeLib(); 
    iDismissed=EFalse;
    DisableKeylock();
    SuppressAppSwitching( ETrue );

    //Excute dialog and check return value
    returnValue = QueryUserResponseL();
    if (!iDismissed)
        {
        SuppressAppSwitching( EFalse );
        RestoreKeylock();
        delete iDialerWatcher;
        iDialerWatcher = NULL;
        CompleteMessage( returnValue );
        }

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
    
    // If dialog is not dismissed this is normal cancel and if query
    // doesn't exsist notifier is canceled during dismission
    if (!iDismissed || !iQuery )
        {        
        delete iDialerWatcher;
        iDialerWatcher = NULL;
        CompleteMessage( KErrCancel );
        }        
    if (iQuery)
        {
        delete iQuery;
        iQuery = NULL;
        }
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgError::Cancel() completed"));
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgError::DialerActivated
// Release all own resources (member variables)
// ----------------------------------------------------------------------------
//
void CUsbUiNotifOtgError::DialerActivated()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::AppKeyPressed()"));
    if ( iQuery )
        {
        iDismissed=ETrue;    
        Cancel();
        }    
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgError::ReActivateDialog
// Release all own resources (member variables)
// ----------------------------------------------------------------------------
//   
void CUsbUiNotifOtgError::ReActivateDialog()
    {    
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::ReActivateDialog()"));
    if ( !IsActive())
        {
        SetActive();
        iStatus = KRequestPending;
        TRequestStatus* stat = &iStatus;
        User::RequestComplete( stat, KErrNone );
        }
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
    if (iDismissed)
        {
        iQuery = CAknQueryDialog::NewL();
        }
    else
        {
        iQuery = CAknQueryDialog::NewL( CAknQueryDialog::EErrorTone );
        }
    
    
    iDismissed=EFalse;
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
