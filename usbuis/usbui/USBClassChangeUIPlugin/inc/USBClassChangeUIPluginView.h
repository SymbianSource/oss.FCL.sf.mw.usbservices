/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declares container control for application.
*
*/


#ifndef USBCLASSCHANGEUIPLUGINVIEW_H
#define USBCLASSCHANGEUIPLUGINVIEW_H

// INCLUDES

#include <aknview.h>            // AVKON components
#include <AknQueryDialog.h>     // AVKON component
#include <eikmenup.h>           // Menu pane definitions
#include <GSPluginInterface.h>  // General Settings plugin
#include <ConeResLoader.h>
#include <data_caging_path_literals.hrh> 

#include "USBClassChangeUIPluginModel.h"   
#include "USBClassChangeUIPluginInterface.h"

// CONSTANTS
/** implementation uid */
const TUid KGsUSBUIPluginUid = { 0x1020E472 };

_LIT(KFileDrive,"z:");
_LIT(KResourceFileName, "USBClassChangeUIPlugin.rsc");
_LIT(KUSBUIconFileName, "usbui.mif");

/** USB modes are listed in the setting page */
const TInt KSettingPageItemsGranularity = 3;

// FORWARD DECLARATIONS
class CUSBClassChangeUIPluginContainer;

// CLASS DECLARATION
 
/**
* Main view of the application. Handles view activation, 
* deactivation, commands and dynamic options menus.
*/
class CUSBClassChangeUIPluginView :
    public CUSBClassChangeUIPluginInterface,
    public MUSBUIObserver
    {
    public: // Constructors and destructor
        	
        /**
        * Entry point for General Settings
        */	
        static CUSBClassChangeUIPluginView* NewPluginL();
        
        /**
        * Entry point for USBClassChangeUIAppUi
        */	
        static CUSBClassChangeUIPluginView* NewL();
        
    	/**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();

		/**
        * Destructor.
        */
        virtual ~CUSBClassChangeUIPluginView();
        
    public: // Functions from base classes

        /**
        * From CAknView Returns view id.
        * @param None.
        * @return View id.
        */
        TUid Id() const;
		
		/**
        * From CAknView Handles user commands.
        * @param aCommand A command id.        
        * @return None.
        */
        void HandleCommandL( TInt aCommand );
        
        /**
        * From CGSPluginInterface
        * Method for getting caption of this plugin. This should be the 
        * localized name of the settings view to be shown in parent view.
        *
        * @param aCaption pointer to Caption variable
        */
        void GetCaptionL( TDes& aCaption ) const;
        
        /**
        * From CGSPluginInterface.
        * Function for getting plugin's value for a certain key.
        *
        * @param aKey Key for the value to be retrieved.
        * @parem aValue Value for the given gey in TDes format.
        */
        void GetValue(const TGSPluginValueKeys aKey, TDes& aValue );

        /**
        * From CGSPluginInterface
        * Creates a new icon of desired type.
        *
        * @param  aIconType UID Icon type UID of the icon to be created.
        * @return Pointer of the icon. NOTE: Ownership of this icon is
        *         transferred to the caller.
        */
        CGulIcon* CreateIconL( const TUid aIconType );
   
        /**
        * Initialize options menu.
        * Remove help option, if the feature is not supported.
        * @param aResId Resource Id
        * @param aMenu The menu pane.
        */
        void DynInitMenuPaneL( TInt aResId, CEikMenuPane* aMenu );
        
        /**
        * From MUSBUIObserver.
        * Informs the observer that a setting has been changed. 
        */
        void SettingChanged();

        /**
         * From MUSBUIObserver/MUSBDeviceStateObserver.
         * Informs the observer that USB device state has changed.
         * @param aState current device state.
         */
        void DeviceStateChanged(TUsbDeviceState aPrevState,
                                TUsbDeviceState aState);

        /**
         * From MUSBUIObserver/MUSBOtgHostStateObserver.
         * @see MUSBOtgHostStateObserver::OtgHostIdPinStateChanged
         */
        void OtgHostIdPinStateChanged(TBool aIsIdPinOn);

        /**
         * From MUSBOtgHostStateObserver
         * @see MUSBOtgHostStateObserver::OtgHostIdPinStateChanged
         */
        void HostEventNotify(TDeviceEventInformation aEventInfo);
        
        // New functions
        
        /**
        * Gets the USB mode (from model)
        * @param None.
        * @return the value of the personality ID
        */
        TInt USBMode() const;
        
        /**
        * Sets the USB mode (through model)
        * @param The value of the personality ID.
        * @return None.
        */
        void SetUSBModeL( TInt );
                          
        /**
         * Checks whether USB device is connected.
         */
        TBool IsUSBDeviceConnected();

     private: 
     
        
         /**
         * Updates parent view if we are loaded by GS plugin.
         */
        void UpdateParentView();

        /**
        * Handle change command for USB mode
        * @param aMode usb mode id
        */
        void HandleCmdChangeUSBModeL(TInt aMode);
        
     private: // Functions from base classes

        /**
        * From CAknView Activates view.
        * @param aPrevViewId Id of previous view.
        * @param aCustomMessageId Custom message id.
        * @param aCustomMessage Custom message.
        * @return None.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                                TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );		

        /**
        * From CAknView Deactivates view.        
        * @param None.
        * @return None.
        */
        void DoDeactivate();

    private: // Data
        //Reference to the UI container
        //owned by this class
        CUSBClassChangeUIPluginContainer*   iContainer;
        
        //reference to the AppUI
        //not owned
        CAknViewAppUi*              		iAppUIRef;
        
        //Reference to the Model
        //owned by this class
        CUSBClassChangeUIPluginModel*	    iModelRef;	
        
        // Flag used if it is needed to load 
        // the UI plugin 
        TBool                       		iLoadAsGSPlugin;
        
        CEikonEnv*                          iEikEnv;
        // Flag for eikon env.
        TInt                                iResourceFileFlag; 
        // Previous view of GS ap
        TVwsViewId 				    		iPrevViewId; 	   
       
        TBool                               iHelpSupported;

    };

#endif  // USBCLASSCHANGEUIPLUGINVIEW_H

// End of File
