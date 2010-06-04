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
* Description:  This is the implementation of application class.
*
*/


// INCLUDE FILES
#include    "USBClassChangeUIApp.h"
#include    "USBClassChangeUIDocument.h"
#include    <eikstart.h>


// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CUSBClassChangeUIApp::AppDllUid()
// Returns application UID
// ----------------------------------------------------------------------------
//
TUid CUSBClassChangeUIApp::AppDllUid() const
    {
    return KUidUSBClassChangeUI;
    }
   
// ----------------------------------------------------------------------------
// CUSBClassChangeUIApp::CreateDocumentL()
// Creates CUSBClassChangeUIDocument object
// ----------------------------------------------------------------------------
//
CApaDocument* CUSBClassChangeUIApp::CreateDocumentL()
    {
    CUSBClassChangeUIDocument* doc = 
        new ( ELeave ) CUSBClassChangeUIDocument( *this );
    return doc; 
    
    }
    
// ================= OTHER EXPORTED FUNCTIONS =================================   

LOCAL_C CApaApplication* NewApplication()
    {
    return new CUSBClassChangeUIApp;
    }

GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication( NewApplication );
    }

// End of File
