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
 * Description:  Declares USB Cable Connected notifier class.
 *
 */

#ifndef USBUINCABLECONNECTEDNOTIFIER_H
#define USBUINCABLECONNECTEDNOTIFIER_H

// INCLUDES

#include <hbsymbiandevicedialog.h>
#include <hbsymbianvariant.h>

#include "usbnotifier.h" // Base class
// CLASS DECLARATION

/**
 *  This class is used to show query related to the USB cable connection
 *
 *  @lib
 */
NONSHARABLE_CLASS(CUSBUICableConnectedNotifier) : public CUSBUINotifierBase, 
    public MHbDeviceDialogObserver
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CUSBUICableConnectedNotifier* NewL();

    /**
     * Destructor.
     */
    virtual ~CUSBUICableConnectedNotifier();

private:
    // Functions from base class

    /**
     * From CUSBUINotifierBase Called when a notifier is first loaded.        
     * @param None.
     * @return A structure containing priority and channel info.
     */
    TNotifierInfo RegisterL();

    /**
     * From CUSBUINotifierBase The notifier has been deactivated 
     * so resources can be freed and outstanding messages completed.
     * @param None.
     * @return None.
     */
    void Cancel();

    /**
     * From CUSBUINotifierBase Gets called when a request completes.
     * @param None.
     * @return None.
     */
    void RunL();

    /**
     * From CUSBUINotifierBase Used in asynchronous notifier launch to 
     * store received parameters into members variables and 
     * make needed initializations.
     * @param aBuffer A buffer containing received parameters
     * @param aReplySlot a Reply slot.
     * @param aMessage Should be completed when the notifier is deactivated.
     * @return None.
     */
    void GetParamsL(const TDesC8& aBuffer, TInt aReplySlot,
            const RMessagePtr2& aMessage);
 
       
private:
    // functions from MHbDeviceDialogObserver
   
    /**
    * lunches the QT usb ui setting
    * @param aData is a CHbSymbianVariantMap daya which contains the keys
    */ 
    void DataReceived(CHbSymbianVariantMap& aData);
    /*
     * Virtual function from MHbDeviceDialogObserver
     * Not implemented
     */     
    void DeviceDialogClosed(TInt aCompletionCode);
         
    

private:
    //New functions
     /**
     * Get the ID for current USB personality mode
     * @param aCurrentPersonality current personality id
     */
    void GetCurrentIdL(TInt& aCurrentPersonality);
    /**
     * Get the mode name and header for current personality
     * The parameters are pushed to the cleanup stack in order
     * aDescription, aHeader.
     * @param aDescription The returned current personality string.
     * @param aHeader The header string for message query.
     */
    void GetPersonalityStringLC(HBufC*& aHeader,HBufC*& aDescription );

    /**
     * Runs the connected discreet note
     */
    void RunQueryL();

  
    /**
     * launches the (USB) application
     * @param aProcessName The process name (*.exe)
     * @param TUidType 
     */
    void LaunchApplication(const TDesC & aProcessName,const TUidType & aUidType) const;

    /**
     * Adds a parameter to the dialog parameters.
     */
    void AddParameterL(
            CHbSymbianVariantMap* aParameters,
            const TDesC& aKey,
            const TAny* aData,
            CHbSymbianVariant::TType aDataType);

private:
    /**
     *  C++ default constructor.
     */
    CUSBUICableConnectedNotifier();
    
    CHbDeviceDialog* iDialog; 

    };
#endif // USBUINCABLECONNECTEDNOTIFIER_H
