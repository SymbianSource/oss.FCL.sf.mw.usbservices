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
* Description:  Central repository key watcher class.
*
*/


#ifndef USBCLASSCHANGEUIPLUGINCRWATCHER_H
#define USBCLASSCHANGEUIPLUGINCRWATCHER_H

// INCLUDES
#include <centralrepository.h>

// FORWARD DECLARATIONS
class CRepository; 

/**
* A callback interface for informing change of a setting in Central Repository
*/
class MUSBUICRSettingObserver
    {
    public: // New functions
        
        /**
        * Informs the observer that a setting has been changed. 
        * @param aKey The changed key. 
        * @return None.
        */
        virtual void SettingChangedL( TUint32 aKey ) = 0;
    };


/**
* CUSBClassChangeUIPluginCRWatcher detects a change of the value of the CR key.
* CUSBClassChangeUIPluginCRWatcher provides CActive object that is used to 
* detect when the value of the selected key in Central Repository is changed 
* by the USBWatcher. One observer can be notified by one or more
* USBClassChangeUIPluginCRWatcher objects, each watching different keys. 
*/
class CUSBClassChangeUIPluginCRWatcher : public CActive
    {
        
    public:

        /**
        * Factory method NewL
        * @param aOwner     The observer to be notified on the change. 
        * @param aKey       The Central Repository key to be watched. 
        * @param aOneShot   If false, the listening is restarted after change.
        * @return           The new watcher object. 
        */
        static CUSBClassChangeUIPluginCRWatcher* NewL( 
            MUSBUICRSettingObserver& aOwner, TUint32 aKey, 
            TBool aOneShot = EFalse);

        /**
        * Starts listening for notifications. If already listening, nothing 
        * happens.
        */
        void StartListeningL();

        /**
        * Stops listening for notifications. If already stopped, nothing 
        * happens.
        * @param none
        * @return none 
        */
        void StopListening();

        /**
        * C++ default constructor
        * @param aOwner     The observer to be notified on the change. 
        * @param aKey       The Central Repository key to be watched. 
        * @param aOneShot   If false, the listening is restarted after change.
        */
        CUSBClassChangeUIPluginCRWatcher( 
            MUSBUICRSettingObserver& aOwner, TUint32 aKey, 
            TBool aOneShot = EFalse);

        /**
        * Destructor.
        */
        virtual ~CUSBClassChangeUIPluginCRWatcher();

    private:

        /**
        * Constructor.
        * @param none
        */

        void ConstructL();

        /**
        *	Implements CActive
        *   If this is not a one-shot CR watcher, the listening is restarted.
        *	@param none
        *	@return none
        */
        void RunL();
    
        /**
        *	Implements CActive
        *	@param aError the error returned
        *	@return error
        */
        TInt RunError(TInt aError);

        /**
        *	Implements CActive
        *	@param none
        *	@return none
        */
        void DoCancel();

     
    private:
        /** USB Watcher repository access */
        CRepository* iPersonalityRepository;
        /** The observer to be notified */
        MUSBUICRSettingObserver& iOwner;
        /** The repository key to be watcher */
        TUint32 iKey;
        /** If iOneShot is false, listening is restarted on notification */
        TBool iOneShot;
    };


#endif      // USBCLASSCHANGEUIPLUGINCRWATCHER_H

// End of File
