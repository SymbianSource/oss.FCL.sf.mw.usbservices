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
* Description:  This class handles the definition of ECom plugin interface 
*                implementation. 
*
*/


#include "usblcdplugin.h"

#ifdef __FLOG_ACTIVE
_LIT8(KLogComponent, "USBLcdPlugin");
#endif

CUsbLcdPlugin* CUsbLcdPlugin::NewL(TLocodBearerPluginParams& aParams)
    {
    LOG_STATIC_FUNC_ENTRY
    CUsbLcdPlugin* self = new (ELeave) CUsbLcdPlugin(aParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CUsbLcdPlugin::~CUsbLcdPlugin()
    {
    LOG_FUNC
    delete iUsbLcdActive;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CUsbLcdPlugin::CUsbLcdPlugin(TLocodBearerPluginParams& aParams)
    : CLocodBearerPlugin(aParams)
    {
    LOG_FUNC
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CUsbLcdPlugin::ConstructL()
    {
    LOG_FUNC
    iUsbLcdActive = CUsbLcdActive::NewL(Observer()); 
    }
// End of file
