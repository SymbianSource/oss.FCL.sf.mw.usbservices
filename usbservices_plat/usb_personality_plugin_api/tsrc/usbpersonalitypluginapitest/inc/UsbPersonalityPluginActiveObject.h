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

#ifndef USBPERSONALITYPLUGINACTIVEOBJECT_H
#define USBPERSONALITYPLUGINACTIVEOBJECT_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib

#include <cusbpersonalityplugin.h>
#include <tusbpersonalityparams.h>

#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>


class CUsbPersonalityPluginActiveObject : public CActive
    {
public:
    // Cancel and destroy
    ~CUsbPersonalityPluginActiveObject();

    // Two-phased constructor.
    static CUsbPersonalityPluginActiveObject* NewL( CUsbPersonalityPlugin*, CStifLogger*, TUsbPersonalityParams* );

    // Two-phased constructor.
    static CUsbPersonalityPluginActiveObject* NewLC( CUsbPersonalityPlugin*, CStifLogger*, TUsbPersonalityParams* );

public:
    // New functions
    // Function for making the initial request
    void PreparePersonalityStart( TRequestStatus& aStatus);

    void FinishPersonalityStart( TRequestStatus& aStatus);

    void PreparePersonalityStop( TRequestStatus& aStatus);

    void FinishPersonalityStop( TRequestStatus& aStatus);

    void ConfirmPersonalityUnload( TRequestStatus& aStatus);
private:
    // C++ constructor
    CUsbPersonalityPluginActiveObject( CUsbPersonalityPlugin*, CStifLogger*, TUsbPersonalityParams* );

    // Second-phase constructor
    void ConstructL();

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

private:
    CUsbPersonalityPlugin* iPlugin;
    CStifLogger* iLog;
    TUsbPersonalityParams* iParams;
    };

#endif // USBPERSONALITYPLUGINACTIVEOBJECT_H
