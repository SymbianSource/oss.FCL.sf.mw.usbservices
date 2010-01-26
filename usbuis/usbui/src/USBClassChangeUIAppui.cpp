/*
* Copyright (c) 2005, 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of AppUI class
*
*/


// INCLUDE FILES

#include    "USBClassChangeUIAppUi.h"
#include    "USBClassChangeUIDocument.h"
#include    "USBClassChangeUIDebug.h"
#include    "USBClassChangeUIPluginInterface.h"

#include    <avkon.hrh>

#include    <gsplugininterface.h>

const TUid KUidImpl = { 0x1027478F };   // implementation UID of the plugin

// ================= MEMBER FUNCTIONS =========================================
//

// ----------------------------------------------------------------------------
// CUSBClassChangeUIAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIAppUi::ConstructL()
    {
    FLOG(_L("[CUSBClassChangeUI]\t CUSBClassChangeUIAppUi::ConstructL()"));
    
    BaseConstructL( EAknEnableSkin | EAknEnableMSK |EAknSingleClickCompatible);
    
    // Create the view using the GSPluginInterface
    //   
    CUSBClassChangeUIPluginInterface* mainView = 
        CUSBClassChangeUIPluginInterface::NewL( KUidImpl, this );
    
    CleanupStack::PushL( mainView );
    AddViewL( mainView ); // Transfer ownership to CAknViewAppUi
    CleanupStack::Pop( mainView );
    
    ActivateLocalViewL( mainView->Id() );
        
    FLOG( _L("[CUSBClassChangeUI]\t CUSBClassChangeUIAppUi::ConstructL complete"));
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIAppUi::~CUSBUIAppUi()
// Destructor
// Frees reserved resources
// ----------------------------------------------------------------------------
//
CUSBClassChangeUIAppUi::~CUSBClassChangeUIAppUi()
    {
    FLOG( _L("[CUSBClassChangeUI]\t CUSBClassChangeUIAppUi::Destructor (empty) complete"));
    }

// ----------------------------------------------------------------------------
// CUSBClassChangeUIAppUi::HandleCommandL(TInt aCommand)
// Takes care of command handling
// ----------------------------------------------------------------------------
//
void CUSBClassChangeUIAppUi::HandleCommandL(TInt aCommand)
    {
    FLOG( _L("[CUSBClassChangeUI]\t CUSBClassChangeUIAppUi::HandleCommandL()"));
    switch ( aCommand )
        {
        case EAknSoftkeyExit:
        case EEikCmdExit:
            {
            Exit();
            break;
            }    
         
        default:
            {
            FLOG( _L("[CUSBClassChangeUI]\t CUSBClassChangeUIAppUi::HandleCommandL default switch"));    
            break;
            }   
        }
        
    FLOG( _L("[CUSBClassChangeUI]\t CUSBClassChangeUIAppUi::HandleCommandL complete"));
    }
// End of File
