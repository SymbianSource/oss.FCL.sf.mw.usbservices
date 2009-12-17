// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// 
// 

/**
 * @file 
 * @internalComponent
 *
 * USB mass storage Server Security Policy definitions for Platform security.
 */
 
#ifndef MSCFILESERVERSECURITYPOLICY_H
#define MSCFILESERVERSECURITYPOLICY_H

#include "usbmscfileshared.h"

// USB masss storage Server Security Policy Definition

const TUint KUsbMsServerRangeCount = 3;

const TInt KUsbMsServerRanges[KUsbMsServerRangeCount] = 
	{
	EMscFileStart,                // NetworkControl	[StartL/Stop]
	EMscFileShutdown,				// DiskAdmin		[Shutdown]
    EMscFileShutdown + 1,         // fail (to KMaxInt)
	};

// Index numbers into KUsbMsServerElements[]
const TInt KPolicyNetworkControl = 0;
const TInt KPolicyDiskAdmin = 1;

// Mapping IPCs to poicy element
const TUint8 KUsbMsServerElementsIndex[KUsbMsServerRangeCount] = 
	{
	KPolicyNetworkControl,          // EMscFileStart & EMscFileStop 
	KPolicyDiskAdmin,			 	// EMscFileShutdown 
	CPolicyServer::ENotSupported,   // EMscFileShutdown + 1 to KMaxTInt    
	};

// Individual policy elements 
const CPolicyServer::TPolicyElement KUsbMsServerElements[] = 
	{
  		{ _INIT_SECURITY_POLICY_C1(ECapabilityNetworkControl), CPolicyServer::EFailClient },
		{ _INIT_SECURITY_POLICY_C1(ECapabilityDiskAdmin), CPolicyServer::EFailClient},
		// the EFailClient means that if the check fails 
        // the CheckFailed method with return KErrPermissionDenied
	};

// Main policy
const CPolicyServer::TPolicy KMscFileServerPolicy = 
	{
	CPolicyServer::EAlwaysPass, // all connect attempts should pass
	KUsbMsServerRangeCount,
	KUsbMsServerRanges,
	KUsbMsServerElementsIndex,
	KUsbMsServerElements,
	};
#endif //MSCFILESERVERSECURITYPOLICY_H
