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
* Description:  USB dialer activation watcher class
 *
*/


#include <w32std.h>  
#include <eikenv.h> 
#include <coeaui.h> 
#include <vwsdef.h>


#include "usbuinotifdialerwatcher.h"
#include "debug.h"
#include "usbuinotifdebug.h"

const TUid KDiallerUid  = { 0x100058B3 };
const TUid KDiallerViewId = { 0x10282D81 };
const TUid KDiallerViewCommand  = { 0x1 };



// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CUsbuinotifDialerWatcher* CUsbuinotifDialerWatcher::NewL( MDialerNotifier* aDialerNotifier )
    {
    FLOG(_L("[USBUINOTIF]\t CUsbuinotifDialerWatcher::NewL"))
    CUsbuinotifDialerWatcher* self = CUsbuinotifDialerWatcher::NewLC( aDialerNotifier);
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//    
CUsbuinotifDialerWatcher* CUsbuinotifDialerWatcher::NewLC( MDialerNotifier* aDialerNotifier )
    {
    FLOG(_L("[USBUINOTIF]\t CUsbuinotifDialerWatcher::NewLC"))
    CUsbuinotifDialerWatcher* self = new ( ELeave ) CUsbuinotifDialerWatcher( aDialerNotifier );
    CleanupStack::PushL( self );
    self->ConstructL(  );
    return self;
    }

// ---------------------------------------------------------------------------
// CUsbuinotifDialerWatcher()
// ---------------------------------------------------------------------------
//
CUsbuinotifDialerWatcher::CUsbuinotifDialerWatcher( MDialerNotifier* aDialerNotifier ) :
    iNotify( aDialerNotifier )
    {
    FLOG(_L("[USBUINOTIF]\t CUsbuinotifDialerWatcher::CUsbuinotifDialerWatcher"))
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CUsbuinotifDialerWatcher::~CUsbuinotifDialerWatcher()
    {
    FLOG(_L("[USBUINOTIF]\t CUsbuinotifDialerWatcher Destructor"))
    CCoeAppUi* appui=CEikonEnv::Static()->AppUi();
    appui->RemoveFromStack(this);
    delete iwsSession;
    }
    
// ---------------------------------------------------------------------------
// ConstructL()
// ---------------------------------------------------------------------------
//
void CUsbuinotifDialerWatcher::ConstructL()
    {    
    FLOG(_L("[USBUINOTIF]\t CUsbuinotifDialerWatcher ConstructL"));    
    CCoeAppUi* appui=CEikonEnv::Static()->AppUi();
    appui->AddToStackL(this,ECoeStackPriorityCba);    
    }

// ---------------------------------------------------------------------------
// HandleViewEventL()
// ---------------------------------------------------------------------------
//
void CUsbuinotifDialerWatcher::HandleViewEventL(const TVwsViewEvent& /*aEvent*/)
    {
    FLOG((_L("[USBUINOTIF]\t HandleViewEventL ")));
    
    if ( !iNotified )
        {
        iNotify->ReActivateDialog();
        iNotified=ETrue;        
        }
    FLOG(_L("[USBUINOTIF]\t HandleViewEventL Done"));
    }

// ---------------------------------------------------------------------------
// OfferKeyEventL
// ---------------------------------------------------------------------------
//
TKeyResponse CUsbuinotifDialerWatcher::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode /*aType*/)
    {     
    if (aKeyEvent.iScanCode == EStdKeyApplication0)
        {    
        if ( !iwsSession )
            {
            iwsSession = CVwsSessionWrapper::NewL(*this);
            }
        iNotified=EFalse;
        const TVwsViewId viewId(KDiallerUid, KDiallerViewId );
        TUid msgId = KDiallerViewCommand;            
        iwsSession->CreateActivateViewEvent( viewId, msgId, KNullDesC8  );            
        CCoeAppUi* appui=CEikonEnv::Static()->AppUi();                        
        iNotify->DialerActivated();
        iwsSession->NotifyNextDeactivation(viewId);  
        }
        
    return EKeyWasNotConsumed;
    }

