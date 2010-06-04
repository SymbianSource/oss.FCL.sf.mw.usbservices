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
* Description:  Declares model class for application.
*
*/


#ifndef USBCLASSCHANGEUIPLUGINMODEL_H
#define USBCLASSCHANGEUIPLUGINMODEL_H


// INCLUDES
#include <badesca.h>
#include <usbman.h>
#include <usbwatcher.h>
#include <eikfrlb.h>
#include <eikclb.h>
#include <aknlists.h> 
#include "USBClassChangeUIPlugin.hrh"
#include "USBClassChangeUIPluginCRWatcher.h"
#include "USBDeviceStateWatcher.h"
#include "usbotghoststatewatcher.h"

// CONSTANTS
/** granularity for supported modes array */
const TInt KUSBClassChangeUISupportedModesGranularity = 3;

// FORWARD DECLARATIONS
class CUSBDeviceStateWatcher;

// CLASS DECLARATION

/**
 * A callback interface for informing change of settings and device state.
 */
class MUSBUIObserver : public MUSBDeviceStateObserver, 
    public MUSBOtgHostStateObserver
    {
    public: // New functions
        
        /**
        * Informs the observer that a setting has been changed. 
        * The list should be refreshed.
        */
        virtual void SettingChanged() = 0;
    };

