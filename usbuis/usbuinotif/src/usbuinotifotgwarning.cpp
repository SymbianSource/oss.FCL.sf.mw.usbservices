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
* Description:  USB UI notes notifier class.
 *
*/


// INCLUDE FILES
#include <eikenv.h>             // Eikon environment
#include <aknnotedialog.h>
#include <bautils.h>            // BAFL utils (for language file)
#include <StringLoader.h>       // Localisation stringloader
#include <AknMediatorFacade.h>  // for cover display support
#include <usbuinotif.rsg>       // Own resources
#include "usbuinotifotgwarning.h"        // Own class definition
#include "usbuinotifdebug.h"                // Debugging macros
#include "usbuinotifsecondarydisplay.h"     // Dialog index for cover UI
// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgWarning::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CUsbUiNotifOtgWarning* CUsbUiNotifOtgWarning::NewL()
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::NewL()"));
    CUsbUiNotifOtgWarning* self = new (ELeave) CUsbUiNotifOtgWarning();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::NewL completed()"));
    return self;
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgWarning::CUsbUiNotifOtgWarning
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CUsbUiNotifOtgWarning::CUsbUiNotifOtgWarning() :
    iStringIds( KUsbUiNotifOtgGeneralNoteGranularity), iNote( NULL)
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::constructor()"));
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CUsbUiNotifOtgWarning::~CUsbUiNotifOtgWarning()
    {
        FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::destructor()"));
    //Make sure that the request is completed. Note that inside the destructor,
    //this virtual function call is to local CUsbUiNotifOtgWarning::Cancel, 
    //not to any possibly derived class implementation. 
    Cancel();
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::destructor completed()"));
    }

void CUsbUiNotifOtgWarning::ConstructL()
    {
    CUSBUINotifierBase::ConstructL();
    iStringIds.AppendL( R_USB_OTG_WARNING_PARTIAL_SUPPORT);
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgWarning::RegisterL
// Register notifier.
// ----------------------------------------------------------------------------
//
CUsbUiNotifOtgWarning::TNotifierInfo CUsbUiNotifOtgWarning::RegisterL()
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::RegisterL"));
    iInfo.iUid = KUsbUiNotifOtgWarning;
    iInfo.iChannel = KUsbUiNotifOtgWarning;
    iInfo.iPriority = ENotifierPriorityVHigh;
        FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::RegisterL completed()"));
    return iInfo;
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgWarning::Cancel
// Release all own resources (member variables)
// ----------------------------------------------------------------------------
//
void CUsbUiNotifOtgWarning::Cancel()
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::Cancel"));
    if (iNote)
        {
        FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::Cancel - delete iNote"));
        delete iNote;
        iNote = NULL;
        }
    CompleteMessage( KErrNone );

    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::Cancel() completed"));
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgWarning::GetParamsL
// Mandatory for USB UI Notifiers when using asynchronous launch. 
// This notifier is synchronous so this function is not used.
// ----------------------------------------------------------------------------
//
void CUsbUiNotifOtgWarning::GetParamsL(const TDesC8& aBuffer,
        TInt aReplySlot, const RMessagePtr2& aMessage)
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::GetParamsL"));
    if (iNote || iReplySlot != 0 || iNeedToCompleteMessage)
        {
        User::Leave( KErrInUse );
        }

    // Get parameters 
    //    
    TPckgC<TInt> pckg( iNoteId );
    pckg.Set( aBuffer );
    iNoteId = pckg();
    
    FTRACE(FPrint(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::GetParamsL iNoteId: %d"), iNoteId ));  
    if ( iNoteId < 0 || iNoteId >= iStringIds.Count() )
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
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::GetParamsL() completed"));
    }

// ----------------------------------------------------------------------------
// CUsbUiNotifOtgWarning::RunL
// Mandatory for Active Objects. This notifier is synchronous 
// so this function is not used.
// ----------------------------------------------------------------------------
//
void CUsbUiNotifOtgWarning::RunL()
    {
    FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::RunL"));

    // Create confirmation note
    //    
    HBufC* str = StringLoader::LoadL( iStringIds[iNoteId] );
    CleanupStack::PushL( str );
    iNote = new (ELeave) CAknWarningNote( ETrue );

    iNote->SetTimeout( CAknNoteDialog::ENoTimeout );

    if (iCoverDisplaySupported)
        {
        iNote->PublishDialogL( iNoteId, KUsbUiNotifOtgWarning );
        }

    TInt t = iNote->ExecuteLD( *str );
    iNote = NULL;
    CleanupStack::PopAndDestroy( str );

    CompleteMessage( KErrNone );
  FLOG(_L("[USBUINOTIF]\t CUsbUiNotifOtgWarning::RunL() completed"));
    }

// End of File
