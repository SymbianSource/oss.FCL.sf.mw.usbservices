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
* Description:  This is the interface to all the settings.
*
*/

// INCLUDE FILES

#include <centralrepository.h> 
#include <usb.h>
#include <usbwatcher.h>
#include <UsbWatcherInternalCRKeys.h>
#include <USBClassChangeUIPlugin.rsg> // Resources
#include <coemain.h>
#include <usbpersonalityids.h>
#include <avkon.mbg>
#include <usbui.mbg>
#include <aknappui.h>
#include <AknIconArray.h>
#include <aknlists.h>
#include <featmgr.h>

#include "USBClassChangeUIPluginModel.h"
#include "USBClassChangeUIPluginDebug.h"
#include "USBClassChangeUIPlugin.h"
#include "USBClassChangeUIPluginView.h"

_LIT( KFirstTab, "%d\t" );
_LIT( KSecondTab, "\t" );
_LIT(KUSBUIEmptyString, "0\t \t "); 

#if defined(__WINS__) && !defined(__USB_EMULATION__)
    // There are two situations under emulator:
    // 1. Do not use UsbManager and UsbWatcher, which is the normal case, and
    // 2. Use UsbManagerDummy and UsbWatcherDummy, for UI testing.
    // Comment the define line for case 2. 
   #define NO_USBWATCHER_USBMANAGER  
#endif //__WINS__

// ================= MEMBER FUNCTIONS ==========================================
//

CUSBClassChangeUIPluginModel::CUSBClassChangeUIPluginModel()
: CActive( EPriorityNormal ), iUsbWatcherConnected(EFalse)
    {
    CActiveScheduler::Add(this);
    }


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CUSBClassChangeUIPluginModel::~CUSBClassChangeUIPluginModel()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::Destructor"));
    
    Cancel();
    delete iCRPersonalityWatcher;
    delete iDeviceStateWatcher;
    delete iOtgHostStateWatcher;
    delete iRepository; 
    iUsbModeIndexes.Close(); 
#ifndef NO_USBWATCHER_USBMANAGER //UsbWatcher/UsbManager API N/A
    iUsbman.Close();
    iUsbWatcher.Close();  
    iDeviceIDs.Close();
#endif //NO_USBWATCHER_USBMANAGER
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::Destructor complete"));
    }


// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginModel::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginModel::ConstructL()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::ConstructL()"));

    iCRPersonalityWatcher = CUSBClassChangeUIPluginCRWatcher::NewL( *this, 
        KUsbWatcherPersonality);

    iUsbModeIndexes = RArray<TInt>(KUSBClassChangeUISupportedModesGranularity);
    iRepository = CRepository::NewL( KCRUidUsbWatcher );

#ifndef NO_USBWATCHER_USBMANAGER //UsbWatcher/UsbManager API N/A
    User::LeaveIfError(iUsbman.Connect());
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::ConstructL() iUsbman OK"));
    
    iDeviceStateWatcher = CUSBDeviceStateWatcher::NewL(*this, iUsbman);
    iOtgHostStateWatcher = CUSBOtgHostStateWatcher::NewL(*this, iUsbman);
    iDeviceIDs.ReserveL(1);
    
#endif //NO_USBWATCHER_USBMANAGER
    
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::ConstructL() complete"));
    }


// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginModel::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUSBClassChangeUIPluginModel* CUSBClassChangeUIPluginModel::NewL()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\tCUSBClassChangeUIPluginModel:NewL"));
    CUSBClassChangeUIPluginModel* self 
        = new (ELeave) CUSBClassChangeUIPluginModel();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self;
    }

// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginModel::SetUSBModeL
// Sets the Central Repository key to the parameter.
// -----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginModel::SetUSBModeL(TInt aMode)
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::SetUSBModeL()"));
    
    // Only change the value if necessary
    TInt usbMode = USBMode();
    if (usbMode != aMode)
        {
#ifndef NO_USBWATCHER_USBMANAGER //UsbWatcher/UsbManager API N/A
        // Change the personality asynchrously, result checked in RunL()
        if( IsActive() ) 
            {
            Cancel();
            }
        if ( (!iUsbWatcherConnected) && (iUsbWatcher.Connect() == KErrNone) )
            {
            iUsbWatcherConnected = ETrue;
            FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::SetUSBModeL iUsbWatcher connected"));
            }
        if (iUsbWatcherConnected)
            {
            FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::SetUSBModeL setting personality"));
            iUsbWatcher.SetPersonality(iStatus, aMode);
            SetActive();
            }
#endif //NO_USBWATCHER_USBMANAGER
        }
        
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::SetUSBModeL complete"));
    }

