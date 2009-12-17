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

#include <usbuinotif.h> 

#include "cusbnotifmanager.h"
#include "cusbwaitnotifier.h"
#include "cusbwarningnotifier.h"
#include "cusbindicatornotifier.h"

#include "debug.h"
#include "panic.h"

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CWaitNotifierInfo::CWaitNotifierInfo(CUsbNotifier* aWaitNotifier,
        MWaitNotifierObserver* aObserver) :
    iWaitNotifier(aWaitNotifier), iObserver(aObserver)
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CWaitNotifierInfo::ConstructL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCWaitNotifierInfo::ConstructL" ) );
    // owenrship for iWaitNotifier transferred in default constructor.
    // this object is responsible for deletion of the iWaitNotifier then	
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CWaitNotifierInfo* CWaitNotifierInfo::NewL(CUsbNotifier* aWaitNotifier,
        MWaitNotifierObserver* aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCWaitNotifierInfo::NewL" ) );
    CWaitNotifierInfo* self = new (ELeave) CWaitNotifierInfo(aWaitNotifier,
            aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CWaitNotifierInfo::~CWaitNotifierInfo()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCWaitNotifierInfo::~CWaitNotifierInfo" ) );
    delete iWaitNotifier;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbNotifier* CWaitNotifierInfo::WaitNotifier() const
    {
    return iWaitNotifier;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
MWaitNotifierObserver* CWaitNotifierInfo::Observer() const
    {
    return iObserver;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbNotifManager::CUsbNotifManager()
    {
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbNotifManager::ConstructL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbNotifManager::ConstructL" ) );

    User::LeaveIfError(iNotifier.Connect());

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbNotifManager* CUsbNotifManager::NewL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbNotifManager::NewL" ) );

    CUsbNotifManager* self = new (ELeave) CUsbNotifManager();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbNotifManager::~CUsbNotifManager()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbNotifManager::~CUsbNotifManager" ) );

    CloseAllNotifiers();

    iNotifier.Close();

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbNotifManager::ShowNotifierL(TUid aCat, TUint aNotifId,
        MWaitNotifierObserver* aObserver)
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbNotifManager::ShowNotifier aCat = 0x%X aNotifId = %d, aObserver=0x%X" ),
                        aCat, aNotifId, aObserver));

    // remove non-feedback notifiers from the list
    for (TUint i(0); i < iWaitNotifiers.Count(); ++i)
        {
        CWaitNotifierInfo* waitNotifier = iWaitNotifiers[i];
        if (!(waitNotifier->WaitNotifier()->IsFeedbackNeeded()))
            {
            delete waitNotifier;
            iWaitNotifiers.Remove(i);
            }
        }
    //If not suspened a short while between closing a present note and showing a new one,
    //error - CUsbWaitNotifier::RunL iStatus = 4 - will happen. Necessary here to pause a while 
    //to allow notfier framework to do some cleaning work.
    //Should find another way to solve this problem.
    const TUint KTenthOfASecond = 100000;
    User::After(TTimeIntervalMicroSeconds32(KTenthOfASecond));

    // can not have switch-case selector here, due to constants are of type Uids
    // notifier requires feedback => create CWaitNotifier for it
    // As notifiers can be more than two types such as warning, error, indicator ...
    // to create concret notifiers according to categories instead of checking aObserver null
    CUsbNotifier* notifier(NULL);
    if (aCat == KUsbUiNotifOtgError)
        {
        __ASSERT_ALWAYS(aObserver != NULL, Panic(EWrongNotifierCategory));
        notifier = CUsbWaitNotifier::NewL(iNotifier, this, aNotifId);
        }
    else if (aCat == KUsbUiNotifOtgWarning)
        {
        notifier = CUsbWarningNotifier::NewL(iNotifier, this, aNotifId);
        }
    else
        {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbNotifManager::ShowNotifierL - Unexpected aCat" ) );
        Panic(EWrongNotifierCategory);
        }

    CleanupStack::PushL(notifier);
    
    iWaitNotifiers.AppendL(CWaitNotifierInfo::NewL(notifier, aObserver));

        FLOG( _L( "[USBOTGWATCHER]\tCUsbNotifManager::ShowNotifierL - Start to show note" ) );
    notifier->ShowL();

    CleanupStack::Pop(notifier);    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbNotifManager::BlinkIndicatorL(TBool aBlinking)
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbNotifManager::BlinkIndicatorL, aBlinking=%d" ), aBlinking));

    if (!iIndicatorNotifier)
        iIndicatorNotifier = CUsbIndicatorNotifier::NewL(this);

    if (aBlinking)
        iIndicatorNotifier->ShowL();
    else
        iIndicatorNotifier->Close();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbNotifManager::ShowIndicatorL(TBool aVisible)
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbNotifManager::ShowIndicatorL, aVisible=%d" ), aVisible ));

    if (!iIndicatorNotifier)
        iIndicatorNotifier = CUsbIndicatorNotifier::NewL(this);

    iIndicatorNotifier->ShowIndicatorL(aVisible);
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CUsbNotifManager::NotifierShowCompletedL(CUsbNotifier* aWaitNotifier,
        TInt aResult, TInt aFeedback)
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbNotifManager::NotifierShowCompleted aResult = %d" ), aResult));

    // remove Notifier from the list
    for (TUint i(0); i < iWaitNotifiers.Count(); ++i)
        {
        if (aWaitNotifier == iWaitNotifiers[i]->WaitNotifier())
            {
            MWaitNotifierObserver* observer = iWaitNotifiers[i]->Observer();

            delete iWaitNotifiers[i];
            iWaitNotifiers.Remove(i);

            if (observer)
                observer->WaitNotifierCompletedL(aFeedback);
            }
        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbNotifManager::CloseAllNotifiers()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbNotifManager::CloseAllNotifiers" ) );

    iWaitNotifiers.ResetAndDestroy();
    delete iIndicatorNotifier;
    iIndicatorNotifier = NULL;
    }
