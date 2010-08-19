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

#include "UsbPersonalityPluginActiveObject.h"

CUsbPersonalityPluginActiveObject::CUsbPersonalityPluginActiveObject( CUsbPersonalityPlugin* aPlugin, CStifLogger* aLog, TUsbPersonalityParams* aParams) : CActive( EPriorityStandard ) // Standard priority
    {
    iPlugin = aPlugin;
    iLog = aLog;
    iParams = aParams;
    }

CUsbPersonalityPluginActiveObject* CUsbPersonalityPluginActiveObject::NewLC(CUsbPersonalityPlugin* aPlugin, CStifLogger* aLog, TUsbPersonalityParams* aParams)
    {
    CUsbPersonalityPluginActiveObject* self =
            new (ELeave) CUsbPersonalityPluginActiveObject( aPlugin, aLog, aParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CUsbPersonalityPluginActiveObject* CUsbPersonalityPluginActiveObject::NewL(CUsbPersonalityPlugin* aPlugin, CStifLogger* aLog, TUsbPersonalityParams* aParams)
    {
    CUsbPersonalityPluginActiveObject* self =
            CUsbPersonalityPluginActiveObject::NewLC( aPlugin, aLog, aParams);
    CleanupStack::Pop(); // self;
    return self;
    }

void CUsbPersonalityPluginActiveObject::ConstructL()
    {
    CActiveScheduler::Add(this); // Add to scheduler
    }

CUsbPersonalityPluginActiveObject::~CUsbPersonalityPluginActiveObject()
    {
    Cancel(); // Cancel any request, if outstanding
    // Delete instance variables if any
    }

void CUsbPersonalityPluginActiveObject::DoCancel()
    {
    }

void CUsbPersonalityPluginActiveObject::RunL()
    {
    CActiveScheduler::Stop();
    }

TInt CUsbPersonalityPluginActiveObject::RunError(TInt aError)
    {
    return aError;
    }

void CUsbPersonalityPluginActiveObject::PreparePersonalityStart( TRequestStatus& aStatus)
    {
    iPlugin->PreparePersonalityStart(aStatus);
    RDebug::Printf( "prepare personality start, status: %d", aStatus.Int());
    
    SetActive();
    CActiveScheduler::Start();
    }

void CUsbPersonalityPluginActiveObject::FinishPersonalityStart( TRequestStatus& aStatus)
    {
    iPlugin->FinishPersonalityStart(aStatus);
    RDebug::Printf( "finish personality start, status: %d", aStatus.Int());
    
    SetActive();
    CActiveScheduler::Start();
    }

void CUsbPersonalityPluginActiveObject::PreparePersonalityStop( TRequestStatus& aStatus)
    {
    iPlugin->PreparePersonalityStop(aStatus);
    RDebug::Printf( "prepare personality start, status: %d", aStatus.Int());
    
    SetActive();
    CActiveScheduler::Start();
    }

void CUsbPersonalityPluginActiveObject::FinishPersonalityStop( TRequestStatus& aStatus)
    {
    iPlugin->FinishPersonalityStop(aStatus);
    RDebug::Printf( "finish personality start, status: %d", aStatus.Int());
    
    SetActive();
    CActiveScheduler::Start();
    }

void CUsbPersonalityPluginActiveObject::ConfirmPersonalityUnload( TRequestStatus& aStatus)
    {
    iPlugin->ConfirmPersonalityUnload(aStatus);
    RDebug::Printf( "confirm personality unload, status: %d", aStatus.Int());
    
    iParams->PersonalityNotifier().CancelQuery(KQueriesNotifier);
    SetActive();
    CActiveScheduler::Start();
    }
