/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for CUSBClassChangeUIPluginInterface class.
*
*/


// -----------------------------------------------------------------------------
// Default constructor.
// -----------------------------------------------------------------------------
//
inline CUSBClassChangeUIPluginInterface::CUSBClassChangeUIPluginInterface()
    {
    }


// -----------------------------------------------------------------------------
// Destructor. 
// -----------------------------------------------------------------------------
//
inline CUSBClassChangeUIPluginInterface::~CUSBClassChangeUIPluginInterface()
    {
    //REComSession::DestroyedImplementation can't be called, because the 
    //GS framework wants to do it. 
    }


// -----------------------------------------------------------------------------
// Create new plugin having the given UID.
// -----------------------------------------------------------------------------
//
inline CUSBClassChangeUIPluginInterface* CUSBClassChangeUIPluginInterface::NewL( 
    TUid aImplementationUid, 
    TAny* aInitParams )
    {
    TAny* ptr;
    TInt32 keyOffset = _FOFF( CUSBClassChangeUIPluginInterface, iDtor_ID_Key );

    ptr = REComSession::CreateImplementationL(
        aImplementationUid,
        keyOffset,
        aInitParams);    
     
    return static_cast< CUSBClassChangeUIPluginInterface* >( ptr );
    }
    

// -----------------------------------------------------------------------------
// Signal destruction of interface implementation to ECOM.
// -----------------------------------------------------------------------------
//
inline void CUSBClassChangeUIPluginInterface::SignalDestruction()
    {
    REComSession::DestroyedImplementation( iDtor_ID_Key );    
    }
