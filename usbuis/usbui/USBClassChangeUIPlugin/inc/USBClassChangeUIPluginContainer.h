/*
* Copyright (c) 2005, 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declares container control for application.
*
*/


#ifndef USBCLASSCHANGEUIPLUGINCONTAINER_H
#define USBCLASSCHANGEUIPLUGINCONTAINER_H

// INCLUDES
#include <coecntrl.h>
#include <eikfrlb.h>
#include <eikclb.h>
#include <aknlists.h> 
#include <AknInfoPopupNoteController.h>
#include <AknIconArray.h>
#include "USBClassChangeUIPlugin.hrh"

// CONSTANTS
/** number of controls inside this component */
const TInt KUSBClassChangeUIComponentCount = 1;
const TInt KUSBClassChangeUIIconGranularity = 3;
const TInt KMaxVisibleTime = 3500;
const TInt KMsToWaitBeforePopup = 1000;

// FORWARD DECLARATIONS
class CUSBClassChangeUIPluginView;
class CUSBClassChangeUIPluginModel;

// CLASS DECLARATION

/**
*  This class handles main view dependent requests from user and 
*  contains listbox for main view items.
*  @lib
*/
class CUSBClassChangeUIPluginContainer : public CCoeControl,                                   
                                         public MEikListBoxObserver,
                                         public MDesCArray
    {
    public: // Constructors and destructor

        /**
        * Factory method NewL
        * @return a pointer to itself
        */
        static CUSBClassChangeUIPluginContainer* NewL(
            CUSBClassChangeUIPluginView* aView,
            CUSBClassChangeUIPluginModel* aModel );
        
	    /**
        * Destructor.
        */
        virtual ~CUSBClassChangeUIPluginContainer();

	private:

        /**
        * C++ constructor.
        */
        CUSBClassChangeUIPluginContainer(
            CUSBClassChangeUIPluginView* aView,
            CUSBClassChangeUIPluginModel* aModel);

	    /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();

	public: // new functions
	 
        /**
        * Handles setting changes.
        */
        void SettingChanged();
 
        /**
        * Handles screen resolution changes
        * @param aType informs the type of resource change
        */
        void HandleResourceChange( TInt aType ); 
        
        /**
        * Returns the current item index (highlight position) 
        * for main setting list.
        * @return Current item index.
        */
        TInt CurrentItemIndex();

        // MDesCArray implementation

        /**
        * Returns number of items in array / main list.
        * @return Number of items.
        */
        TInt MdcaCount() const;

        /**
        * Returns the current format string for the given item.
        * @param aIndex Index of the item.
        * @return Pointer to the format string.
        */
        TPtrC MdcaPoint( TInt aIndex ) const;
        
        /**
        * selects the string to be passed to ShowPopUpL
        * @param aValue is the index of the item in the list.
        */
        void ShowPopUpsL( TInt aIndex);
             
    private: // Functions from base classes
        
        /**
        * From CoeControl Gets the number of controls contained 
        * in a compound control.        
        * @return The number of component controls contained by this control.
        */
        TInt CountComponentControls() const;

        /**
        * From CoeControl Gets the specified component of a compound control.
        * @param aIndex The index of the control to get.
        * @return Reference to the component control.
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        * From CoeControl Handles key events.
        * @param aKeyEvent The key event that occurred.
        * @param aType The event type.
        * @return Response to the key event.
        */
        TKeyResponse OfferKeyEventL(
            const TKeyEvent& aKeyEvent, TEventCode aType );        

        /**
        * From CoeControl Gets the control's help context.
        * @param aContext The control's help context
        */
        void GetHelpContext(TCoeHelpContext& aContext) const;
    
        /**
        * From CoeControl Responds to size changes. 
        * Sets the size and position of the contents of this control.
        */
        void SizeChanged();
        
        /**
        * From CoeControl Responds to a change in focus.
        * @param aDrawnow Contains the value that was passed to it by 
        *                 SetFocus(). 
        */
        void FocusChanged( TDrawNow aDrawnow );
     
  
    protected:
        /**
        * From MEikListBoxObserver Handles list box events.
        * @param aListBox The originating list box.
        * @param aEventType The event type.
        */
        void HandleListBoxEventL(
            CEikListBox* aListBox, TListBoxEvent aEventType );
        
    private: // Data

        // The setting items listbox
        // owned by this class
        CAknDoubleLargeStyleListBox* iUSBMainListBox;		     
        // Reference to appui object
        CUSBClassChangeUIPluginView* iViewRef;
        // To get personality IDs arrays
        // not owned by this class
        CUSBClassChangeUIPluginModel* iModelRef;
        // All format string combinations 
        // owned by this class              
        CDesCArrayFlat* iUSBAllItemsArray; 
        // All format string combinations 
        // owned by this class 
        CDesCArrayFlat* iUSBAllItemsArrayDefault; 
        // All format string combinations 
        // owned by this class 
        CDesCArrayFlat* iUSBAllItemsArrayActive; 
        // All format string combinations 
        // owned by this class 
        CDesCArrayFlat* iPopUpItems; 
       
        // Current highlighted main list item 
        TInt iCurrentItemIndex;

        // The controller for the PopUps (tooltips) in UI Setting view 
        // owned by this class 
        CAknInfoPopupNoteController* iPopupController;
 
};

#endif

// End of File
