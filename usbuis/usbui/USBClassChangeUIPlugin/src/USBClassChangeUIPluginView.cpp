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
* Description:  This the implementation of the view class
*
*/


// INCLUDE FILES

#include <aknradiobuttonsettingpage.h>
#include <akntitle.h>
#include <bautils.h>         // BAFL utils (for language file)
#include <StringLoader.h> 
#include <usbui.mbg>
#include <featmgr.h>
#include <hlplch.h>         // Help launcher
#include <usbpersonalityids.h>
#include <gsparentplugin.h>
#include <gsfwviewuids.h>
#include <USBClassChangeUIPlugin.rsg> // Resources

#include "USBClassChangeUIPluginView.h"
#include "USBClassChangeUIPluginDebug.h"
#include "USBClassChangeUIPlugin.h"
#include "USBClassChangeUIPluginContainer.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CUSBClassChangeUIPluginView::NewPluginL
// Entry point for CGSPluginInterface.
// Used by General Settings Application
// ---------------------------------------------------------
//
CUSBClassChangeUIPluginView* CUSBClassChangeUIPluginView::NewPluginL()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::NewPluginL()"));
    
    CUSBClassChangeUIPluginView* self = new ( ELeave ) CUSBClassChangeUIPluginView();
    
    self->iLoadAsGSPlugin = ETrue;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
       
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::NewPluginL() completed"));
    
    return self;
    }
    
// ---------------------------------------------------------
// CUSBClassChangeUIPluginView::NewL
// Entry point for CGSPluginInterface.
// Used by USBClassChangeUIPluginAppUI.
// ---------------------------------------------------------
//
CUSBClassChangeUIPluginView* CUSBClassChangeUIPluginView::NewL()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::NewL()"));
    
    CUSBClassChangeUIPluginView* self = new ( ELeave ) CUSBClassChangeUIPluginView();
    
    self->iLoadAsGSPlugin = EFalse;
     
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::NewL() completed"));
    
    return self;
    }    
    
// ---------------------------------------------------------
// CUSBClassChangeUIPluginView::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------
//
void CUSBClassChangeUIPluginView::ConstructL()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::ConstructL()"));

    iEikEnv = CEikonEnv::Static();

    TFileName filename;
    filename += KFileDrive;
    filename += KDC_RESOURCE_FILES_DIR; 
    filename += KResourceFileName;
    BaflUtils::NearestLanguageFile(iEikEnv->FsSession(),filename);
    iResourceFileFlag = iEikEnv->AddResourceFileL(filename);
    
    //"Back" or "Exit" right-softkey resource
    BaseConstructL( iLoadAsGSPlugin ? 
        R_USBUI_MAIN_VIEW_GS_PLUGIN : R_USBUI_MAIN_VIEW );
    
    iAppUIRef = AppUi();
    
    iModelRef = CUSBClassChangeUIPluginModel::NewL();
    iModelRef->SetSettingsObserver(this);

    FeatureManager::InitializeLibL();
    iHelpSupported = FeatureManager::FeatureSupported( KFeatureIdHelp );
    FeatureManager::UnInitializeLib(); 
    
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::ConstructL() completed"));
    }
     
// ---------------------------------------------------------
// Destructor.
// ---------------------------------------------------------
//
CUSBClassChangeUIPluginView::~CUSBClassChangeUIPluginView()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::~CUSBClassChangeUIPluginView()"));

    if ( iContainer )
        {
        if ( iAppUIRef )
            {
            iAppUIRef->RemoveFromViewStack( *this, iContainer );
            }
        delete iContainer;
            }

    if ( iEikEnv )
        {
        iEikEnv->DeleteResourceFile ( iResourceFileFlag );
        }
    
    if ( iModelRef )
        {
        delete iModelRef;    
        }
   
    if ( !iLoadAsGSPlugin )
        {
        SignalDestruction();
        }
    
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::~CUSBClassChangeUIPluginView() completed"));
    }
    
