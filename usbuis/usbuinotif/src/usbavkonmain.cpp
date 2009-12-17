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
 * Description:  Methods for USB UI notifiers.
 *
 */

// INCLUDE FILES
#include <EikNotApi.h>
#include <eikenv.h>

#include "usbuincableconnectednotifier.h"
#include "usbuinqueriesnotifier.h"
#include "usbuinotifdebug.h"

// CONSTANTS
const TInt KUSBUINotifierArrayIncrement = 1;


// ================= EXPORTED FUNCTIONS =======================================

// ----------------------------------------------------------------------------
//
// Instantiate notifiers
//
// ----------------------------------------------------------------------------

LOCAL_C void CreateUSBUINotifiersL( 
    CArrayPtrFlat<MEikSrvNotifierBase2>* aNotifiers )
    {
    FLOG(_L("[USBUINOTIF]\t CreateUSBUINotifiersL"));
    
    CUSBUICableConnectedNotifier* cableConnectedNotifier = 
        CUSBUICableConnectedNotifier::NewL();
    CleanupStack::PushL( cableConnectedNotifier );
    aNotifiers->AppendL( cableConnectedNotifier );
    CleanupStack::Pop( cableConnectedNotifier );


    FLOG(_L("[USBUINOTIF]\t CreateUSBUINotifiersL completed"));
    }

// ----------------------------------------------------------------------------
//
// Lib main entry point: Creates a notifiers array.
//
// ----------------------------------------------------------------------------

EXPORT_C CArrayPtr<MEikSrvNotifierBase2>* NotifierArray()
    {
    FLOG(_L("[USBUINOTIF]\t NotifierArray"));
    
    CArrayPtrFlat<MEikSrvNotifierBase2>* notifiers = NULL;
    
    TRAPD( err, notifiers = 
        new (ELeave)CArrayPtrFlat<MEikSrvNotifierBase2>( 
            KUSBUINotifierArrayIncrement ));
            
    if ( err == KErrNone )
        {
        if( notifiers )
            {
            TRAPD( errCreate, CreateUSBUINotifiersL( notifiers ));
            if( errCreate )
                {
                TInt count = notifiers->Count();
                while(count--)
                    {
                    (*notifiers)[count]->Release();
                    }
                delete notifiers;
                notifiers = NULL;
                }
                
            FLOG(_L("[USBUINOTIF]\t NotifierArray completed"));
            }
        }
    else
        {
        FLOG(_L("[USBUINOTIF]\t NotifierArray Failed!!!"));
        }  
        
    return notifiers;
    }

//  End of File
