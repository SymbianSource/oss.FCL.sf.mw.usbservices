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
* Description:  Usb warning notifier implementation
 *
*/


#include <e32std.h>
#include <usbuinotif.h>

#include "cusbnotifmanager.h"
#include "cusbnotifier.h"
#include "cusbwarningnotifier.h"

#include "definitions.h"
#include "debug.h"
#include "panic.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUsbWarningNotifier* CUsbWarningNotifier::NewL(RNotifier& aNotifier,
        CUsbNotifManager* aNotifManager, TUint aNotifId)
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbWarningNotifier::NewL aNotifId = %d" ), aNotifId));

    CUsbWarningNotifier* self = new (ELeave) CUsbWarningNotifier(aNotifier,
            aNotifManager, aNotifId);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CUsbWarningNotifier::~CUsbWarningNotifier()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbWarningNotifier::~CUsbWarningNotifier" ) );
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CUsbWarningNotifier::CUsbWarningNotifier(RNotifier& aNotifier,
        CUsbNotifManager* aNotifManager, TUint aNotifId) :
    CUsbNoteNotifier(aNotifier, aNotifManager, KUsbUiNotifOtgWarning,
            aNotifId)
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbWarningNotifier::CUsbWarningNotifier" ) );
    }

// -----------------------------------------------------------------------------
// Second-phase constructor
// -----------------------------------------------------------------------------
//
void CUsbWarningNotifier::ConstructL()
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbWarningNotifier::ConstructL" ) );

    CUsbNoteNotifier::ConstructL();
    }

// End of file
