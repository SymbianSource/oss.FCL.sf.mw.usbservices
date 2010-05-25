/*
 * Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <aknlistquerydialog.h> 
#include <AknQueryDialog.h>

#include "usbnotifier.h" // Base class
// CLASS DECLARATION

/**
 *  This class is used to show query related to the USB cable connection
 *
 *  @lib
 */
NONSHARABLE_CLASS(CUSBUICableConnectedNotifier) : public CUSBUINotifierBase,
       public MEikCommandObserver
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
 
    /**
     * Handles the command on USB connected note
     */
    void ProcessCommandL(TInt aCommandId);

private:
    //New functions
     /**
     * Get the ID for current USB personality mode
     * @param aCurrentPersonality current personality id
     */
    void GetCurrentIdL(TInt& aCurrentPersonality);

    /**
     * Get the mode name and header for current personality
     * @param aDescription The returned current personality string.
     * @param aHeader The header string for message query.
     */
    void GetPersonalityStringL(HBufC*& aHeader,HBufC*& aDescription );

    /**
     * Runs the connected discreet note
     */
    void RunQueryL();
  
    /**
     * creates the USB UI setting view
     * @param aProcessName The process name (USBClassChangeUI.exe)
     * @param TUidType 
     */
    void CreateChosenViewL(const TDesC & aProcessName,const TUidType & aUidType) const;

private:
    /**
     *  C++ default constructor.
     */
    CUSBUICableConnectedNotifier();
    
    /**
     * Waiter for canceling notifier. Canceling is not posible when note is visible
     */
    CActiveSchedulerWait    iNoteWaiter;    
    
    /**
     * Note visible
     */
    TBool   iNoteVisible;

    /**
     * Note tapped
     */
    TBool   iNoteTapped;

    };

#endif // USBUINCABLECONNECTEDNOTIFIER_H
