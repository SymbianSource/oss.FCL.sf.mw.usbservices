/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

#include "DismountClient.h"

CDismountClient::CDismountClient() :
    CActive(EPriorityStandard) // Standard priority
    {
    }

CDismountClient* CDismountClient::NewLC(CStifLogger *aLog)
    {
    CDismountClient* self = new (ELeave) CDismountClient();
    CleanupStack::PushL(self);
    self->ConstructL(aLog);
    return self;
    }

CDismountClient* CDismountClient::NewL(CStifLogger *aLog)
    {
    CDismountClient* self = CDismountClient::NewLC(aLog);
    CleanupStack::Pop(); // self;
    return self;
    }

void CDismountClient::ConstructL(CStifLogger *aLog)
    {
    User::LeaveIfError(iTimer.CreateLocal());
    iLog = aLog;
    User::LeaveIfError( iFs.Connect() );
    
    CActiveScheduler::Add(this); // Add to scheduler
    }

CDismountClient::~CDismountClient()
    {
    Cancel(); // Cancel any request, if outstanding
    // Delete instance variables if any
    iTimer.Close();
    iFs.NotifyDismountCancel();
    iFs.Close();
    }

void CDismountClient::DoCancel()
    {
    iTimer.Cancel();
    iFs.NotifyDismountCancel();
    }

void CDismountClient::StartL(TAllowDismountOption aAllowDismountOption)
    {
    Cancel();
    
    iAllowDismountOption = aAllowDismountOption;
    if (iAllowDismountOption == EAllowDismountTooLate || 
            iAllowDismountOption == EAllowDismountAfter2Sec)
        {
        iState = EDelay;
        }
    
    iFs.NotifyDismount(EDriveE,iStatus,EFsDismountRegisterClient);
    RDebug::Printf( " DiscmountClient StartL ");
    iFs.NotifyDismountCancel();
    SetActive();
    }

void CDismountClient::RunL()
    {
    
    switch (iAllowDismountOption)
        {
        case EAllowDismount:
            iFs.AllowDismount(EDriveE);
            break;
            
        case EAllowDismountAfter2Sec:
            if (iState ==EDelay)
                {
                iTimer.After(iStatus,(TTimeIntervalMicroSeconds32)2000000);
                iState=EWaitForDelay;
                SetActive();
                }
            else
                if(iState==EWaitForDelay)
                    {
                    iFs.AllowDismount(EDriveE);
                    iState=EDelay;
                    }
            break;
            
        case EAllowDismountTooLate:
            if (iState == EDelay)
                {
            iTimer.After(iStatus,(TTimeIntervalMicroSeconds32)8000000);
            iState=EWaitForDelay;
            SetActive();
                }
            else
                if(iState==EWaitForDelay)
                    {
                    iFs.AllowDismount(EDriveE);
                    iState=EDelay;
                    }
            break;
            
        case ENotAllowDismount:
        default:
            break;
        }
    }

TInt CDismountClient::RunError(TInt aError)
    {
    return aError;
    }