void CUSBClassChangeUIPluginModel::RunL()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::RunL()"));
    
    //Check the return value of SetPersonality()
    //Leave if KErrDiskFull
    if( iStatus == KErrDiskFull ) // Other errors not leave
        {
        User::Leave( KErrDiskFull );    
        }
    
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::RunL complete"));
    }
    
void CUSBClassChangeUIPluginModel::DoCancel()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::DoCancel()"));
    
    if (iUsbWatcherConnected)
        {
        iUsbWatcher.CancelSetPersonality();
        }

    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::DoCancel complete()"));
    }
    
// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginModel::USBMode
// Returns the value in Central Repository.
// -----------------------------------------------------------------------------
//
TInt CUSBClassChangeUIPluginModel::USBMode()
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::USBMode()"));
    TInt mode = 0;
    iRepository->Get(KUsbWatcherPersonality, mode);
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::USBMode complete"));
    return mode;
    }

// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginModel::GetDeviceState
// Gets the current device state
// -----------------------------------------------------------------------------
//
TInt CUSBClassChangeUIPluginModel::GetDeviceState(TUsbDeviceState& aState)
    {
#ifdef NO_USBWATCHER_USBMANAGER //UsbWatcher/UsbManager API N/A
        aState = EUsbDeviceStateUndefined;
        return KErrNone;
#else
        return iUsbman.GetDeviceState(aState);
#endif
    }

// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginModel::GetDescription
// Gets description for the specified USB mode (personality ID)
// -----------------------------------------------------------------------------
//
TInt CUSBClassChangeUIPluginModel::GetDescription(TInt aMode, HBufC*& aDescription)
    {
        return iUsbman.GetDescription(aMode, aDescription);
    }

// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginModel::UpdateMainContainerReference
// -----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginModel::SetSettingsObserver(MUSBUIObserver* aObserver)
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::SetSettingsObserver()"));
    iObserver = aObserver;
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::SetSettingsObserver complete"));
    }

