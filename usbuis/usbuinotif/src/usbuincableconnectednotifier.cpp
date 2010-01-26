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
 * Description:  USB Cable Connected notifier class.
 *
 */
// INCLUDE FILES
#include <eikenv.h>          // Eikon environment
#include <bautils.h>         // BAFL utils (for language file)
#include <aknlists.h>
#include <aknPopup.h> 
#include <StringLoader.h>    // Localisation stringloader
#include <utf.h>             // Unicode character conversion utilities
#include <AknsUtils.h>
#include <usb.h>
#include <usbuinotif.rsg>                   // Own resources
#include <centralrepository.h> 
#include <e32property.h>
//for loading icons USB fork and empty
#include <AknIconArray.h>
#include <avkon.mbg>
#include <AknsConstants.h>
#include <aknmessagequerydialog.h>
//for cover display support
#include <AknMediatorFacade.h>
#include <SecondaryDisplay/usbuinotifsecondarydisplay.h> // Dialog index for cover UI
#include <akndiscreetpopup.h>
#include <e32uid.h> // KExecutableImageUid
#include <usbui.mbg>
#include "usbuincableconnectednotifier.h"   // Own class definition
#include "usbuinotifdebug.h"                // Debugging macros
#include "UsbWatcherInternalCRKeys.h"
#include "usbpersonalityids.h"
#include "usbuinotif.hrh"

// Literals

_LIT(KUSBExe, "usbclasschangeui.exe");
const TInt KUSBUIUid = 0x102068E2;
_LIT(KFileDrive,"z:");
_LIT(KUSBUIconFileName, "usbui.mif");

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CUSBUICableConnectedNotifier* CUSBUICableConnectedNotifier::NewL()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::NewL"));
    CUSBUICableConnectedNotifier* self =
            new (ELeave) CUSBUICableConnectedNotifier();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::NewL completed"));
    return self;
    }

// ----------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::CUSBUICableConnectedNotifier
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CUSBUICableConnectedNotifier::CUSBUICableConnectedNotifier()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::default constructor"));
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CUSBUICableConnectedNotifier::~CUSBUICableConnectedNotifier()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::destructor"));
    //Make sure that the request is completed. Note that inside the destructor,
    //this virtual function call is to the local CUSBUICableConnectedNotifier::
    //Cancel, not to any possibly derived class implementation. 
    Cancel();
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::destructor completed"));
    }

// ----------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::RegisterL
// Register notifier.
// ----------------------------------------------------------------------------
//
CUSBUICableConnectedNotifier::TNotifierInfo CUSBUICableConnectedNotifier::RegisterL()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::RegisterL"));
    iInfo.iUid = KCableConnectedNotifierUid;
    iInfo.iChannel = KQueriesNotifier; // work in the same channel with the other 
    // queries so that we can use priorities
    iInfo.iPriority = ENotifierPriorityLow; // must be smaller than queries notifier
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::RegisterL completed"));
    return iInfo;
    }

// ----------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::GetParamsL
//  Jump to RunL as soon as possible.
// ----------------------------------------------------------------------------
//
void CUSBUICableConnectedNotifier::GetParamsL(const TDesC8& /*aBuffer*/,
        TInt aReplySlot, const RMessagePtr2& aMessage)
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::GetParamsL"));
    if ( iReplySlot != 0 || iNeedToCompleteMessage)
        {
        User::Leave(KErrInUse);
        }

    iMessage = aMessage;
    iNeedToCompleteMessage = ETrue;
    iReplySlot = aReplySlot;

    // Call SetActive() so RunL() will be called by the active scheduler
    //
    iStatus = KRequestPending;
    TRequestStatus* stat = &iStatus;
    SetActive();
    User::RequestComplete(stat, KErrNone);
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::GetParamsL() completed"));
    }

// ----------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::RunL
// Ask user response and return it to caller.
// ----------------------------------------------------------------------------
//
void CUSBUICableConnectedNotifier::RunL()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::RunL"));

    DisableKeylock();
    SuppressAppSwitching(ETrue);
    RunQueryL();
    SuppressAppSwitching(EFalse);
    RestoreKeylock();

    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::RunL() completed"));
    }

