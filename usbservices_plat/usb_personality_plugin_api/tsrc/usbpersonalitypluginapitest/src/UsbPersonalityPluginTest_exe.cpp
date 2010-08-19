/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This material, including documentation and any related computer
* programs, is protected by copyright controlled by Nokia. All
* rights are reserved. Copying, including reproducing, storing
* adapting or translating, any or all of this material requires the
* prior written consent of Nokia. This material also contains
* confidential information which may not be disclosed to others
* without the prior written consent of Nokia.
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/

// INCLUDE FILES
#include <e32base.h>
#include <e32svr.h>
#include <StifTestInterface.h>
// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// ================= OTHER EXPORTED FUNCTIONS =================================

/*
-------------------------------------------------------------------------------

    Class: -

    Method: E32Main

    Description: 

    Parameters: None

    Return Values: TInt: Symbian error code

    Errors/Exceptions: None

    Status: Draft

-------------------------------------------------------------------------------
*/
GLDEF_C TInt E32Main()
    {
    _LIT( KProcessMsgStart, "New process starting" );
    RDebug::Print( KProcessMsgStart );


    // This starts a new session that get capabilites that is used in 
    // usbpersonalityplugintest_exe.mmp file.
    TInt r = StartSession();

    _LIT( KProcessMsgEnd, "New process ends" );
    RDebug::Print( KProcessMsgEnd );

    return r;

    }

// End of File