// -----------------------------------------------------------------------------
// CUSBClassChangeUIPluginModel::GetSupportedUSBModesL
// Reads the supported USB Modes from USBManager
// The lists received as parameters are updated. 
// -----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginModel::SupportedUSBModeArraysL( 
        CDesCArrayFlat& aListBox,CDesCArrayFlat& aListBoxDefault,
        CDesCArrayFlat& aListBoxActive, CDesCArrayFlat& aPopUpItems,
        CArrayPtrFlat<CGulIcon>& aIconArray)
    {
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::SupportedUSBModeArraysL()"));  
    RArray<TInt> personalityIds;
    CleanupClosePushL( personalityIds );
    HBufC* usbModeListBox = NULL;
    HBufC* usbModeListBoxActive = NULL;
   
    // Allocate memory for descriptors to hold texts for listbox 
    usbModeListBox = HBufC::NewLC( KUsbStringDescStringMaxSize );
    TPtr usbModeListBoxPtr = usbModeListBox->Des();
       
    usbModeListBoxActive = HBufC::NewLC( KUsbStringDescStringMaxSize );
    TPtr usbModeListBoxActivePtr = usbModeListBoxActive->Des();

    HBufC* usbDefaultText = NULL;
    usbDefaultText = CCoeEnv::Static()->AllocReadResourceLC( R_USB_MODE_DEFAULT );
   
    HBufC* usbActiveText = NULL;
    usbActiveText = CCoeEnv::Static()->AllocReadResourceLC( R_USB_MODE_ACTIVE );
   
    //Check phone as modem is supported or not
    FeatureManager::InitializeLibL();
    TBool phoneAsModemSupported = FeatureManager::FeatureSupported(
        KFeatureIdUsbModem );
    FeatureManager::UnInitializeLib();
 
    // Get personality ids
     iPersonalityCount = 0;
#ifndef NO_USBWATCHER_USBMANAGER //UsbWatcher/UsbManager API N/A
    if (iUsbman.GetPersonalityIds(personalityIds) == KErrNone)
        {
        FLOG(_L("CUSBClassChangeUIPluginModel::SupportedUSBModeArraysL(): Personality Ids got"));              
        for (TInt i = 0; i < personalityIds.Count(); i++)
            {  
            FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t i= %d "), i));
            
            if (personalityIds[i] == KUsbPersonalityIdModemInst)
                {
                if (!phoneAsModemSupported)
                    {
                    FLOG(_L("Phone as modem is not supported!"));    
                    continue;
                    }
                }
				
			TUint32 property;
			TInt ret  =  iUsbman.GetPersonalityProperty(personalityIds[i], property);
			if (ret == KErrNone)
				{
				FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin] property\t i= 0x%x "), property));
				if (property & KUsbPersonalityPropertyHidden)
					{
					FLOG(_L("CUSBClassChangeUIPluginModel::SupportedUSBModeArraysL(): The Personality is hidden"));
					continue;
					}
				}
			else
				{
				FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin] Error to get the property\t i= %d "), ret));
				}

            HBufC* description = NULL;
            HBufC* detailDescription = NULL;
            
            if (iUsbman.GetDescription(personalityIds[i], description) == KErrNone)
                {
                CleanupStack::PushL(description);
                FLOG(_L("CUSBClassChangeUIPluginModel::SupportedUSBModeArraysL(): Description read"));
                //mode strings for setting page
                TPtr descriptionPtr = description->Des();                                        
                iUsbModeIndexes.Append(personalityIds[i]);
 
                //modes with labels for list box
                usbModeListBoxPtr.Zero();
                usbModeListBoxActivePtr.Zero();
                
                usbModeListBoxPtr.Format(KFirstTab, iPersonalityCount);
                usbModeListBoxPtr.Append(descriptionPtr);
                usbModeListBoxPtr.Append(KSecondTab);
                
                usbModeListBoxActivePtr.Copy(usbModeListBoxPtr);
                            
                aListBox.AppendL(usbModeListBoxPtr);
                                
                usbModeListBoxPtr.Append(*usbDefaultText);
                usbModeListBoxActivePtr.Append(*usbActiveText);
               
                aListBoxDefault.AppendL(usbModeListBoxPtr);
                aListBoxActive.AppendL(usbModeListBoxActivePtr);
                
                CleanupStack::PopAndDestroy(description); 
                //creating the icon list
                AddIconL (personalityIds[i], aIconArray);
                if (iUsbman.GetDetailedDescription(personalityIds[i], detailDescription) == KErrNone)
                	{
                	CleanupStack::PushL(detailDescription);
                	aPopUpItems.AppendL(detailDescription->Des());
                	CleanupStack::PopAndDestroy(detailDescription); 
               		}
                else
                	{ 
                	aPopUpItems.AppendL(KNullDesC);
                	}
                iPersonalityCount++;
                }
                
            }
        }
