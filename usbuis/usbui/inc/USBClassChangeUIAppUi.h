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
* Description:  Declares UI class for application.
*
*/


#ifndef USBCLASSCHANGEUIAPPUI_H
#define USBCLASSCHANGEUIAPPUI_H

// INCLUDES
#include <aknappui.h>
#include <aknViewAppUi.h>


// FORWARD DECLARATIONS
class CUSBClassChangeUIView;
class CUSBClassChangeUIModel;

// CLASS DECLARATION

/**
*  AppUI class.
*
*  @lib 
*  @since Series 60 3.0
*/
class CUSBClassChangeUIAppUi : public CAknViewAppUi
    {
    public: // Constructors and destructor

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();

        /**
        * Destructor
        */
        virtual ~CUSBClassChangeUIAppUi();
        
    public: // Functions from base classes

        /**
        * From CAknViewAppUi Handles user commands.
        *
        * @param aCommand A command id.        
        * @return None.
        */
        void HandleCommandL(TInt aCommand);

    public: // New functions
		
     
        
    private: // New functions
    
        /**
        * Handles the "No USB mode active" state in startup.
        * @since Series 60 3.0
        * @param None.
        * @return None.
        */
        void NoUSBModeActiveL();
        
    private: // Data
    };

#endif

// End of File
