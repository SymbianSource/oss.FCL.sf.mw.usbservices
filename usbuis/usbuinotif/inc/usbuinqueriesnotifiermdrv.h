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
 * Description:  Declares USB UI Queries notifier.
 *
 */

#ifndef USBUINQUERIESNOTIFIER_H
#define USBUINQUERIESNOTIFIER_H

// INCLUDES

#include "usbnotifier.h"      // Base class
#include <AknQueryDialog.h>   // AVKON component
// CLASS DECLARATION

/**
 *  This class is used to show USB query dialogs.
 *
 *  @lib 
 *  @since S60 3.0
 */
NONSHARABLE_CLASS(CUSBUIQueriesNotifier) : public CUSBUINotifierBase
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CUSBUIQueriesNotifier* NewL();

    /**
     * Destructor.
     */
    virtual ~CUSBUIQueriesNotifier();

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
     * @param aReplySlot A reply slot.
     * @param aMessage Should be completed when the notifier is deactivated.
     * @return None.
     */
    void GetParamsL(const TDesC8& aBuffer, TInt aReplySlot,
            const RMessagePtr2& aMessage);

private:
    /*
     * From MEikSrvNotifierBase2 Synchronic notifier launch.        
     * @param aBuffer Received parameter data.
     * @return A pointer to return value.
     */
    virtual TPtrC8 StartL(const TDesC8& aBuffer);

    /**
     *  C++ default constructor.
     */
    CUSBUIQueriesNotifier();

private:
    // New functions

    /**
     * Show query dialog 
     * @param aStringHolder   The string for the query. 
     * @param aCoverDialogId  The dialog ID for the cover UI.
     * @param aIsCancelKey    Does the dialog show Cancel key.
     * @return KErrNone - accepted, KErrCancel - Cancel or End call key
     */
    TInt QueryUserResponseL(const TDesC& aStringHolder, TInt aCoverDialogId,
            TBool aIsCancelKey);

    /**
     * Get attributes for the query dialog
     * The query type is idenfied by the member variable iQueryType.
     * The caller should pop and destroy the returned heap descriptor 
     * when it is no longer needed.
     * @param aCoverDialogId  Returned dialog ID for the cover UI.
     * @param aIsCancelKey    Returned info about showing the Cancel key.
     * @return                The string holder for the query. 
     */
    HBufC* GetQueryAttributesLC(TInt& aCoverDialogId, TBool& aIsCancelKey);

private:
    // Data
    
    /**
    * Not own, destroys self when lauched.
    */
    CAknQueryDialog* iUSBQueryDlg;
    TUSBUIQueries iQueryType; // To store the type of the query

    };
#endif // USBUINQUERIESNOTIFIER_H
