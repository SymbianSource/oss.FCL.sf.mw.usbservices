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
* Description:  This is the implementation of document class.
*
*/


// INCLUDE FILES
#include "USBClassChangeUIDocument.h"
#include "USBClassChangeUIAppUi.h"
#include "USBClassChangeUIDebug.h"

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CUSBClassChangeUIDocument::~CUSBClassChangeUIDocument()
    {  
    FLOG(_L("[CUSBClassChangeUI]\t CUSBClassChangeUIDocument::Destructor (empty) completed"));
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIDocument::CreateAppUiL()
// Constructs CUSBClassChangeUIAppUi
// ----------------------------------------------------------------------------
//
CEikAppUi* CUSBClassChangeUIDocument::CreateAppUiL()
    {
    FLOG(_L("[CUSBClassChangeUI]\t CUSBClassChangeUIDocument::CreateAppUiL()"));
    
    return new ( ELeave ) CUSBClassChangeUIAppUi;
    }

// End of File  
