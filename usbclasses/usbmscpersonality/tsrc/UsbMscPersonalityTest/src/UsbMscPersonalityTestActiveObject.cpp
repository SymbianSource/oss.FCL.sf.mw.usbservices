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
#include "UsbMscPersonalityTestActiveObject.h"
#include <e32debug.h>

CUsbMscPersonalityTestActiveObject::CUsbMscPersonalityTestActiveObject( CUsbPersonalityPlugin* aPlugin, CStifLogger* aLog, TUsbPersonalityParams* aParams ) :
    CActive(EPriorityStandard) // Standard priority
    {
    iPlugin = aPlugin;
    iLog = aLog;
    iParams = aParams;
    }

CUsbMscPersonalityTestActiveObject* CUsbMscPersonalityTestActiveObject::NewLC( CUsbPersonalityPlugin* aPlugin, CStifLogger* aLog, TUsbPersonalityParams* aParams )
    {
    CUsbMscPersonalityTestActiveObject* self =
            new (ELeave) CUsbMscPersonalityTestActiveObject( aPlugin, aLog, aParams );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CUsbMscPersonalityTestActiveObject* CUsbMscPersonalityTestActiveObject::NewL( CUsbPersonalityPlugin* aPlugin, CStifLogger* aLog, TUsbPersonalityParams* aParams )
    {
    CUsbMscPersonalityTestActiveObject* self =
            CUsbMscPersonalityTestActiveObject::NewLC( aPlugin, aLog, aParams );
    CleanupStack::Pop(); // self;
    return self;
    }

void CUsbMscPersonalityTestActiveObject::ConstructL()
    {
    CActiveScheduler::Add(this); // Add to scheduler
    }

CUsbMscPersonalityTestActiveObject::~CUsbMscPersonalityTestActiveObject()
    {
    Cancel(); // Cancel any request, if outstanding
    
    // Delete instance variables if any
    }

void CUsbMscPersonalityTestActiveObject::DoCancel()
    {
    }

void CUsbMscPersonalityTestActiveObject::RunL()
    {
    CActiveScheduler::Stop();
    }

TInt CUsbMscPersonalityTestActiveObject::RunError(TInt aError)
    {
    return aError;
    }

void CUsbMscPersonalityTestActiveObject::PreparePersonalityStart( TRequestStatus& aStatus)
    {
    iPlugin->PreparePersonalityStart(aStatus);
    RDebug::Printf( "prepare personality start, status: %d", aStatus.Int());
    
    SetActive();
    CActiveScheduler::Start();
    }

void CUsbMscPersonalityTestActiveObject::FinishPersonalityStart( TRequestStatus& aStatus)
    {
    iPlugin->FinishPersonalityStart(aStatus);
    RDebug::Printf( "finish personality start, status: %d", aStatus.Int());
    
    SetActive();
    CActiveScheduler::Start();
    }

void CUsbMscPersonalityTestActiveObject::PreparePersonalityStop( TRequestStatus& aStatus)
    {
    iPlugin->PreparePersonalityStop(aStatus);
    RDebug::Printf( "prepare personality start, status: %d", aStatus.Int());
    
    SetActive();
    CActiveScheduler::Start();
    }

void CUsbMscPersonalityTestActiveObject::FinishPersonalityStop( TRequestStatus& aStatus)
    {
    iPlugin->FinishPersonalityStop(aStatus);
    RDebug::Printf( "finish personality start, status: %d", aStatus.Int());
    
    SetActive();
    CActiveScheduler::Start();
    }

void CUsbMscPersonalityTestActiveObject::ConfirmPersonalityUnload( TRequestStatus& aStatus)
    {
    iPlugin->ConfirmPersonalityUnload(aStatus);
    RDebug::Printf( "confirm personality unload, status: %d", aStatus.Int());
    
    iParams->PersonalityNotifier().CancelQuery(KQueriesNotifier);
    SetActive();
    CActiveScheduler::Start();
    }
