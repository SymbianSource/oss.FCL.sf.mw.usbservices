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
* Description: 
*        Header file for CUSBClassChangeUIPluginInterface class.
*
*/

#ifndef USBCLASSCHANGEUIPLUGININTERFACE_H
#define USBCLASSCHANGEUIPLUGININTERFACE_H

// System includes
#include <GSPluginInterface.h>


/**
 * ECOM plug-in creation for the standalone application. 
 * The view class must inherit this class to be able to signal the
 * destruction of the interface implementation. If this implementation
 * was created in the GS application, the destruction must not be 
 * signalled, since the view framework does it. If this implementation
 * was created in the standalone USB UI application, the destruction
 * must be signalled, since otherwise there will be memory leaks. 
 * 
 * The inherited CGSPluginInterface destructor does not signal the
 * desctruction. Also, it cannot be signalled in the destructor of 
 * this class, since it would be called while deleting the view. 
 * This class stores the instance ID iDtor_ID_Key, because it can't
 * access the private member variable of class CGSPluginInterface.
 * 
 * Note that the implementation is inline, because the concrete 
 * plug-in has not been loaded. The implementation of this class can't
 * reside in the plug-in to be loaded by the ECOM framework. 
 * 
 */ 
class CUSBClassChangeUIPluginInterface	: public CGSPluginInterface
    {    
    public: // Constructors & destructors

        /**
         * Creates new USBClassChangeUI view plugin having the given UID.
         * Uses Leave code KErrNotFound, if implementation is not found.
         *
         * @param aImplementationUid Implementation UID of the plugin to be 
         *        created.
         * @param aInitParams The AppUi of the USBClassChangeUI application.
         * @return The new view. 
         */                                                 
        static CUSBClassChangeUIPluginInterface* NewL(
            const TUid aImplementationUid, 
            TAny* aInitParams);

        /**
         * Destructor
         */
        ~CUSBClassChangeUIPluginInterface();

        /**
         * Signal destruction of the interface implementation to ECOM.
         * This should not be called from the GS plugin, since the 
         * framework wants to do it itself. From the application, the
         * view must call this before destruction. 
         */
        void SignalDestruction();
            
    protected: 

        /**
         * Default constructor
         */    
        CUSBClassChangeUIPluginInterface();
        
    private: // Data

        /**
         * ECOM plugin instance UID. 
         * The inherited member variable with the same name is hidden, but
         * there is no way to access it. 
         */
        TUid iDtor_ID_Key;
    };


#include "USBClassChangeUIPluginInterface.inl"

#endif //USBCLASSCHANGEUIPLUGININTERFACE_H
//End of file
