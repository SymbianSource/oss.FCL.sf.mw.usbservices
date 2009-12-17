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
* Description:  Usb waiting notifier implementation
 *
*/


#include <e32std.h>
#include <usbuinotif.h>

#include "cusbnotifmanager.h"
#include "cusbwaitnotifier.h"

#include "debug.h"
#include "panic.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUsbWaitNotifier* CUsbWaitNotifier::NewL(RNotifier& aNotifier,
        CUsbNotifManager* aNotifManager, TUint aNotifId)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbWaitNotifier::NewL" ) );

    CUsbWaitNotifier* self = new (ELeave) CUsbWaitNotifier(aNotifier,
            aNotifManager, aNotifId);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CUsbWaitNotifier::~CUsbWaitNotifier()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbWaitNotifier::~CUsbWaitNotifier" ) );
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CUsbWaitNotifier::CUsbWaitNotifier(RNotifier& aNotifier,
        CUsbNotifManager* aNotifManager, TUint aNotifId) :
    CUsbNoteNotifier(aNotifier, aNotifManager, KUsbUiNotifOtgError, aNotifId)
    {
    SetFeedbackNeeded();
    }

// ---------------------------------------------------------------------------
// Second-phase constructor
// ---------------------------------------------------------------------------
//
void CUsbWaitNotifier::ConstructL()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbWaitNotifier::ConstructL" ) );

    CUsbNoteNotifier::ConstructL();
    }

// End of file
