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
* Description:  Declares USB UI connection notifier.
 *
*/


#ifndef USBUINOTIFOTGWARNING_H
#define USBUINOTIFOTGWARNING_H

// INCLUDES
#include <AknNoteWrappers.h>
#include <usbuinotif.h>

#include "usbnotifier.h"   // Base class
#define KUsbUiNotifOtgGeneralNoteGranularity 1
// CLASS DECLARATION

/**
 *  This class is used to show general USB note.
 *  Synchronous call is enouph.
 * 
 */
NONSHARABLE_CLASS(CUsbUiNotifOtgWarning) : public CUSBUINotifierBase
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CUsbUiNotifOtgWarning* NewL();

    /**
     * Destructor.
     */
    virtual ~CUsbUiNotifOtgWarning();

protected:

    /**
     * Second phase contructor
     */
    void ConstructL();

private:
    // Functions from base class


    /**
     * From CUSBUINotifierBase. Called when a notifier is first loaded.        
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
     * From CUSBUINotifierBase. Gets called when a request completes.
     * @param None.
     * @return None.
     */
    void RunL();

    /**
     * From CUSBUINotifierBase. Used in asynchronous notifier launch to 
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

    /**
     *  C++ default constructor.
     */
    CUsbUiNotifOtgWarning();

    /**
     * Publish the dialog to the cover UI
     * The cover UI may use the personality ID or the localized
     * personality name e.g. "Mass storage". 
     * @param aNote           The dialog to be published.
     * @param aPersonalityId  The personality ID.
     * @param aLocalizedPersonalityDescriptor The personality as a string.
     */
    void
            PublishToCoverUiL(CAknResourceNoteDialog* aNote,
                    TInt aPersonalityId,
                    const HBufC* aLocalizedPersonalityDescriptor);

private:
    // data
    RArray<TInt> iStringIds;

    //Own
    CAknResourceNoteDialog* iNote;

    TInt iNoteId;
    };
#endif // USBUINOTIFOTGWARNING_H
// End of file
