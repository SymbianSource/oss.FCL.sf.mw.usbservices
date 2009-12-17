/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "USBObexApiTestActiveObject.h"

// -----------------------------------------------------------------------------
// CUSBObexApiTestActiveObject::CUSBObexApiTestActiveObject
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUSBObexApiTestActiveObject::CUSBObexApiTestActiveObject( CStifLogger* aLog,
        CUsbClassControllerPlugIn* aUsbClassControllerPlugIn ) :
    CActive( EPriorityStandard )
    {
    iUsbClassControllerPlugIn = aUsbClassControllerPlugIn;
    iLog = aLog;
    }

// Destructor
CUSBObexApiTestActiveObject::~CUSBObexApiTestActiveObject()
    {
    Cancel();
    }

// ---------------------------------------------------------------------------
// CUSBObexApiTestActiveObject::RunL
// ---------------------------------------------------------------------------
//
void CUSBObexApiTestActiveObject::RunL()
    {
    iLog->Log( _L("ActiveTestObject::RunL"));
    CActiveScheduler::Stop();
    iLog->Log( _L("ActiveScheduler Stopped"));
    iUsbServiceState = iUsbClassControllerPlugIn -> State();
    }

// -----------------------------------------------------------------------------
// CUSBObexApiTestActiveObject::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUSBObexApiTestActiveObject* CUSBObexApiTestActiveObject::NewL(
        CStifLogger* aLog, CUsbClassControllerPlugIn* aUsbClassControllerPlugIn )
    {
    CUSBObexApiTestActiveObject* self = new ( ELeave )CUSBObexApiTestActiveObject( aLog, aUsbClassControllerPlugIn );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CUSBObexApiTestActiveObject::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUSBObexApiTestActiveObject::ConstructL()
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CUSBObexApiTestActiveObject::DoCancel
// ---------------------------------------------------------------------------
//
void CUSBObexApiTestActiveObject::DoCancel()
    {
    iLog->Log( _L("ActiveTestObject::DoCancel"));
    }

// ---------------------------------------------------------------------------
// CUSBObexApiTestActiveObject::Start
// ---------------------------------------------------------------------------
//
void CUSBObexApiTestActiveObject::Start()
    {
    iUsbClassControllerPlugIn -> Start( iStatus );
    SetActive();
    iLog->Log( _L("Obex Controller Started"));
    }

// ---------------------------------------------------------------------------
// CUSBObexApiTestActiveObject::Stop
// ---------------------------------------------------------------------------
//
void CUSBObexApiTestActiveObject::Stop()
    {
    iUsbClassControllerPlugIn -> Stop( iStatus );
    SetActive();
    iLog->Log( _L("Obex Controller Stopped"));
    }

