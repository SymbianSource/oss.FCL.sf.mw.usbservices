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
* Description:  Container that contains the USB mode listbox
*
*/


// INCLUDE FILES
#include <barsread.h>	// Resource reader

#include <usbpersonalityids.h>
#include <USBClassChangeUIPlugin.rsg> // Resources
#include <csxhelp/usb.hlp.hrh>   // Help id
#include <StringLoader.h>
#include <AknIconArray.h>
#include <eikmenub.h>
#include <eikmobs.h>

#include "USBClassChangeUIPlugin.h"
#include "USBClassChangeUIPluginContainer.h"
#include "USBClassChangeUIPluginModel.h"
#include "USBClassChangeUIPluginView.h"
#include "USBClassChangeUIPluginDebug.h"

// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUSBClassChangeUIPluginContainer*
CUSBClassChangeUIPluginContainer::NewL(
    CUSBClassChangeUIPluginView* aView,
    CUSBClassChangeUIPluginModel* aModel)
    {
    FLOG(_L("[USBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer:NewL"));
    CUSBClassChangeUIPluginContainer* self 
        = new (ELeave) CUSBClassChangeUIPluginContainer(aView, aModel);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self); 
    return self;
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::ConstructL(const TRect& aRect)
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginContainer::ConstructL()
    { 
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::ConstructL()"));
    
    CreateWindowL();
    iPopupController = CAknInfoPopupNoteController::NewL();
   
    iUSBAllItemsArray = new ( ELeave ) CDesCArrayFlat( 
        KUSBClassChangeUISupportedModesGranularity );
    iUSBAllItemsArrayDefault = new ( ELeave ) CDesCArrayFlat( 
            KUSBClassChangeUISupportedModesGranularity );
    iUSBAllItemsArrayActive = new ( ELeave ) CDesCArrayFlat( 
            KUSBClassChangeUISupportedModesGranularity );
    iPopUpItems = new ( ELeave ) CDesCArrayFlat( 
            KUSBClassChangeUISupportedModesGranularity );
    CArrayPtrFlat<CGulIcon>* iconArray = new(ELeave) CArrayPtrFlat<CGulIcon>(KUSBClassChangeUIIconGranularity);  
    CleanupStack::PushL( iconArray );
       
    // prepare the listbox items and the tooltip strings for the USB UI view
    iModelRef->SupportedUSBModeArraysL( *iUSBAllItemsArray, *iUSBAllItemsArrayDefault, *iUSBAllItemsArrayActive, *iPopUpItems, *iconArray);

    // Create DoubleLargeStyleListBox for USB UI view
    //
    iUSBMainListBox = new ( ELeave ) CAknDoubleLargeStyleListBox;
    iUSBMainListBox->SetContainerWindowL( *this );
    iUSBMainListBox->ConstructL( this, EAknListBoxMarkableList );

    // Set up scroll bar for the Listbox
    iUSBMainListBox->CreateScrollBarFrameL( ETrue );
    iUSBMainListBox->ScrollBarFrame()->SetScrollBarVisibilityL( 
        CEikScrollBarFrame::EOn, CEikScrollBarFrame::EAuto );
    
    // As soon as the list is created the tooltip info of the first item in the list is shown
    
   
    //creating a handle to the icon array and passing its ownership to data object
    //no need to handle its deletion
    iUSBMainListBox->ItemDrawer()->FormattedCellData()->SetIconArray(iconArray);
    CleanupStack::Pop(iconArray);
    
    iUSBMainListBox->Model()->SetItemTextArray( this );
    iUSBMainListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    iUSBMainListBox->SetListBoxObserver( this );

    SetRect( iViewRef->ClientRect() );
   
    const RArray<TInt> usbIds = iModelRef->GetUsbIds();
    TInt currentMode = usbIds.Find(iViewRef->USBMode());
    ShowPopUpsL(currentMode);
    ActivateL();
    
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::ConstructL complete"));
    }

