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
* Description:  source file for RNDIS uplink authorization notifier
*
*/

/**
@file
@internalComponent
*/

#include <ecom/implementationproxy.h>
#include <rndis/rndis.h>
#include <rndis/rndisuiinterfaces.h>
#include "rndisauthorizationnotifier.hrh"
#include "rndisauthorizationnotifier.h"

using namespace UsbRndis;

CRndisAuthorizationNotifier* CRndisAuthorizationNotifier::NewLC()
	{
	CRndisAuthorizationNotifier* self = new (ELeave)CRndisAuthorizationNotifier();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CRndisAuthorizationNotifier::CRndisAuthorizationNotifier()
	{
	}

void CRndisAuthorizationNotifier::ConstructL()
	{
	}

CRndisAuthorizationNotifier::~CRndisAuthorizationNotifier()
	{
	}

void CRndisAuthorizationNotifier::Release()
 	{
 	delete this;
 	}

CRndisAuthorizationNotifier::TNotifierInfo CRndisAuthorizationNotifier::RegisterL()
	{
	//Return TNotifierInfo, describes the parameters of the notifier
	iInfo.iUid = TUid::Uid(KUidUplinkAuthNotifierImp);
	iInfo.iChannel = TUid::Uid(KUidUplinkAuthNotifierImp);
	iInfo.iPriority = ENotifierPriorityHigh;
	return iInfo;
	}

CRndisAuthorizationNotifier::TNotifierInfo CRndisAuthorizationNotifier::Info() const
	{
	//Return TNotifierInfo, describes the parameters of the notifier
	return iInfo;
	}

TPtrC8 CRndisAuthorizationNotifier::StartL(const TDesC8& /*aBuffer*/)
	{
	// Do not support synchronous start, just return.
	return KNullDesC8();
	}

void CRndisAuthorizationNotifier::StartL(const TDesC8& /*aBuffer*/, TInt aReplySlot, const RMessagePtr2& aMessage)
	{
	// Just to set as DisAllow defaultly ..
	TRndisUplinkAuthorizationResultPckg selection(ERndisDisallowUplinkAccess);
	aMessage.Write(aReplySlot, selection);
	aMessage.Complete(KErrNone);
	}

void CRndisAuthorizationNotifier::Cancel()
	{
	}
	
TPtrC8 CRndisAuthorizationNotifier::UpdateL(const TDesC8&)
	{
	// Do not support update, just return.
	return KNullDesC8();
	}

CArrayPtr<MEikSrvNotifierBase2>* NotifierArrayL()
	{
	CArrayPtrFlat<MEikSrvNotifierBase2>* notifiers = new(ELeave) CArrayPtrFlat<MEikSrvNotifierBase2>(1);
	CleanupStack::PushL(notifiers);
	CRndisAuthorizationNotifier* notifier = CRndisAuthorizationNotifier::NewLC();
	notifiers->AppendL(notifier);
	CleanupStack::Pop(2, notifiers);
	return notifiers;
	}

// ECom plugin entry point
const TImplementationProxy ImplementationTable[] =
	{
    IMPLEMENTATION_PROXY_ENTRY(KUidUplinkAuthNotifierImp, NotifierArrayL)
	};

// ECom plugin entry point
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
