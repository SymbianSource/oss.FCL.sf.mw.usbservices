/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active object class used to manage asynchronous request.
*
*/


#include <e32property.h>
#include "usblcdactive.h"
#include <UsbWatcherInternalPSKeys.h>
#include <usbpersonalityids.h>

#ifdef __FLOG_ACTIVE
_LIT8(KLogComponent, "USBLcdPlugin");
#endif

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructs a CUsbLcdActive object.
// ---------------------------------------------------------------------------
//
CUsbLcdActive* CUsbLcdActive::NewL(MLocodBearerPluginObserver& aObserver)
    {
    LOG_STATIC_FUNC_ENTRY
    CUsbLcdActive* self = new (ELeave) CUsbLcdActive(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CUsbLcdActive::~CUsbLcdActive()
    {
    LOG_FUNC
    Cancel();
    iProperty.Close();
    }


// ---------------------------------------------------------------------------
// From class CActive
// ---------------------------------------------------------------------------
//
void CUsbLcdActive::DoCancel()
    {
    iProperty.Cancel();
    }

// ---------------------------------------------------------------------------
// RunL() From class CActive
// ---------------------------------------------------------------------------
//
   
void CUsbLcdActive::RunL()
    {
    LOG_FUNC
    TInt value;
    
    if( iStatus != KErrNone )
       {
       LOGTEXT2(_L8("CUsbLcdActive::RunL() error = %d"),iStatus.Int());
       }
    else
       {

        TInt ret = RProperty::Get(KPSUidUsbWatcher, KUsbWatcherSelectedPersonality, value);
        LOGTEXT2(_L8("CUsbLcdActive::RunL() Personality = %d"), value);  
        iProperty.Subscribe(iStatus);
        SetActive();
        if (ret == KErrNone)
        { 
        switch (value)
            {
            case KUsbPersonalityIdPCSuite:
            case KUsbPersonalityIdPCSuiteMTP:
                NotifyLcd(ETrue);
                LOGTEXT(_L8("CUsbLcdActive::RunL() in PC Suite mode"));
                break;
               
            default:
                NotifyLcd(EFalse);
                LOGTEXT(_L8("CUsbLcdActive::RunL() mode is not PC Suite"));
                break;
            }
        }    
       }
    }

// ---------------------------------------------------------------------------
// From class CActive
// ---------------------------------------------------------------------------
//
    
TInt CUsbLcdActive::RunError( TInt /*aError*/ )
    {
    LOG_FUNC
    return KErrNone; 
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CUsbLcdActive::CUsbLcdActive(MLocodBearerPluginObserver& aObserver)
    : CActive(EPriorityStandard), iObserver(aObserver)
    {
    LOG_FUNC
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// Method to perform second phase construction.
// ---------------------------------------------------------------------------
//
void CUsbLcdActive::ConstructL()
    {  
    LOG_FUNC
    User::LeaveIfError(iProperty.Attach(KPSUidUsbWatcher, KUsbWatcherSelectedPersonality));
    Start();
    }

// ---------------------------------------------------------------------------
// Start() function is called to initiate RProperty subscription
// ---------------------------------------------------------------------------
//
void CUsbLcdActive::Start()
    {     
    LOG_FUNC
    iProperty.Subscribe(iStatus);
    SetActive();
    }
     
// ---------------------------------------------------------------------------
// This function notifying LoCoD framewark about active USB personality
// If Personality == 113 (PC Suite), then ETrue; in all other cases is EFalse 
// ---------------------------------------------------------------------------
//
void CUsbLcdActive::NotifyLcd(TBool aUsbStatus)
    {
    LOG_FUNC
    iObserver.NotifyBearerStatus(ELocodBearerUSB, aUsbStatus);
    }
    
//End of file
