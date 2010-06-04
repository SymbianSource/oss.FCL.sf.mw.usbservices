/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  OtgHost state watcher class.
 *
*/


#ifndef USBOTGHOSTSTATEWATCHER_H
#define USBOTGHOSTSTATEWATCHER_H

// INCLUDE FILES
#include <e32base.h>
#include <e32property.h>
#include <usbman.h>
#include <usbhostdefs.h>


/**
 * A callback interface for informing about OtgHost state changes
 */
class MUSBOtgHostStateObserver
    {
    public:
        
        /**
         * Informs the observer that USB Otg Id Pin state has changed.
         * @param aIsPinOn Id pin state - ETrue(On), EFalse(Off) 
         */
        virtual void OtgHostIdPinStateChanged(TBool aIsIdPinOn) = 0;

        /**
         * Informs the observer that USB host event.
         * @param aEventInfo Device information related to this event
         */
        virtual void HostEventNotify(TDeviceEventInformation aEventInfo) = 0;
    };

// CLASS DECLARATION

/**
 * Class that listens for USB OtgHost state changes then notifies
 * the observer.
 */
class CUSBOtgHostStateWatcher : CBase
    {
    class CUSBOtgIdPinStateWatcher : public CActive
        {
    public:
        /**
         * Two-phased constructor. 
         *
         * @param aOtgHostStateWatcher Reference to the owner - CUSBOtgHostStateWatcher.
         * @return Pointer to created object.
         */
        static CUSBOtgIdPinStateWatcher* NewL(CUSBOtgHostStateWatcher& aOtgHostStateWatcher);

        /** C++ destructor */
        virtual ~CUSBOtgIdPinStateWatcher();

        /**
         * Get Id pin state
         * @aIsIdPinOn Receive Id pin on/off state. ETrue: Id pin on, EFalse: Id pin off.
         * @return KErrNone if successful, otherwise the error that occurred 
         */
        TInt IsIdPinOn(TBool &aIsIdPinOn);
        
    protected:
        // from base class CActive
        /**
         * From CActive.
         * This method is called when Id pin state has changed.
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
         * C++ constructor
         * @param aOtgHostStateWatcher Reference to the owner - CUSBOtgHostStateWatcher.
         */
        CUSBOtgIdPinStateWatcher(CUSBOtgHostStateWatcher& aOtgHostStateWatcher);
        
        /**
         * 2nd phase constructor.
         */
        void ConstructL();
        
    private: // Data
        /**
         * The observer observes property change
         */
        RProperty iIdPin;
        
        /**
         * The owner - CUSBOtgHostStateWatcher
         */
        CUSBOtgHostStateWatcher& iOtgHostStateWatcher;
        };

    class CUSBHostEventNotifWatcher : public CActive
        {
    public:
        /**
         * Two-phased constructor. 
         *
         * @param aOtgHostStateWatcher Reference to the owner - CUSBOtgHostStateWatcher.
         * @return Pointer to created object.
         */
        static CUSBHostEventNotifWatcher* NewL(CUSBOtgHostStateWatcher& aOtgHostStateWatcher);

        /** C++ destructor */
        virtual ~CUSBHostEventNotifWatcher();

    protected:
        // from base class CActive
        /**
         * From CActive.
         * This method is called when host event emerged .
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
         * C++ constructor
         * @param aOtgHostStateWatcher Reference to the owner - CUSBOtgHostStateWatcher.
         */
        CUSBHostEventNotifWatcher(CUSBOtgHostStateWatcher& aOtgHostStateWatcher);
        
        /**
         * 2nd phase constructor.
         */
        void ConstructL();
        
    private: // Data
        /**
         * Notification info comes to this member
         */
        TDeviceEventInformation iEventInfo;
        
        /**
         * The owner - CUSBOtgHostStateWatcher
         */
        CUSBOtgHostStateWatcher& iOtgHostStateWatcher;
        };
    
    public:  // Constructors and destructor
        
        /**
         * Two-phased constructor. Uses existing usb manager session.
         * Note that it's not possible (and usually not necessary) to attach
         * more than one USBOtgHostStateWatcher to the same session.
         *
         * @param aObserver  Reference to OtgHost state observer.
         * @param aUsbMan    Existing usb manager session.
         * @return Pointer to created object.
         */
        static CUSBOtgHostStateWatcher* NewL(MUSBOtgHostStateObserver& aObserver,
            RUsb& aUsbMan);
        
        /**
         * Destructor.
         */
        virtual ~CUSBOtgHostStateWatcher();

        /**
         * Get Id pin state
         * @aIsIdPinOn Receive Id pin on/off state. ETrue: Id pin on, EFalse: Id pin off.
         * @return KErrNone if successful, otherwise the error that occurred 
         * @see CUSBOtgIdPinStateWatcher::IsIdPinOn
         */
        TInt IsIdPinOn(TBool &aIsIdPinOn);
        
        /**
         * Check wheather a pheripheral device is connected or not. 
         * @aIsConnected A pheripheral device connected(ETrue) or not(EFailse).
         * @return KErrNone if successful, otherwise the error that occurred 
         */
       TInt IsPeripheralConnected(TBool &aIsConnected);
        
    private:

        /**
         * Private constructor.
         * @param aObserver Reference to MUSBOtgHostStateObserver.
         * @param aUsbMan Existing usb manager session.
         */
        CUSBOtgHostStateWatcher(MUSBOtgHostStateObserver& aObserver, RUsb& aUsbMan);

        /**
         * 2nd phase constructor.
         */
        void ConstructL();
        
    private: // Data

        /**
         * Usb Manager
         */
        RUsb& iUsbMan;

        /**
         * Property of peripheral-connected
         */
        RProperty iPeripheral;
        
        /**
         * The observer of OTG/host events
         */
        MUSBOtgHostStateObserver& iObserver;
        
        /**
         * The watcher of Otg Id Pin state
         * Own.
         */
        CUSBOtgIdPinStateWatcher* iOtgIdPinWatcher;
        
        /**
         * The watcher of host event
         * Own.
         */
        CUSBHostEventNotifWatcher* iHostEventWatcher;
        
    };

#endif   // USBOTGHOSTSTATEWATCHER_H
