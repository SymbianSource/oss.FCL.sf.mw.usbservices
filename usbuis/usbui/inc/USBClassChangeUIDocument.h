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
* Description:  Declares document for application.
*
*/


#ifndef USBCLASSCHANGEUIDOCUMENT_H
#define USBCLASSCHANGEUIDOCUMENT_H

// INCLUDES
#include <AknDoc.h>

// CONSTANTS

// FORWARD DECLARATIONS

class CEikAppUi;
class CUSBClassChangeUIModel;

// CLASS DECLARATION

/**
*  This class is a base class mandatory for all Symbian OS UI applications.
*
*  @lib
*  @since Series 60 3.0
*/
class CUSBClassChangeUIDocument : public CAknDocument
    {
    public: // Constructors and destructor

		
	    /**
        * C++ default constructor.
        */
        CUSBClassChangeUIDocument( 
            CEikApplication& aApp ): CAknDocument( aApp ){ }
		
		
	    /**
        * Destructor.
        */
        virtual ~CUSBClassChangeUIDocument();
        
    public:		// New functions
	
	private: // Functions from base classes

	    /**
        * From CEikDocument, to create CUSBClassChangeUiAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();
	
	private:
	
        
	private:	// Data	
    };

#endif

// End of File
