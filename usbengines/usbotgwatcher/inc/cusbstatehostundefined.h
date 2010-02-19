/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the concret state
 *
*/


#ifndef C_USBSTATEHOSTUNDEFINED_H
#define C_USBSTATEHOSTUNDEFINED_H

#include "cusbotgwatcher.h"
#include "cusbstatehostabase.h"
#ifndef STIF

#else

#endif

/**
 *  This class implements behaviour in undefined state (when host)
 *
 */
NONSHARABLE_CLASS( CUsbStateHostUndefined ) : public CUsbStateHostABase
    {
    public:

        /**
         * Destruction
         */
        virtual ~CUsbStateHostUndefined();
        
        /**
         * Two-phased constructor.
         * @param aWatcher owner
         */
        static CUsbStateHostUndefined* NewL(CUsbOtgWatcher* aWatcher);

    private:
        /**
         * default constructor is protected to be able to be inherited by child classes
         * @param aOwner owner of the state
         */
        CUsbStateHostUndefined(CUsbOtgWatcher* aOwner);
        
        /**
         * Second phase construction
         */
        void ConstructL();

        /**
         * This function is needed to find a real state by id
         * This might be used in switching states in watcher. New state object searched by its id
         * @return Id
         */
        virtual TUsbStateIds Id();

        /**
         * Id Pin OFF
         */
        void IdPinOffL();
        /**
         * Id Pin On
         */
        void IdPinOnL();

        // From VBus observer
        /**
         * VBus down
         */
        void VBusDownL();
        /**
         * VBus UP
         */
        void VBusUpL();
        
        /**
         * VBus error
         */
        void VBusErrorL();
        
        // From OTG state observer
         /**
          * Became to Idle when A
          */
        void AIdleL();
         /**
         * AVBus Error
         */
        void AVBusErrorL();
        /**
         * Became to Idle when B
         */
        void BIdleL();
        /**
         * Became to Peripheral when B
         */
        void BPeripheralL();
        
        // From Host Event notification observer
        /**
         * Device is detached
         * @param aInfo Device event data
         */
        virtual void DeviceDetachedL(TDeviceEventInformation aInfo);

private:
    // data
    };

#endif //  C_USBSTATEHOSTUNDEFINED_H
