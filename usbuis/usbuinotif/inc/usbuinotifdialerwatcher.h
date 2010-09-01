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
* Description:  Declares USB dialer activation watcher class
 *
*/

#ifndef C_USBUINOTIFDIALERWATCHER_H
#define C_USBUINOTIFDIALERWATCHER_H

#include <e32base.h>
#include <coecntrl.h> 
#include <viewclipartner.h> 
#include <viewcli.h> 


/**
 * Dialer activation callback
 *
 * This class specifies the functions when dialer is activated 
 * or note can be reactivated 
 * 
 */
class MDialerNotifier
    {
public:
    /**     
     * The function to be when Dialaer is activated
     *          
     */
    virtual void DialerActivated() = 0;
    
    /**     
     * The function to be when Dialaer is deactivated
     * and note can be shown again
     *          
     */
    virtual void ReActivateDialog() = 0;
    };

/** 
 *
 * This class will check if application key is pressed when error note is active. 
 * When application key is pressed dialer view is activated and error notifier is 
 * informed so it can dismis dialog. When dialer is deactivated error notifier is 
 * notified that dialog can be again shown.
 *
 */
NONSHARABLE_CLASS( CUsbuinotifDialerWatcher):  public CCoeControl,  
                                               public MVwsSessionWrapperObserver  

    {
public:
    /**
     * Two-phased constructor.     
     * @param aDialerKeyNotifier Callback interface for dialer activation/deactivation
     * @return dialer watcher
     */
     
    static CUsbuinotifDialerWatcher* NewL( MDialerNotifier* aDialerNotifier );

    /**
     * Two-phased constructor.
     * @param aDialerKeyNotifier Callback interface for dialer activation/deactivation     
     * @return dialer watcher 
     */
    static CUsbuinotifDialerWatcher* NewLC( MDialerNotifier* aDialerNotifier  );

    /**
     * Destructor
     */
    virtual ~CUsbuinotifDialerWatcher();
    
private:
    
    /**
     * From CCoeControl
     * @param aKeyEvent Key Event
     * @param aType Type of event
     */
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);    

private:
    
    /**
     * From MVwsSessionWrapperObserver
     * @param aEvent View event
     */
    void HandleViewEventL(const TVwsViewEvent &aEvent);
    

private:

    /**
     * Constructor
     * @param aDialerKeyNotifier Callback interface for dialer activation/deactivation     
     */
    CUsbuinotifDialerWatcher( MDialerNotifier* aDialerKeyNotifier  );
    
    /**
     * Two-phased constructor.
     */
    void ConstructL();

private:
    // Member variables
    
    /**
     *  The observer for this objects events
     *  Not own. 
     */
    MDialerNotifier* iNotify;
    
    /**
     *  The observer for this objects events
     *  Own. 
     */
    CVwsSessionWrapper* iwsSession;
    
    /**
     * Is notifier allready notified. 
     */
    TBool iNotified;
    };
#endif // C_USBUINOTIFDIALERWATCHER_H
