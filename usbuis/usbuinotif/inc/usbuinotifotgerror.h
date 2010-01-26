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
* Description:  Declares USB UI Queries notifier.
 *
*/


#ifndef USBUINOTIFOTGERROR_H
#define USBUINOTIFOTGERROR_H

// INCLUDES

#include "usbnotifier.h"      // Base class
#include <AknQueryDialog.h>   // AVKON component
#define KUsbUiNotifOtgGeneralQueryGranularity 3
// CLASS DECLARATION

/**
 *  This class is used to show general USB query.
 *  Asynchronous call is required.
 *
 *  @lib
 */
NONSHARABLE_CLASS(CUsbUiNotifOtgError) : public CUSBUINotifierBase
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CUsbUiNotifOtgError* NewL();

    /**
     * Destructor.
     */
    virtual ~CUsbUiNotifOtgError();

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
     * From CUSBUINotifierBase Gets called when a request completes.
     */
    void RunL();

    /**
     * From CUSBUINotifierBase Used in asynchronous notifier launch to 
     * store received parameters into members variables and 
     * make needed initializations.
     * @param aBuffer A buffer containing received parameters
     * @param aReplySlot A reply slot.
     * @param aMessage Should be completed when the notifier is deactivated.
     */
    void GetParamsL(const TDesC8& aBuffer, TInt aReplySlot,
            const RMessagePtr2& aMessage);

private:

    /**
     *  C++ default constructor.
     */
    CUsbUiNotifOtgError();

private:
    // New functions

    /**
     * Show query dialog     
     * @return KErrNone - accepted, KErrCancel - Cancel or End call key
     */
    TInt QueryUserResponseL();

private:
    // Data
    /**
     *  Query
     *  Not own, destroys self when lauched.
     */
    CAknQueryDialog* iQuery; 
    RArray<TInt> iStringIds;
    TInt iErrorId;
    };
#endif // USBUINOTIFOTGERROR_H
