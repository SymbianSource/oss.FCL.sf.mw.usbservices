/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Used by CUsbWatcher class to notify about completed request.
*
*/


#ifndef MUSBWATCHERNOTIFY_H
#define MUSBWATCHERNOTIFY_H

class MUsbWatcherNotify
    {
public:
    virtual void Notify( TInt aPersonalityId, TInt aStatus ) = 0;  

    /**
     * Check if ask on connection mode query is suppressed in this session.
     */
    virtual TBool IsAskOnConnectionSuppressed() = 0;
    };

#endif

// End of file
