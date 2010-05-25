/*
 * Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Declares USB UI notifiers base class.
 *
 */

// INCLUDE FILES
#include <eikenv.h>          // Eikon environment
#include <bautils.h>         // BAFL utils (for language file)
#include <StringLoader.h>    // Localisation stringloader
#include <centralrepository.h> 
#include <coreapplicationuisdomainpskeys.h>
#include <data_caging_path_literals.hrh> 
#include <featmgr.h>
#include <AknNotiferAppServerApplication.h>

#include "usbnotifier.h"     // Own class 
#include "usbuinotifdebug.h"
#include "aknkeylock.h" //RAknKeyLock
// CONSTANTS

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::CBTNotifierBase
// C++ default constructor can NOT contain any code, that
// might leave. Sets the AOs priority and puts
// itself to the active scheduler stack.
// ----------------------------------------------------------------------------
//
CUSBUINotifierBase::CUSBUINotifierBase() :
    CActive( EPriorityStandard )
    {
        FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::Default constructor()"));
    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::ConstructL
// Symbian 2nd phase constructor can leave.
// Create registry object and open resource file.
// ----------------------------------------------------------------------------
//
void CUSBUINotifierBase::ConstructL()
    {
    iEikEnv = CEikonEnv::Static();
    iAppsKeyBlocked = EFalse;
    iKeylockChanged = EFalse;

    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::ConstructL()"));
    TFileName filename;

    const TDriveNumber KStoreDrive = EDriveZ;
    TDriveUnit driveUnit( KStoreDrive );
    TDriveName drive = driveUnit.Name();
    filename.Insert( 0, drive );

    filename += KDC_RESOURCE_FILES_DIR; // From data_caging_path_literals.hrh
    filename += KResourceFileName;
    BaflUtils::NearestLanguageFile( iEikEnv->FsSession(), filename );
    iResourceFileFlag = iEikEnv->AddResourceFileL( filename );

    FeatureManager::InitializeLibL();
    iCoverDisplaySupported = FeatureManager::FeatureSupported(
            KFeatureIdCoverDisplay );
    FeatureManager::UnInitializeLib();

    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::ConstructL() completed"));
    }

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
//
CUSBUINotifierBase::~CUSBUINotifierBase()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::Destructor"));
    //Make sure that the request is completed. Note that inside the destructor,
    //this virtual function call is to the local CUSBUINotifierBase::Cancel, 
    //not to any possibly derived class implementation. 
    Cancel();
    iEikEnv->DeleteResourceFile( iResourceFileFlag );

    // Complete the RMessage2 if needed
    //
    CompleteMessage( KErrDied );

    // Activate apps -key again (if not previously activated yet)
    SuppressAppSwitching( EFalse );

    //Don't call RestoreKeylock here. It's up to specific note notfier logic. 
    //If failed to RestoreKeylock there, it doesn't matter to leave keylock open.

    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::Destructor completed"));
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::Release
// Release itself. Call to destructor.
// ----------------------------------------------------------------------------
//
void CUSBUINotifierBase::Release()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::Release()"));
    delete this;
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::Info
// Return registered information.
// ----------------------------------------------------------------------------
//
CUSBUINotifierBase::TNotifierInfo CUSBUINotifierBase::Info() const
    {
    FTRACE(FPrint(_L("[USBUINOTIF]\t CUSBUINotifierBase::Info() id %d channel %d priority %d"), iInfo.iUid, iInfo.iChannel, iInfo.iPriority ));
    return iInfo;
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::StartL
// Synchronic notifier launch. Does nothing
// ----------------------------------------------------------------------------
//
TPtrC8 CUSBUINotifierBase::StartL(const TDesC8& /*aBuffer*/)
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::StartL()"));

    TPtrC8 ret( KNullDesC8 );
    return (ret);
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::StartL
// Asynchronic notifier launch.
// ----------------------------------------------------------------------------
//
void CUSBUINotifierBase::StartL(const TDesC8& aBuffer, TInt aReplySlot,
        const RMessagePtr2& aMessage)
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::StartL()"));
    TRAPD( err, GetParamsL( aBuffer, aReplySlot, aMessage ));
    if (err)
        {
        aMessage.Complete( err );
        iNeedToCompleteMessage = EFalse;
        User::Leave( err );
        }
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::StartL completed()"));
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::Cancel
// Cancelling method.
// ----------------------------------------------------------------------------
//
void CUSBUINotifierBase::Cancel()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::Cancel()"));
    CActive::Cancel();
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::Cancel completed()"));
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::UpdateL
// Notifier update. Not supported.
// ----------------------------------------------------------------------------
//
TPtrC8 CUSBUINotifierBase::UpdateL(const TDesC8& /*aBuffer*/)
    {
    TPtrC8 ret( KNullDesC8 );
    return (ret);
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::DoCancel
// This method will be called by framework (CActive)
// if active object is still active.
// Does nothing here.
// ----------------------------------------------------------------------------
//
void CUSBUINotifierBase::DoCancel()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::DoCancel()"));
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::RunError
// This method is called if any leaving has been occured
// during RunL. Optional method for CActive derived objects.
// ----------------------------------------------------------------------------
//
TInt CUSBUINotifierBase::RunError(TInt aError)
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::RunError()"));

    // Activate apps -key again (if not previously activated yet)
    //
    SuppressAppSwitching( EFalse );

    // Write error message to caller
    //
    CompleteMessage( aError );

    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::RunError() completed"));

    return aError;
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::SuppressAppSwitching
// 
// ----------------------------------------------------------------------------
//
void CUSBUINotifierBase::SuppressAppSwitching(TBool aEnable)
    {
    FTRACE(FPrint(_L("[USBUINOTIF]\t CUSBUINotifierBase::SuppressAppSwitching() %d"), aEnable));

    if (iAppsKeyBlocked != aEnable)
        {
        TInt err = iAknServer.ConnectAndSendAppsKeySuppress( aEnable ); // error is stored only for logging purposes
        iAppsKeyBlocked = aEnable;
        FTRACE(FPrint(_L("[USBUINOTIF]\t CUSBUINotifierBase::SuppressAppSwitching() ConnectAndSendAppsKeySuppress returned %d"), err ));
        }

    if (!iAppsKeyBlocked)
        {
        iAknServer.Close(); // close the connection once we have re-enabled swithcing
        }
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::SuppressAppSwitching() completed"));
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::CompleteMessage
// Check if message needs to be completed and complete it. 
// ----------------------------------------------------------------------------
//
void CUSBUINotifierBase::CompleteMessage(TInt aReason)
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::CompleteMessage()"));
    if (iNeedToCompleteMessage)
        {
        iMessage.Complete( aReason );
        iNeedToCompleteMessage = EFalse;
        }
    iReplySlot = 0;
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::CompleteMessage() completed"));
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::DisableKeylock
// ----------------------------------------------------------------------------
// Turn off the keyguard if it was on.
//
void CUSBUINotifierBase::DisableKeylock()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::DisableKeylock()"));
    RAknKeylock2 keylock;
    iKeylockChanged = EFalse;
    if (KErrNone == keylock.Connect())
        {
        if (keylock.IsKeyLockEnabled()) //Check and save the keylock status
            {
            keylock.DisableWithoutNote();// Unlock
            iKeylockChanged = ETrue;
            }
        keylock.Close();
        }
    else
        {
        FLOG( _L( "[USBUINOTIF]\t CUSBUINotifierBase::DisableKeylock() fail caused by RAknKeylock2::Connect()") );
        }

    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::DisableKeylock() completed"));
    }

// ----------------------------------------------------------------------------
// CUSBUINotifierBase::RestoreKeylock
// ----------------------------------------------------------------------------
// Restore the keyguard on.
//
void CUSBUINotifierBase::RestoreKeylock()
    {
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::RestoreKeylock()"));
    if (iKeylockChanged)
        {
        FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::RestoreKeylock(): iKeylockChanged true"));
        RAknKeylock2 keylock;
        FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::RestoreKeylock(): RAknKeyLock2 initialized"));
        if (KErrNone == keylock.Connect())
            {
            FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::RestoreKeylock(): RAknKeyLock2::Connect() complete"));
            keylock.EnableWithoutNote();// Lock back
            FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::RestoreKeylock(): RAknKeyLock2::EnableWithoutNote() complete"));
            keylock.Close();
            FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::RestoreKeylock(): RAknKeyLock2::Close() complete"));
            iKeylockChanged = EFalse;
            }
        else
            {
            FLOG( _L( "[USBUINOTIF]\t CUSBUINotifierBase::RestoreKeylock() fail caused by RAknKeylock2::Connect()") );
            }
        }
    FLOG(_L("[USBUINOTIF]\t CUSBUINotifierBase::RestoreKeylock() completed"));
    }

// End of File
