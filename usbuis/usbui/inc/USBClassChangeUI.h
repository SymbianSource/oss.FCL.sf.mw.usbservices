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
* Description:  Common declarations for USBClassChange classes
*
*/


#ifndef USBCLASSCHANGEUI_H
#define USBCLASSCHANGEUI_H

#include <e32svr.h> 	// For RDebug and Panic

// Application name
//
_LIT( KUSBUIAppName, "USBUI" );

// UID of the application
//
const TUid KUidUSBClassChangeUI = { 0x102068E2 };

// MACROS
//
#define PANIC( aPanic ) User::Panic( KUSBUIAppName, aPanic )

// UID of the General Settings Plugin
const TUid KGsUSBUiPluginUid = { 0x1020E473 };

#endif //USBCLASSCHANGEUI_H

// End of file