// ---------------------------------------------------------
// TUid CUSBClassChangeUIPluginView::Id
// ---------------------------------------------------------
//
TUid CUSBClassChangeUIPluginView::Id() const
    {
    if ( iLoadAsGSPlugin )
    	{
	    return KGsUSBUIPluginUid; //same as ecom impl UID, required by GS interface.
    	}
    else
    	{
    	return KUSBUIMainViewId;  //view id
    	}
    }
    
// ---------------------------------------------------------
// CUSBClassChangeUIPluginView::HandleCommandL
// Redirects commands to the appui class.
// ---------------------------------------------------------
//
void CUSBClassChangeUIPluginView::HandleCommandL(TInt aCommand)
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::HandleCommandL()"));

    switch ( aCommand )
        {
        case EAknSoftkeyExit:
        case EEikCmdExit:
            {
            iAppUIRef->Exit();
            break;
            }
        
        case EAknSoftkeyBack:
            /** 
            * UPDATE THIS if this view has a sub-view. 
            * In this case constant UID must be used instead of the
            * previous view - otherwise the "back" might return to the 
            * sub-view. See GSFWViewUIDs.h for constant UIDs.
            */
            iAppUIRef->ActivateLocalViewL( iPrevViewId.iViewUid );
            break; 
            
        case EUSBUICmdHelp:
        case EAknCmdHelp:
            {
            HlpLauncher::LaunchHelpApplicationL( 
                iEikonEnv->WsSession(), iAppUIRef->AppHelpContextL() );
            break;
            }
      
        case EUSBUICmdSelect:
        case EUSBUICmdMskSelect:
            {
             TInt CurrentIndex = iContainer->CurrentItemIndex() ;
             HandleCmdChangeUSBModeL(CurrentIndex);
             break;
            }

        default:
            {
            FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginAppUi::HandleCommandL default switch"));    
            break;
            }
        }
    }
    
// ---------------------------------------------------------
// CUSBClassChangeUIPluginView::DoActivateL
// ---------------------------------------------------------
//
void CUSBClassChangeUIPluginView::DoActivateL( const TVwsViewId& aPrevViewId,
                                 TUid /*aCustomMessageId*/,
                                 const TDesC8& /*aCustomMessage*/)
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::DoActivateL()"));

    iPrevViewId = aPrevViewId;
    if( !iContainer )
        {
        iContainer = CUSBClassChangeUIPluginContainer::NewL(this, iModelRef);
        iContainer->SetMopParent( this ); // MObjectProvider
    
        // Setup the title
        //
        HBufC* usbTitleMain = iCoeEnv->AllocReadResourceLC( R_USB_TITLE );
        CEikStatusPane* statusPane = StatusPane();
        CAknTitlePane* titlePane;
        titlePane = ( CAknTitlePane* ) statusPane->ControlL(TUid::Uid( EEikStatusPaneUidTitle ));
        titlePane->SetTextL( *usbTitleMain );
        CleanupStack::PopAndDestroy(usbTitleMain);
        iAppUIRef->AddToStackL( *this, iContainer );
        }
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::DoActivateL() completed"));
    }
    
// ---------------------------------------------------------
// CUSBClassChangeUIPluginView::DoDeactivate
// ---------------------------------------------------------
//
void CUSBClassChangeUIPluginView::DoDeactivate()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::DoDeactivate()"));

    if ( iContainer )
        {
        iAppUIRef->RemoveFromStack( iContainer );
        delete iContainer;
        iContainer = NULL;
        }


    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::DoDeactivate() completed"));
    }
 
