/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Device state watcher class.
 *
*/


#ifndef USBDEVICESTATEWATCHER_H
#define USBDEVICESTATEWATCHER_H

// INCLUDE FILES
#include <e32base.h>
#include <usbman.h>

/**
 * A callback interface for informing about device state changes
 */
class MUSBDeviceStateObserver
    {
    public:
        
        /**
         * Informs the observer that USB device state has changed.
         * @param aPreviousState previous device state.
         * @param aCurrentState current device state.
         */
        virtual void DeviceStateChanged(TUsbDeviceState aPreviousState,
                                        TUsbDeviceState aCurrentState) = 0;
    };

// CLASS DECLARATION

/**
 * Class that listens for USB device state changes and notifies
 * the observer.
 */
class CUSBDeviceStateWatcher : public CActive
    {
    public:  // Constructors and destructor
        
        /**
         * Two-phased constructor. Uses existing usb manager session.
         * Note that it's not possible (and usually not necessary) to attach
         * more than one USBDeviceStateWatcher to the same session.
         *
         * @param aObserver  Reference to device state observer.
         * @param aUsbMan    Existing usb manager session.
         * @return Pointer to created object.
         */
        static CUSBDeviceStateWatcher* NewL(MUSBDeviceStateObserver& aObserver,
            RUsb& aUsbMan);
        
        /**
         * Two-phased constructor. Creates its own usb manager session.
         *
         * @param aObserver  Reference to device state observer.
         * @return Pointer to created object.
         */
        static CUSBDeviceStateWatcher* NewL(MUSBDeviceStateObserver& aObserver);
        
        /**
        * Destructor.
        */
        virtual ~CUSBDeviceStateWatcher();

    public: // from base class CActive

        /**
        * From CActive.
        * This method is called when device state has changed.
        */
        void RunL();

        /**
        * From CActive.
        * In this implementation this method should never be called.
        *
        * @param aError the leave code
        * @return KErrNone
        */
        TInt RunError(TInt aError);

        /**
        * From CActive
        * If there is outstanding request pending when Cancel() is called then
        * this method must cancel request.
        */
        void DoCancel();

    private:

        /**
         * Private constructor.
         *
         * @param aObserver Reference to MUSBDeviceStateObserver.
         */
        CUSBDeviceStateWatcher(MUSBDeviceStateObserver& aObserver);

        /**
         * 2nd phase constructor.
         * Creates its own usb manager session.
         */
        void ConstructL();
        
        /**
         * 2nd phase constructor.
         * Uses existing usb manager session.
         *
         * @param aUsbMan    Existing usb manager session.
         */
        void ConstructL(RUsb& aUsbMan);
        
        /**
         * Code shared by all ConstructL methods.
         */
        void CommonConstructL();
        
        // Disable default C++ behavior that makes no sense
        // for this implementation.
        CUSBDeviceStateWatcher();
        CUSBDeviceStateWatcher(const CUSBDeviceStateWatcher&);
        CUSBDeviceStateWatcher& operator=(const CUSBDeviceStateWatcher&);

    private: // Data

        /**
         * Handle to Usb Manager
         */
        RUsb iUsbMan;

        /**
         * Current device state
         */
        TUsbDeviceState iCurrentState;

        /**
         * Last known device state
         */
        TUsbDeviceState iPreviousState;

        /**
         * Refernce to the observer
         */
        MUSBDeviceStateObserver& iObserver;
    };

#endif   // USBDEVICESTATEWATCHER_H
