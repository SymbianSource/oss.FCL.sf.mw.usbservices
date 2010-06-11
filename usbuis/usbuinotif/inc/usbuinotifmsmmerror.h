/*
* Copyright (c) 2007, 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declares MSMM error notifier.
 *
*/


#ifndef USBUINOTIFMSMMERROR_H
#define USBUINOTIFMSMMERROR_H

// INCLUDES
#include <hb/hbwidgets/hbdevicemessageboxsymbian.h>
#include "usbnotifier.h"      // Base class

#define KUsbUiNotifOtgGeneralQueryGranularity 3

// CLASS DECLARATION

/**
 *  This class is used to show general USB query.
 *  Asynchronous call is required.
 *
 *  @lib
 */
NONSHARABLE_CLASS(CUsbUiNotifMSMMError) : public CUSBUINotifierBase,
                                          public MHbDeviceMessageBoxObserver
    {
public:

    /**
     * Indexes for the strings used in KUsbUiNotifMSMMError which are mapped to 5 errors.
     */
    enum TUsbUiNotifMSMMError
        {
        EUsbMSMMGeneralError,	
        EUsbMSMMUnknownFileSystem,
        EUsbMSMMOutOfMemory
        };

    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CUsbUiNotifMSMMError* NewL();

    /**
     * Destructor.
     */
    virtual ~CUsbUiNotifMSMMError();

    /**
     * Call back function to observe device message box closing.
     * @param aMessageBox Pointer to the closing message box instance.
     * @param aButton Button that was pressed.
     */
    void MessageBoxClosed(const CHbDeviceMessageBoxSymbian* aMessageBox,
        CHbDeviceMessageBoxSymbian::TButtonId aButton);

protected:

    /**
     * Second phase contructor
     */
    void ConstructL();

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
     */
    void Cancel();

    /**
     * From CUSBUINotifierBase Used in asynchronous notifier launch to 
     * store received parameters into members variables and 
     * make needed initializations.
     * @param aBuffer A buffer containing received parameters
     * @param aReplySlot A reply slot.
     * @param aMessage Should be completed when the notifier is deactivated.
     */
    void StartDialogL(const TDesC8& aBuffer, TInt aReplySlot,
            const RMessagePtr2& aMessage);

private:

    /**
     *  C++ default constructor.
     */
    CUsbUiNotifMSMMError();

private:
    // New functions

private:
    // Data
    CHbDeviceMessageBoxSymbian* iQuery;
    CDesCArrayFlat* iStringIds;
     };
#endif // USBUINOTIFMSMMERROR_H