// ----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CUSBClassChangeUIPluginContainer::CUSBClassChangeUIPluginContainer( 
    CUSBClassChangeUIPluginView* aView,
    CUSBClassChangeUIPluginModel* aModel) :
    iViewRef(aView), iModelRef(aModel)
    {
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
CUSBClassChangeUIPluginContainer::~CUSBClassChangeUIPluginContainer()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::Destructor"));

	delete iUSBMainListBox;
	
	delete iUSBAllItemsArray;
	delete iUSBAllItemsArrayDefault;
	delete iUSBAllItemsArrayActive;
	delete iPopUpItems;
	
    delete iPopupController;
	FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::Destructor complete"));
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::SizeChanged()
// Called by framework when the view size is changed
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginContainer::SizeChanged()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::SizeChangedL()"));
    
    iUSBMainListBox->SetRect( Rect() ); 
    iUSBMainListBox->SetFocus( ETrue );
    
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::SizeChangedL complete"));
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::FocusChanged()
// Called by framework when the focus is changed
// ----------------------------------------------------------------------------
//    
void CUSBClassChangeUIPluginContainer::FocusChanged( TDrawNow /*aDranow*/ )
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::FocusChanged()"));
    if( iUSBMainListBox)
        {
        iUSBMainListBox->SetFocus( IsFocused() );
        }
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::FocusChanged() complete"));    
    }
    
// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::CountComponentControls() const
// Returns the number of controls inside this container
// ----------------------------------------------------------------------------
//
TInt CUSBClassChangeUIPluginContainer::CountComponentControls() const
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::CountComponentControls()"));
    
    return KUSBClassChangeUIComponentCount; 
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::ComponentControl(TInt aIndex) const
// Returns the control inside this container. Note that this function is
// inherited from CCoeControl and thus must be const even it returns a pointer,
// which can be used for modifying the class. 
// ----------------------------------------------------------------------------
//
CCoeControl* CUSBClassChangeUIPluginContainer::ComponentControl( 
    TInt aIndex ) const
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::ComponentControl()"));
    
    switch ( aIndex )
        {
        case 0:
            {
            return iUSBMainListBox;
            }
        default:
            {
            return NULL;
            }            
        }
    }


// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::OfferKeyEventL
// Redirects keypresses to the listbox
// ----------------------------------------------------------------------------
//
TKeyResponse CUSBClassChangeUIPluginContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,TEventCode aType )
    {
    TInt maxItemcount = iModelRef->PersonalityCount();
    TInt maxIndex = maxItemcount-1;
    
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::OfferKeyEventL()"));
    TBool isfocus =  iViewRef->MenuBar()->ItemSpecificCommandsEnabled();   
    if ( isfocus && aType == EEventKey && 
            (aKeyEvent.iCode == EKeyDownArrow || aKeyEvent.iCode == EKeyUpArrow) )
            {
            iCurrentItemIndex = iUSBMainListBox->CurrentItemIndex();
            if( aKeyEvent.iCode == EKeyDownArrow )
                {
                if( iCurrentItemIndex >= maxIndex )
                    {
                    iCurrentItemIndex = 0;
                    }
                else
                    {
                    iCurrentItemIndex++;
                    }
                }
            else // ( aKeyEvent.iCode == EKeyUpArrow )    
                {
                if( iCurrentItemIndex <= 0 )
                    {
                    iCurrentItemIndex = maxIndex;
                    }
                else
                    {
                    iCurrentItemIndex--;
                    }
                }
            ShowPopUpsL( iCurrentItemIndex );
            iCurrentItemIndex = iUSBMainListBox->CurrentItemIndex();
            }

   
          TKeyResponse res = iUSBMainListBox->OfferKeyEventL( aKeyEvent, aType );
           FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::OfferKeyEventL complete"));

          return res;
    }