/**
* This class has access to all data needed by this application.
* All manipulation of data is done through this class.
*/
class CUSBClassChangeUIPluginModel : public CActive, 
    public MUSBUICRSettingObserver, MUSBDeviceStateObserver, MUSBOtgHostStateObserver
    {
    public: // Constructors and destructor

	    /**
        * Destructor.
        */
        virtual ~CUSBClassChangeUIPluginModel(); 

    public: // New functions
        
        
        /**
        * Factory method NewL
        * @return a pointer to itself
        */
        static CUSBClassChangeUIPluginModel* NewL();
        
        
        /**
        * Sets the USB mode.
        * @param aMode the id of the selected personality 
        */
        void SetUSBModeL( TInt aMode );

        /**
        * Gets the USB mode.
        * @return the id of the personality
        */
        TInt USBMode();
        
        /**
         * Gets the number of usb personalities.
         * @return the number of existing personalities 
         */
        TInt PersonalityCount() ;
        /**
         * Gets the current device state.
         * @param   aState  receives the current state of the USB device
         * @return  KErrNone if successful, otherwise the error that occurred
         */
        TInt GetDeviceState(TUsbDeviceState& aState);

        /**
         * Gets description for the specified USB mode (personality ID)
         * @param   aMode a personality id
         * @param   aDescription a localized text string
         * @return  KErrNone if successful, otherwise the error that occurred.
         */
        TInt GetDescription(TInt aMode, HBufC*& aDescription);

        /**
        * Updates temporary member variable data.        
        * @param aObserver Reference to MUSBUIObserver instance.        
        */
        void SetSettingsObserver(MUSBUIObserver* aObserver);
        
        /*
        * Reads the supported USB modes from the resource file and CenRep.
        * Adds modes to the CDesCArrayFlat lists to be used either
        * for list box or for setting page
        * @param aListBox The list where the USB modes are appended for list box
        * @param aListBoxDefault The list where the USB modes are appended with their secondary line "default" 
        * @param aListBoxActive The list where the USB modes are appended with their secondary line "active" 
        * @param aPopUpItems The list where the descriptive strings will be stored 
        * @param aIconArray The list where icons for USB modes are stored
        */
        void SupportedUSBModeArraysL(  CDesCArrayFlat& aListBox, CDesCArrayFlat& aListBoxDefault,
                CDesCArrayFlat& aListBoxActive, CDesCArrayFlat& aPopUpItems, CArrayPtrFlat<CGulIcon>& aIconArray );       
        
        /**
        * From MUSBUICRSettingObserver.
        * Changes the USB mode in the USB application. 
        * Called by CUSBClassChangeUICenRepWatcher, when the key 
        * KUsbWatcherPersonality is changed by the USBWatcher.
        * @param aKey identifies the setting that has changed
        */
        void SettingChangedL( TUint32 aKey );

        /**
         * From MUSBOtgHostStateObserver
         * @see MUSBOtgHostStateObserver::OtgHostIdPinStateChanged
         */
        void OtgHostIdPinStateChanged(TBool aIsIdPinOn);
        
        /**
         * From MUSBOtgHostStateObserver
         * @see MUSBOtgHostStateObserver::OtgHostIdPinStateChanged
         */
        void HostEventNotify(TDeviceEventInformation aEventInfo);
        
        /**
         * From MUSBDeviceStateObserver.
         * Informs the observer that USB device state has changed.
         * @param aPreviousState previous device state.
         * @param aCurrentState current device state.
         */
        void DeviceStateChanged(TUsbDeviceState aPreviousState,
                                TUsbDeviceState aCurrentState);

        
        /**
        * Gets the IDs of supported USB personalities.
        * @return RArray of personality IDs
        */
        RArray<TInt>& GetUsbIds();
             
        /**
         * Returns if it acts as a host and the driver to the attached pheripheral 
         * is loaded successfully(or partially). 
         * NOTE: Only single perihperal supported now.
         * @param aHasPeripheralEnabled Is A-device is ready to be used or not
         * @return KErrNone if successful, otherwise the error that occurred
         */
        TInt HasPeripheralEnabled(TBool& aHasPeripheralEnabled);
        
  
    protected:
        /**
        * Default ConstructL can leave.
        */
        void ConstructL();

	    /**
        * C++ default constructor.
        */
        CUSBClassChangeUIPluginModel();        
        
        /**
        * Derived from CActive
        */
        void RunL();

        /**
        * Derived from CActive
        */
        void DoCancel();
        
    private:
        /**
         * selects the icon related to each mode to be added into the iconlist
         * @param aMode the mode id
         * @param aIconArray The list where icons for USB modes are stored
         */
        void AddIconL (TInt aMode, CArrayPtrFlat<CGulIcon>& aIconArray);
               
        /**
         * creats and append the icons to the list
         * @param aID Item ID of the masked bitmap to be created.
         * @param aFilename Filename to be used to construct the item.
         * @param aBitmapId The ID if bitmap 
         * @param aMaskId The ID of bitmap's mask
         * @param aIconArray The list where icons for USB modes are stored
         */
        void CreateAndAppendIconL( const TAknsItemID& aID,
                                   const TDesC& aFileName,
                                   const TInt aBitmapId,
                                   const TInt aMaskId,
                                   CArrayPtrFlat<CGulIcon>& aIconArray);
                 
    private: // Data
         // Reference to settings observer
         MUSBUIObserver* iObserver;

         // References to central repository watcher
         // owned by this class
         CUSBClassChangeUIPluginCRWatcher* iCRPersonalityWatcher;

         // Reference to device state watcher
         // owned by this class
         CUSBDeviceStateWatcher* iDeviceStateWatcher;
         
         // Contains USB mode indexes
         RArray<TInt> iUsbModeIndexes; 
         
         // Central repository handle 
         // owned by this class
         CRepository* iRepository; 
         
         /** Handle to usbman for retrieving the personalities */
         RUsb iUsbman;
         
         /** Handle to USBWatcher for setting the personality */
         RUsbWatcher iUsbWatcher;         
         
         /** Is connected to USB Watcher */
         TBool iUsbWatcherConnected;
         
         /** UsbOtg status watcher */
         CUSBOtgHostStateWatcher* iOtgHostStateWatcher;
         
         /**
          * Ids of the devices for which host drivers have been loaded.
          * When we start to support hubs, there may be more than one element
          * in this array.
          */
         RArray<TInt> iDeviceIDs;
		 
         //Number of personalities
         TInt iPersonalityCount;
         
    };

#endif // USBCLASSCHANGEUIPLUGINMODEL_H

// End of File
