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
#include <usbman.h>
#include <usbuinotif.rsg>                   // Own resources
#include <centralrepository.h> 
#include <e32property.h>
#include <e32uid.h> // KExecutableImageUid
#include "usbuincableconnectednotifier.h"   // Own class definition
#include "usbuinotifdebug.h"                // Debugging macros
#include "UsbWatcherInternalCRKeys.h"
#include "usbuinotif.hrh"

// Literals

_LIT(KUSBExe, "usbapplication.exe");
const TInt KUSBUIUid = 0x2002BCA3;
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
    delete iDialog;
         
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
void CUSBUICableConnectedNotifier::GetPersonalityStringLC(
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
    CleanupStack::PushL(aDescription);
    aHeader = StringLoader::LoadL(R_USB_MODE_MSG_HEADER);
    CleanupStack::PushL(aHeader);
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
 
    _LIT(KNotificationType,"com.nokia.hb.devicenotificationdialog/1.0");
    _LIT(KTouchActivation, "touchActivation");
    _LIT(KIconName, "iconName");
    _LIT(KTitle, "title");
    _LIT(KText, "text");

    HBufC* header = NULL;
    HBufC* description =NULL;   
    GetPersonalityStringLC(header, description);
   
    TFileName usbUiIconFilename( KFileDrive );
    usbUiIconFilename += KDC_APP_BITMAP_DIR;
    usbUiIconFilename += KUSBUIconFileName;
    if( iDialog )
        {
        delete iDialog;
        iDialog = NULL;
        }
    iDialog = CHbDeviceDialog::NewL();
    CHbSymbianVariantMap *parameters =CHbSymbianVariantMap::NewL();
    CleanupStack::PushL(parameters);

    AddParameterL(parameters, KTitle, header, CHbSymbianVariant::EDes);
    AddParameterL(parameters, KText, description, CHbSymbianVariant::EDes);
    AddParameterL(parameters, KIconName, &usbUiIconFilename, CHbSymbianVariant::EDes);
    TBool activation=ETrue;
    AddParameterL(parameters, KTouchActivation, &activation, CHbSymbianVariant::EBool);

    FLOG( _L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::RunQueryL() show") );     
    TBool error = iDialog->Show(KNotificationType ,*parameters, this);
    FTRACE( FPrint(
        _L( "[USBUINOTIF]\t CUSBUICableConnectedNotifier::RunQueryL() error = %d" ),
        error ) );

    CleanupStack::PopAndDestroy(parameters);    
    CleanupStack::PopAndDestroy(header);
    CleanupStack::PopAndDestroy(description);
    
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
    TInt ret = repository->Get(KUsbWatcherPersonality, aCurrentPersonality);
    FTRACE( FPrint(
        _L( "[USBWATCHER]\t CUSBUICableConnectedNotifier::GetCurrentIdL() ret = %d" ),
        ret ) );    
    CleanupStack::PopAndDestroy(repository);
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::GetCurrentIdL completed"));
    }

// ----------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::DataReceived
// launches the QT usb ui setting
// ----------------------------------------------------------------------------
//
void CUSBUICableConnectedNotifier::DataReceived(CHbSymbianVariantMap& aData)
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::DataReceived"));
    _LIT(KResult, "result");
    _LIT(KActivated, "activated");
    TInt result = 0;
    MDesCArray& keys = aData.Keys();
    HBufC* key = NULL;
    for(TInt i = 0; i < keys.MdcaCount(); i++)
        {
        result = keys.MdcaPoint(i).Compare(KResult); 
        if (result == 0)
            {        
            key = HBufC::New(KResult().Length());
            if (key)
                {
                *key = KResult;            
                const CHbSymbianVariant* variant = aData.Get(*key);
                delete key;
                HBufC* value = static_cast<HBufC*>(variant->Data());
                result = value->Des().Compare(KActivated);
                if (result == 0)
                    {
                    TUidType uidtype(KExecutableImageUid, TUid::Uid(0x00),
                            TUid::Uid(KUSBUIUid));
                    LaunchApplication(KUSBExe(), uidtype);
                    }
                break;
                }
            }            
        }
    }

// ----------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::DeviceDialogClosed
// Not implemented
// ----------------------------------------------------------------------------
//      
 void CUSBUICableConnectedNotifier::DeviceDialogClosed(TInt /*aCompletionCode*/)
     {
     // no implementation    
     }
      
// ---------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::LaunchApplication()
// launches the application 
// ---------------------------------------------------------------------------
//  
void CUSBUICableConnectedNotifier::LaunchApplication(const TDesC & aProcessName, 
        const TUidType & aUidType) const
    {                 
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::LaunchApplication()"));
    RProcess usbUiProcess;                
    TInt result = usbUiProcess.Create(aProcessName, KNullDesC, aUidType);
    FTRACE( FPrint(
        _L( "[USBUINOTIF]\t CUSBUICableConnectedNotifier::LaunchApplication() result = %d" ),
        result ) );    
    usbUiProcess.Resume();
    usbUiProcess.Close();                     
    FLOG(_L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::LaunchApplication() completed"));             
    }
    
// ---------------------------------------------------------------------------
// CUSBUICableConnectedNotifier::AddParameter()
// Adds a string parameter to the dialog parameters.
// ---------------------------------------------------------------------------
//  
void CUSBUICableConnectedNotifier::AddParameterL(
        CHbSymbianVariantMap* aParameters,
        const TDesC& aKey,
        const TAny* aData,
        CHbSymbianVariant::TType aDataType)
    {
    FLOG( _L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::AddStringParameter()") );
    FTRACE( FPrint(
        _L( "[USBWATCHER]\t CUSBUICableConnectedNotifier::RunQueryL() aKey = %S" ),
        &aKey ) );
    CHbSymbianVariant* variant = CHbSymbianVariant::NewL(aData, aDataType);
    //aParameters takes variant ownership
    User::LeaveIfError(aParameters->Add(aKey, variant));
    FLOG( _L("[USBUINOTIF]\t CUSBUICableConnectedNotifier::AddStringParameter() completed") );
    }

// End of File
