/*
* Copyright (c) 2005, 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This implements USBClassChangeUIPluginCRWatcher class.
*
*/



// INCLUDE FILES
#include "USBClassChangeUIPluginCRWatcher.h"
#include "UsbWatcherInternalCRKeys.h"
#include "USBClassChangeUIPluginModel.h"
#include "USBClassChangeUIPluginDebug.h"




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginCRWatcher::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUSBClassChangeUIPluginCRWatcher* CUSBClassChangeUIPluginCRWatcher::NewL(
    MUSBUICRSettingObserver& aOwner, TUint32 aKey, TBool aOneShot)
    {
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher:NewL"));
    CUSBClassChangeUIPluginCRWatcher* self 
        = new (ELeave) CUSBClassChangeUIPluginCRWatcher(aOwner, aKey, aOneShot);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }


// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginCRWatcher::~CUSBClassChangeUIPluginCRWatcher
// C++ default destructor
// -----------------------------------------------------------------------------
//    
CUSBClassChangeUIPluginCRWatcher::~CUSBClassChangeUIPluginCRWatcher()
    {
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher: desctructor"));
    Cancel(); // cancels any existing active object orders for iStatus
    delete iPersonalityRepository; // deletes CRepository* member object.
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher: desctructor complete"));
    }


// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginCRWatcher::CUSBClassChangeUIPluginCRWatcher
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUSBClassChangeUIPluginCRWatcher::CUSBClassChangeUIPluginCRWatcher(
    MUSBUICRSettingObserver& aOwner, TUint32 aKey, TBool aOneShot)
    : CActive(EPriorityStandard), 
    iOwner(aOwner),
    iKey(aKey),
    iOneShot(aOneShot)
    {
    }



// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginCRWatcher::RunError
// Standard active object error function.
// -----------------------------------------------------------------------------
//

TInt CUSBClassChangeUIPluginCRWatcher::RunError(TInt /*aError*/)
    {
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher::RunError: ERROR %d \
        from RunL.") );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginCRWatcher::RunL
// This function will be called upon a change in the watched key.
// -----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginCRWatcher::RunL()
    {
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher::RunL"));

    iOwner.SettingChangedL( iKey );        
    if (!iOneShot)
        {
        StartListeningL(); 
        }

    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher::RunL complete."));
    }


// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginCRWatcher::ConstructL
// Construct member variable iPersonalityRepository that is to access the
// repository. Then start listening of the changes in the repository.
// -----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginCRWatcher::ConstructL()
    {
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher: ConstructL"));
    CActiveScheduler::Add(this);

    iPersonalityRepository = CRepository::NewL( KCRUidUsbWatcher );
    
    StartListeningL();
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher: ConstructL complete"));        
    }

// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginCRWatcher::StopListening
// Cancels notifications.
// -----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginCRWatcher::StopListening()
    {
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher: StopListening"));
    Cancel();
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher: StopListening complete"));
    }

// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginCRWatcher::StartListeningL
// Orders notifications and sets the object active.
// -----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginCRWatcher::StartListeningL()
    {
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher: StartListeningL"));
    if(IsActive())
        {
        return;   
        }

    User::LeaveIfError(iPersonalityRepository->NotifyRequest(
        iKey, 
        iStatus ) );
    SetActive();
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher: StartListening complete"));
    }


// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginCRWatcher::DoCancel
// Standard active object cancellation function.
// -----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginCRWatcher::DoCancel()
    {
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher::DoCancel"));
    
    if ( iPersonalityRepository )
        {
        // cancel the order for change notifications
        iPersonalityRepository->NotifyCancel( iKey );
        }
    FLOG(_L("[USBCLASSCHANGEUIPLUGIN]\tCUSBClassChangeUIPluginCRWatcher::DoCancel complete"));
    }


// End of file