//------------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::showpopupsFinal(TInt &aValue)
// Display the popups
//------------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginContainer::ShowPopUpsL(TInt aIndex)
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::ShowPopUpsL "));
    iPopupController->SetTimePopupInView( KMaxVisibleTime);
    TPtrC chosen = iPopUpItems->MdcaPoint(aIndex);
    iPopupController->SetTextL( chosen );
    iPopupController->SetPositionByHighlight( TRect( 
                   iUSBMainListBox->View()->ItemPos( aIndex + 1 ), 
                   iUSBMainListBox->View()->ItemSize() ) );
    iPopupController->SetTimeDelayBeforeShow( KMsToWaitBeforePopup );
    iPopupController->ShowInfoPopupNote();
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::ShowPopUpsL complete"));
  
}

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::HandleListBoxEventL(
//     CEikListBox* aListBox,TListBoxEvent aEventType)
// Handle events from the ListBox
// ----------------------------------------------------------------------------
//    
void CUSBClassChangeUIPluginContainer::HandleListBoxEventL(
    CEikListBox* /*aListBox*/,TListBoxEvent aEventType )
   {
   FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::HandleListBoxEventL()"));
   
    __ASSERT_DEBUG( iViewRef!=0, PANIC( EUSBUIPanicIllegalValue ) );
    
    //Selection key pressed, when MSK is not enabled. Or touch stylus 
    //double clicked.
    //
    if( AknLayoutUtils::PenEnabled() )
           {
           switch ( aEventType )
               {
               case EEventItemSingleClicked:
                   {
                   FLOG(_L("[CUSBClassChangeUIPlugin]\t HandleListBoxEventL()EEventItemSingleClicked"));
                   iViewRef->HandleCommandL( EUSBUICmdSelect );
                   break;
                   }
               case EEventPanningStarted:
               case EEventPanningStopped:
               case EEventFlickStarted:
               case EEventFlickStopped:
               case EEventPenDownOnItem:
               case EEventItemDraggingActioned:   
                   {
                   ShowPopUpsL (CurrentItemIndex()) ;
                   break;
                   }
              default:
                     break;
                     }
                }
    if( (aEventType == EEventEnterKeyPressed) || 
        (aEventType == EEventItemDoubleClicked) ) //for touch support
            {
            iViewRef->HandleCommandL( EUSBUICmdSelect );
            }
        
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::HandleListBoxEventL complete"));
	}

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::MdcaCount()
// ----------------------------------------------------------------------------
//
TInt CUSBClassChangeUIPluginContainer::MdcaCount() const
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::MdcaCount()"));
    TInt numberOfMainViewItems =iModelRef->PersonalityCount();
    FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::MdcaCount()= %d"), numberOfMainViewItems));
    return numberOfMainViewItems;
    }
    
// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::MdcaPoint()
// Main view listbox item formatter:
// Returns the current format string depending value
// of the item concerned.
// ----------------------------------------------------------------------------
//
TPtrC CUSBClassChangeUIPluginContainer::MdcaPoint( TInt aIndex ) const
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::MdcaPoint()"));
    
    TPtrC chosen;
    const RArray<TInt> usbIds = iModelRef->GetUsbIds();
    TInt currentIndex = usbIds.Find(iViewRef->USBMode());
    if (aIndex == currentIndex)
        {
        if(iViewRef->IsUSBDeviceConnected())
            {
            FLOG(_L("[CUSBClassChangeUIPlugin]\t MdcaPoint()The selected mode and usbconnected"));
            chosen.Set(iUSBAllItemsArrayActive->MdcaPoint(aIndex));
            }
        else
            {
            FLOG(_L("[CUSBClassChangeUIPlugin]\t MdcaPoint()The selected mode and usb not connected"));
            chosen.Set(iUSBAllItemsArrayDefault->MdcaPoint(aIndex));
            }
        }
    else
        {
        FLOG(_L("[CUSBClassChangeUIPlugin]\t MdcaPoint()not selected mode"));
        chosen.Set(iUSBAllItemsArray->MdcaPoint(aIndex));
        }
     
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::MdcaPoint complete"));
    
    return chosen;
    }
    
// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::SettingChanged
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginContainer::SettingChanged()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::SettingChangedL()"));
    
	iUSBMainListBox->DrawDeferred();
    
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::SettingChangedL complete"));
    }
    
// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::GetHelpContext
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginContainer::GetHelpContext(TCoeHelpContext& aContext) const
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::GetHelpContext()"));
    
    aContext.iMajor = KUidUSBClassChangeUI;
    aContext.iContext = KUSB_HLP_MAIN;
    
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::GetHelpContext complete"));
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::HandleResourceChange
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginContainer::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType ); 
    if( aType == KEikDynamicLayoutVariantSwitch )
        {
        SetRect( iViewRef->ClientRect() );
        DrawNow(); 
        }
    else 
        {
        if ( aType == KAknsMessageSkinChange )
            {
            iUSBMainListBox->HandleResourceChange( aType ); 
            }
        }
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginContainer::CurrentItemIndex
// ----------------------------------------------------------------------------
//
TInt CUSBClassChangeUIPluginContainer::CurrentItemIndex()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginContainer::CurrentItemIndex()"));

    return iUSBMainListBox->CurrentItemIndex();
    }

// End of File  
