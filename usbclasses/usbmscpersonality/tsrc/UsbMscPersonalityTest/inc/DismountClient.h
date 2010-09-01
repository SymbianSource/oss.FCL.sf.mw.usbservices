/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

#ifndef DISMOUNTCLIENT_H
#define DISMOUNTCLIENT_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <StifLogger.h>
#include <f32file.h>

enum TAllowDismountOption
{
    EAllowDismount,
    EAllowDismountAfter2Sec,
    EAllowDismountTooLate,
    ENotAllowDismount
};

class CDismountClient : public CActive
    {
public:
    // Cancel and destroy
    ~CDismountClient();

    // Two-phased constructor.
    static CDismountClient* NewL(CStifLogger *aLog);

    // Two-phased constructor.
    static CDismountClient* NewLC(CStifLogger *aLog);

public:
    // New functions
    // Function for making the initial request
    void StartL(TAllowDismountOption aAllowDismountOption);

private:
    // C++ constructor
    CDismountClient();

    // Second-phase constructor
    void ConstructL(CStifLogger *aLog);

private:
    // From CActive
    // Handle completion
    void RunL();

    // How to cancel me
    void DoCancel();

    // Override to handle leaves from RunL(). Default implementation causes
    // the active scheduler to panic.
    TInt RunError(TInt aError);

private:
    enum TDismountClientState
        {
        EWaitForDelay,
        EDelay,
        EUninitialized, // Uninitialized
        EInitialized, // Initalized
        EError // Error condition
        };
  
private:
    TInt iState; // State of the active object
    RFs iFs;
    CStifLogger *iLog;
    RTimer iTimer;
    TAllowDismountOption iAllowDismountOption;
    
    };

#endif // DISMOUNTCLIENT_H