// ---------------------------------------------------------
// CUSBClassChangeUIPluginView::GetCaptionL
// Method for getting caption of this plugin.
// ---------------------------------------------------------
//   
void CUSBClassChangeUIPluginView::GetCaptionL (TDes& aCaption) const
    {   
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::GetCaptionL()")); 
    // The resource file is already opened, see ConstructL()
    //
    HBufC* result = StringLoader::LoadL( R_GS_USBUI_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;  
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::GetCaptionL() completed"));
    }
    
// ---------------------------------------------------------
// CUSBClassChangeUIPluginView::GetValue
// Function for getting plugin's value for a certain key.
// ---------------------------------------------------------
//   
void CUSBClassChangeUIPluginView::GetValue(const TGSPluginValueKeys aKey,
                                           TDes& aValue)
    {
    switch( aKey )
        {
        case EGSPluginKeySettingsItemValueString:
            {
            // Get Id pin state of UsbOtg-capable device
            TBool hasPeripheralEnabled = EFalse;
            // Ignore error code
            TInt ret = iModelRef->HasPeripheralEnabled(hasPeripheralEnabled);
            FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::GetValue - HasPeripheralEnabled=%d"), ret));

            TInt resId = 0;
            if (hasPeripheralEnabled)
                {
                resId = R_CP_DETAIL_USB_OTG_CONN;
                }
            else if (IsUSBDeviceConnected())
                {
                TInt mode = USBMode();
                switch (mode)
                    {
                    case KUsbPersonalityIdPCSuite:
                    case KUsbPersonalityIdPCSuiteMTP:
                        resId = R_CP_DETAIL_USB_CONN_PC_SUITE;
                        break;
                    case KUsbPersonalityIdMS:
                        resId = R_CP_DETAIL_USB_CONN_MASS_STORAGE;
                        break;
                    case KUsbPersonalityIdPTP:
                        resId = R_CP_DETAIL_USB_CONN_PICTURE_TRANSFER;
                        break;
                    case KUsbPersonalityIdMTP:
                        resId = R_CP_DETAIL_USB_CONN_MEDIA_TRANSFER;
                        break;
                    case KUsbPersonalityIdModemInst:
                        resId = R_CP_DETAIL_USB_CONN_MODEM_INST;
                        break;
					
					case KUsbPersonalityIdRNDIS:
                        resId = R_CP_DETAIL_USB_CONN_RNDIS;
                        break;
                    default:
                        // Leave value empty
                        break;
                    }
                }
            else
                {
                resId = R_CP_DETAIL_USB_NOT_CONNECTED;
                }

            if (resId)
                {
                    TRAP_IGNORE(
                        HBufC* text = StringLoader::LoadL(resId);
                        aValue.Append(*text);
                        delete text;
                    );
                }
            }
            break;

        default:
            CGSPluginInterface::GetValue(aKey, aValue);
            break;
        }
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginView::SettingChanged()
// A setting has been changed. 
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginView::SettingChanged()
    {
        if (iContainer)
            {
                iContainer->SettingChanged();
            }

        // Personality changes only affect USB UI when USB device is connected
        if (IsUSBDeviceConnected())
            {
                UpdateParentView();
            }
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginView::DeviceStateChanged()
// USB device state has changed.
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginView::DeviceStateChanged(TUsbDeviceState /*aPrevState*/,
                                                     TUsbDeviceState aState)
    {
		//Transitions to EUsbDeviceStateUndefined, EUsbDeviceStateConfigured 
		//and EUsbDeviceStateAttached states update UI view.
        if (aState == EUsbDeviceStateConfigured ||
            aState == EUsbDeviceStateUndefined ||
			aState == EUsbDeviceStateAttached)
            {
                UpdateParentView();
                if (iContainer)
                           {
                               iContainer->SettingChanged();
                           }
            }
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginView::OtgHostIdPinStateChanged
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginView::OtgHostIdPinStateChanged(TBool aIsIdPinOn)
    {
    if (!aIsIdPinOn)
        {
        UpdateParentView();
        }
    // else, Ignore Id pin on event, host event to be checked further
    }

// ----------------------------------------------------------------------------
// From MUSBOtgHostStateObserver
// Handle host event notification
// ----------------------------------------------------------------------------
void CUSBClassChangeUIPluginView::HostEventNotify(TDeviceEventInformation /*aEventInfo*/)
    {
    UpdateParentView();
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginView::USBMode()
// Gets the current USB mode from model
// ----------------------------------------------------------------------------
//
TInt CUSBClassChangeUIPluginView::USBMode() const
    {
    return iModelRef->USBMode();
    }
// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginView::SetUSBModeL(TInt)
// Sets the current USB mode through model
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginView::SetUSBModeL( TInt aMode )
    {
    FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::SetUSBModeL()"));
    
    iModelRef->SetUSBModeL( aMode );
    
    FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::SetUSBModeL complete"));
    }   
 
// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginView::CreateIconL()
// Creates icon for General Settings application
// ----------------------------------------------------------------------------
//  
CGulIcon* CUSBClassChangeUIPluginView::CreateIconL( const TUid aIconType )
{
    FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::CreateIconL() - aIconType=0x%x"), aIconType.iUid));

    CGulIcon* icon = NULL;
        
    if( aIconType == KGSIconTypeLbxItem )
        {
        TFileName usbUiIconFilename( KFileDrive );
        usbUiIconFilename += KDC_APP_BITMAP_DIR;
        usbUiIconFilename += KUSBUIconFileName;
      

        // Get peripheral connected state of UsbOtg-capable device
        TBool hasPeripheralEnabled = EFalse;
        TInt ret = iModelRef->HasPeripheralEnabled(hasPeripheralEnabled);
        // In case of failure of getting the status, hasPeripheralEnabled is EFalse.
        // So ignore the error code here.
        FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::CreateIconL - HasPeripheralEnabled=%d"), ret));

        if (hasPeripheralEnabled || IsUSBDeviceConnected())
            {
            icon = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(), 
                KAknsIIDQgnPropSetAppsUsb, usbUiIconFilename, 
                EMbmUsbuiQgn_prop_set_apps_usb, 
                EMbmUsbuiQgn_prop_set_apps_usb_mask );
            }
        else
            {
            icon = AknsUtils::CreateGulIconL( AknsUtils::SkinInstance(), 
                KAknsIIDQgnPropSetAppsUsbOff, usbUiIconFilename, 
                EMbmUsbuiQgn_prop_set_apps_usb_off, 
                EMbmUsbuiQgn_prop_set_apps_usb_off_mask );
            }
        }
    else 
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }
        
    FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::CreateIconL() completed"));
    return icon;
}


// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginView::HandleCmdChangeUSBModeL()
// sets the selected usb mode 
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginView::HandleCmdChangeUSBModeL(TInt aMode)
    {
    FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::HandleCmdChangeUSBModeL()"));
    const RArray<TInt> usbIndexes = iModelRef->GetUsbIds(); 
    SetUSBModeL( usbIndexes[aMode] );
    FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::HandleCmdChangeUSBModeL() completed"));
    }


// ----------------------------------------------------------------------------
// Remove Help from options menu, if the feature is not supported. 
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginView::DynInitMenuPaneL( TInt /*aResId*/, CEikMenuPane* aMenu )
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::DynInitMenuPaneL()"));
	//dim help if it is not supported and it exists
	if (!iHelpSupported)
	    {
    	TInt tmp;
        if (  aMenu->MenuItemExists(EUSBUICmdHelp, tmp) )
            {			
            aMenu->SetItemDimmed(EUSBUICmdHelp, ETrue);
            }
        }
    FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginView::DynInitMenuPaneL() completed"));
    }
    
// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginView::IsUSBDeviceConnected()
// Checks whether USB device is connected.
// ----------------------------------------------------------------------------
//
TBool CUSBClassChangeUIPluginView::IsUSBDeviceConnected()
    {
        TUsbDeviceState state = EUsbDeviceStateUndefined;
        return (iModelRef->GetDeviceState(state) == KErrNone && 
                state != EUsbDeviceStateUndefined);
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginView::UpdateParentView()
// Updates parent view if we are loaded by GS plugin.
// Copied from irappgspluginview.cpp
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginView::UpdateParentView()
    {
        if (iLoadAsGSPlugin)
            {
                // Note: GetActiveViewId returns KErrNotFound if GS application
                // is running in the background.
		        TVwsViewId vid;
		        AppUi()->GetTopViewId(vid);
		        if (vid.iViewUid == KGSConPluginUid)
			        {
			            CGSParentPlugin* parent;
			            parent = (CGSParentPlugin*)AppUi()->View(vid.iViewUid);
			            parent->UpdateView();
			        }
            }
    }

// End of File
