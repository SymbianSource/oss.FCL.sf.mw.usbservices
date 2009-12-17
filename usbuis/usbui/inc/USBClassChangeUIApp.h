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
* Description:  Declares main application class.
*
*/


#ifndef USBCLASSCHANGEUIAPP_H
#define USBCLASSCHANGEUIAPP_H

// INCLUDES
#include <aknapp.h>
#include "USBClassChangeUI.h"


// CONSTANTS

// CLASS DECLARATION

/**
* CUSBCLASSCHANGEUIAPP application class.
* Provides factory to create concrete document object.
*
*/
class CUSBClassChangeUIApp : public CAknApplication
    {
    private:

        /**
        * From CAknApplication, creates CUSBClassChangeUIDocument document
        * object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();

        /**
        * From CAknApplication, returns application's UID (KUidUSBUI).
        * @return The value of KUidUSBClassChangeUI.
        */
        TUid AppDllUid() const;
    };

#endif
// End of File