#endif // NO_USBWATCHER_USBMANAGER
 
    if (iPersonalityCount == 0)
        {
        // Add empty value
        AddIconL (iPersonalityCount, aIconArray);
        iUsbModeIndexes.Append(0);
        aPopUpItems.AppendL(KNullDesC);
        aListBox.AppendL(KUSBUIEmptyString);
        aListBoxDefault.AppendL(KUSBUIEmptyString);
        aListBoxActive.AppendL(KUSBUIEmptyString);
        }
    
    CleanupStack::PopAndDestroy( usbActiveText ); 
    CleanupStack::PopAndDestroy( usbDefaultText ); 
    CleanupStack::PopAndDestroy( usbModeListBoxActive ); 
    CleanupStack::PopAndDestroy( usbModeListBox ); 
    CleanupStack::PopAndDestroy( &personalityIds ); 
    
    FLOG(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::SupportedUSBModeArraysL() complete"));  
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginModel::AddIconL()
// creates the icon list 
// 
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginModel::AddIconL (TInt aMode, CArrayPtrFlat<CGulIcon>& aIconArray )
    {
    FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::AddIconL()"));
    TFileName usbUiIconFilename( KFileDrive );
    usbUiIconFilename += KDC_APP_BITMAP_DIR;
    usbUiIconFilename += KUSBUIconFileName;

    switch (aMode)
        {
        case KUsbPersonalityIdPCSuite:
        case KUsbPersonalityIdPCSuiteMTP:
            CreateAndAppendIconL( KAknsIIDQgnPropUsbPcsuite,
                    usbUiIconFilename,
                    EMbmUsbuiQgn_prop_usb_pcsuite,
                    EMbmUsbuiQgn_prop_usb_pcsuite_mask,
                    aIconArray);
            break;
        case KUsbPersonalityIdMS:
            CreateAndAppendIconL( KAknsIIDQgnPropUsbMemcLarge,
                    usbUiIconFilename,
                    EMbmUsbuiQgn_prop_usb_memc_large,
                    EMbmUsbuiQgn_prop_usb_memc_large_mask,
                    aIconArray);
            break;
        case KUsbPersonalityIdPTP:
            CreateAndAppendIconL( KAknsIIDQgnPropUsbPrint,
                    usbUiIconFilename,
                    EMbmUsbuiQgn_prop_usb_print,
                    EMbmUsbuiQgn_prop_usb_print_mask,
                    aIconArray);
            break;
        case KUsbPersonalityIdMTP:
            CreateAndAppendIconL( KAknsIIDQgnPropUsbMtp,
                    usbUiIconFilename,
                    EMbmUsbuiQgn_prop_usb_mtp,
                    EMbmUsbuiQgn_prop_usb_mtp_mask,
                    aIconArray);
            break;
        case KUsbPersonalityIdModemInst:
            CreateAndAppendIconL( KAknsIIDQgnPropUsbModem,
                    usbUiIconFilename,
                    EMbmUsbuiQgn_prop_usb_modem,
                    EMbmUsbuiQgn_prop_usb_modem_mask,
                    aIconArray);
            break;
        default:
            CreateAndAppendIconL( KAknsIIDQgnPropSetAppsUsb, 
                    usbUiIconFilename, 
                    EMbmUsbuiQgn_prop_set_apps_usb,
                    EMbmUsbuiQgn_prop_set_apps_usb_mask,
                    aIconArray);
           break;
       }
    FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::AddIconL() completed"));
    }
// ----------------------------------------------------
// CUSBClassChangeUIPluginContainer::CreateAndAppendIconL
// ----------------------------------------------------
void CUSBClassChangeUIPluginModel::CreateAndAppendIconL( 
    const TAknsItemID& aID,
    const TDesC& aFileName,
    const TInt aBitmapId,
    const TInt aMaskId,
    CArrayPtrFlat<CGulIcon>& aIconArray)
    {    
    FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::CreateAndAppendIconL"));

    CGulIcon* icon = AknsUtils::CreateGulIconL(AknsUtils::SkinInstance(),
                                    aID, aFileName, aBitmapId, aMaskId);
    
    CleanupStack::PushL(icon);    
    aIconArray.AppendL(icon);
    CleanupStack::Pop(icon);
    FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::CreateAndAppendIconL completed"));

    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginModel::GetPersonalityCount()
// Informs the container, that a setting has changed. 
// 
// ----------------------------------------------------------------------------
//
TInt CUSBClassChangeUIPluginModel::PersonalityCount()
    {
    FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t PersonalityCount= %d "), iPersonalityCount));
       return iPersonalityCount;
    }  
// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginModel::SettingChangedL()
// Informs the container, that a setting has changed. 
// 
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIPluginModel::SettingChangedL( TUint32 /*aKey*/ )
    {
    FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::SettingChangedL()"));
    
    if ( iObserver )
        {
        iObserver->SettingChanged();  
        }
        
    FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::SettingChangedL() completed"));
    }

// ----------------------------------------------------------------------------
// From MUSBOtgHostStateObserver
// Handle Otg Id pin on/off notification
// ----------------------------------------------------------------------------
void CUSBClassChangeUIPluginModel::OtgHostIdPinStateChanged(TBool aIsIdPinOn)
    {
    iDeviceIDs.Reset();
    if (iObserver)
        {
        iObserver->OtgHostIdPinStateChanged(aIsIdPinOn);
        }
    }

// ----------------------------------------------------------------------------
// From MUSBOtgHostStateObserver
// Handle host event notification
// ----------------------------------------------------------------------------
void CUSBClassChangeUIPluginModel::HostEventNotify(TDeviceEventInformation aEventInfo)
    {
    if (iObserver)
        {
        FLOG( _L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::HostEventNotify"));        
        FTRACE( FPrint(_L( "[CUSBClassChangeUIPlugin]\t iEventInfo.iDeviceId         = %d" ), aEventInfo.iDeviceId));
        FTRACE( FPrint(_L( "[CUSBClassChangeUIPlugin]\t iEventInfo.iEventType        = %d" ), aEventInfo.iEventType));
        FTRACE( FPrint(_L( "[CUSBClassChangeUIPlugin]\t iEventInfo.iError            = %d" ), aEventInfo.iError));
        FTRACE( FPrint(_L( "[CUSBClassChangeUIPlugin]\t iEventInfo.iDriverLoadStatus = %d" ), aEventInfo.iDriverLoadStatus));
        FTRACE( FPrint(_L( "[CUSBClassChangeUIPlugin]\t iEventInfo.iVid              = %d" ), aEventInfo.iVid));
        FTRACE( FPrint(_L( "[CUSBClassChangeUIPlugin]\t iEventInfo.iPid              = %d" ), aEventInfo.iPid));

        switch (aEventInfo.iEventType)
            {
            case EDeviceAttachment:
                {
                iObserver->HostEventNotify(aEventInfo);
                break;
                }
            case EDeviceDetachment:
                {
                TInt index = iDeviceIDs.Find(aEventInfo.iDeviceId);
                if (index >= 0)
                    {
                     iDeviceIDs.Remove(index);
                    }
                iObserver->HostEventNotify(aEventInfo);
                break;
                }
            case EDriverLoad:
                {
                switch (aEventInfo.iDriverLoadStatus)
                    {
                    case EDriverLoadSuccess:
                    case EDriverLoadPartialSuccess:
                        {
                        // Drivers are loaded more or less successfully
                        iDeviceIDs.Append(aEventInfo.iDeviceId);
                        iObserver->HostEventNotify(aEventInfo);
                        break;
                        }
                    }
                break;
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// Informs the observer that USB device state has changed.
// ----------------------------------------------------------------------------
void CUSBClassChangeUIPluginModel::DeviceStateChanged(
    TUsbDeviceState aPreviousState, TUsbDeviceState aCurrentState)
{
    if (iObserver)
        {
            iObserver->DeviceStateChanged(aPreviousState, aCurrentState);
        }
}

// ----------------------------------------------------------------------------
// CUSBClassChangeUIPluginModel::GetUsbIds()
// Return an RArray of the personality indexes
// 
// ----------------------------------------------------------------------------
//
RArray<TInt>& CUSBClassChangeUIPluginModel::GetUsbIds()
    {
    return iUsbModeIndexes; 
    }
// ----------------------------------------------------------------------------
// Returns if it's A-device and the driver to the attached pheripheral 
// is loaded successfully(or partially). 
// ----------------------------------------------------------------------------
//
TInt CUSBClassChangeUIPluginModel::HasPeripheralEnabled(TBool& aHasPeripheralEnabled)
    {
    aHasPeripheralEnabled = EFalse;
#ifdef NO_USBWATCHER_USBMANAGER     ////UsbWatcher/UsbManager API N/A 
    return KErrNone;
#else
    TBool idPinOn;
    TInt ret = iOtgHostStateWatcher->IsIdPinOn(idPinOn);
    if (ret != KErrNone || !idPinOn)
        {
        return ret;
        }
    
    //aHasPeripheralEnabled = (iDeviceIDs.Count() > 0 ? ETrue : EFalse);
    ret = iOtgHostStateWatcher->IsPeripheralConnected(aHasPeripheralEnabled); 
    if (ret != KErrNone)
        {
        return ret;
        }
    
    FTRACE(FPrint(_L("[CUSBClassChangeUIPlugin]\t CUSBClassChangeUIPluginModel::HasPeripheralEnabled - aHasPeripheralEnabled=%d"), aHasPeripheralEnabled));
    
    return KErrNone;
#endif
    }

// End of file
