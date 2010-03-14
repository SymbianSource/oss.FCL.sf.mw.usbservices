/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation
 *
*/


#include "cusbtimer.h"

#include "debug.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbTimer::CUsbTimer(MUsbTimerObserver* aObserver, TUsbTimerId aTimerId) :
    CActive(CActive::EPriorityStandard), iObserver(aObserver), iTimerId(
            aTimerId)
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbTimer::~CUsbTimer()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbTimer::~CUsbTimer" ) );
    Cancel();
    iTimer.Close();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbTimer::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbTimer::ConstructL" ) );
    User::LeaveIfError(iTimer.CreateLocal());
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbTimer* CUsbTimer::NewL(MUsbTimerObserver* anObserver,
        TUsbTimerId aTimerId)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbTimer::NewL" ) );

    CUsbTimer* self = new (ELeave) CUsbTimer(anObserver, aTimerId);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self); // pop self
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbTimer::After(TInt aMilliseconds)
    {
//        FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbTimer::After aMilliseconds %d, timerId=%d" ), aMilliseconds, iTimerId))

    if (IsActive()) 
        {
        Cancel();
        }

    // RunL will be called after KInactiveTimeForShutDown milliseconds
    iTimer.After(iStatus, TTimeIntervalMicroSeconds32(aMilliseconds));
    SetActive();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbTimer::RunL()
    {

    if(KErrNone != iStatus.Int())
        {
        FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbTimer::RunL iStatus %d" ), iStatus.Int()));
        User::Leave(iStatus.Int());
        }

    iObserver->TimerElapsedL(iTimerId);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbTimer::RunError(TInt aError)
    {
        FTRACE(FPrint(_L( "[USBOTGWATCHER]\tCUsbTimer::RunError aError %d" ), aError ));

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbTimer::DoCancel()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbTimer::DoCancel" ) )
    iTimer.Cancel();
    }
