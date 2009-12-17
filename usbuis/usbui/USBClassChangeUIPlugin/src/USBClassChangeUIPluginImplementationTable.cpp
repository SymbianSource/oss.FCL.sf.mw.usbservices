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
* Description:  ECOM proxy table for this plugin
*
*/


// System includes
#include <e32std.h>
#include <implementationproxy.h>

// User includes
#include "USBClassChangeUIPluginView.h"

const TImplementationProxy KUSBClassChangeUIPluginImplementationTable[] = 
    {
    // Uid for plugin implementation for General Settings application:
    IMPLEMENTATION_PROXY_ENTRY( 0x1020E473,	CUSBClassChangeUIPluginView::NewPluginL ),
    // Uid for plugin implementation for USBClassChangeUIAppUi:
    IMPLEMENTATION_PROXY_ENTRY( 0x1027478F,	CUSBClassChangeUIPluginView::NewL )
    };

// ---------------------------------------------------------------------------
// ImplementationGroupProxy
// Gate/factory function
//
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof(KUSBClassChangeUIPluginImplementationTable) 
        / sizeof(TImplementationProxy);
    return KUSBClassChangeUIPluginImplementationTable;
    }


// End of file