// ----------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::Cancel()
// Release all own resources (member variables)
// ----------------------------------------------------------------------------
//
void CUSBUICableConnectedNotifier::Cancel()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::Cancel() "));

    CompleteMessage(KErrCancel);
    CUSBUINotifierBase::Cancel();

    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::Cancel() completed"));
    }

// -----------------------------------------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::GetPersonalityStringL
// Get the strings for ask on connection message query 
// -----------------------------------------------------------------------------------------------------------
//
void CUSBUICableConnectedNotifier::GetPersonalityStringL(
       HBufC*& aHeader,HBufC*& aDescription )
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::GetPersonalityStringL"));
    TInt CurrentPersonalityID = 0; // The ID of current USB mode
    GetCurrentIdL(CurrentPersonalityID);
    RUsb usbman;
    User::LeaveIfError(usbman.Connect());
    CleanupClosePushL(usbman);
    User::LeaveIfError(usbman.GetDescription(CurrentPersonalityID,
            aDescription));
    CleanupStack::PopAndDestroy(&usbman);
   
    aHeader = StringLoader::LoadL(R_USB_MODE_MSG_HEADER);

    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::GetPersonalityStringL completed"));
    }

// -----------------------------------------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::RunQueryL
// Run the ask on connection message query
// -----------------------------------------------------------------------------------------------------------
//
void CUSBUICableConnectedNotifier::RunQueryL()
    {
    FLOG( _L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::RunQueryL()"));
  
   HBufC* header = NULL;
   HBufC* description =NULL;
   GetPersonalityStringL(header, description);
  
   CleanupStack::PushL(header);
   CleanupStack::PushL(description);
  
   TFileName usbUiIconFilename( KFileDrive );
   usbUiIconFilename += KDC_APP_BITMAP_DIR;
   usbUiIconFilename += KUSBUIconFileName;
   CAknDiscreetPopup::ShowGlobalPopupL(*header,*description,  KAknsIIDQgnPropUsb, AknIconUtils::AvkonIconFileName(),
           EMbmAvkonQgn_prop_usb, EMbmAvkonQgn_prop_usb_mask,KAknDiscreetPopupDurationLong, EUSBUICmdDiscreetTapped,( MEikCommandObserver* ) this);
 
    CompleteMessage(KErrCancel);
    CleanupStack::PopAndDestroy(description);
    CleanupStack::PopAndDestroy(header);
   
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::RunQueryL completed"));
    }
// ----------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::GetCurrentIdL
// get the current personality id
// ----------------------------------------------------------------------------
//
void CUSBUICableConnectedNotifier::GetCurrentIdL(TInt& aCurrentPersonality)
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::GetCurrentIdL"));
    // Connecting and initialization:
    CRepository* repository = CRepository::NewL(KCRUidUsbWatcher);
    CleanupStack::PushL(repository);
    // Get the current USB mode
    repository->Get(KUsbWatcherPersonality, aCurrentPersonality);
    CleanupStack::PopAndDestroy(repository);
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::GetCurrentIdL completed"));
    }

// ----------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::ProcessCommandL()
// when discreet pop up is tapped the command will be handled here 
// ----------------------------------------------------------------------------
//
void CUSBUICableConnectedNotifier::ProcessCommandL(TInt aCommandId)
    {
    switch ( aCommandId )
            {
        case EUSBUICmdDiscreetTapped:
            {
            TUidType uidtype(KExecutableImageUid, TUid::Uid(0x00),TUid::Uid(KUSBUIUid));
            CreateChosenViewL(KUSBExe(),uidtype);  
            }
            break;
        default:
                  
        break;
            }
    }
// ---------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::CreateChosenViewL()
// creates the USB UI setting view
// ---------------------------------------------------------------------------
//  
    void CUSBUICableConnectedNotifier::CreateChosenViewL(const TDesC & aProcessName,const TUidType & aUidType) const
    {                 
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::CreateDesiredViewL() "));
        RProcess usbUiProcess;                
        User::LeaveIfError(usbUiProcess.Create(aProcessName, KNullDesC, aUidType));   
        usbUiProcess.Resume();
        usbUiProcess.Close();                     
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::CreateDesiredViewL() "));         
    
    }
// End of File
