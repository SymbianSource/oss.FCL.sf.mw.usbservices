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
* Description:  Source file for RNDIS refusal notifier
*
*/

/**
@file
@internalComponent
*/

#include <ecom/implementationproxy.h>
#include <rndis/rndisuiinterfaces.h>
#include "rndisrefusalnotifier.hrh"
#include "rndisrefusalnotifier.h"

using namespace UsbRndis;

//Number of notifier needed.
const TInt KNotfierCount = 1;

CRndisNetworkConnectionRefusalNotifier* CRndisNetworkConnectionRefusalNotifier::NewLC()
	{
	CRndisNetworkConnectionRefusalNotifier* self = new (ELeave) CRndisNetworkConnectionRefusalNotifier;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CRndisNetworkConnectionRefusalNotifier::CRndisNetworkConnectionRefusalNotifier()
	{
	}

CRndisNetworkConnectionRefusalNotifier::~CRndisNetworkConnectionRefusalNotifier()
	{
	}

void CRndisNetworkConnectionRefusalNotifier::ConstructL()
	{
	}

void CRndisNetworkConnectionRefusalNotifier::Release()
	{
	delete this;
	}

MEikSrvNotifierBase2::TNotifierInfo CRndisNetworkConnectionRefusalNotifier::RegisterL()
	{
	iInfo.iUid=TUid::Uid(KUidNetworkConnectionRefusalNotifierImp);
	iInfo.iChannel = TUid::Uid(KUidNetworkConnectionRefusalNotifierImp);
	iInfo.iPriority=ENotifierPriorityHigh;
	return iInfo;
	}

MEikSrvNotifierBase2::TNotifierInfo CRndisNetworkConnectionRefusalNotifier::Info() const
	{
	return iInfo;
	}

TPtrC8 CRndisNetworkConnectionRefusalNotifier::StartL(const TDesC8& aBuffer)
	{
	return aBuffer;
	}
 
void CRndisNetworkConnectionRefusalNotifier::StartL(const TDesC8& /*aBuffer*/, TInt /*aReplySlot*/, const RMessagePtr2& aMessage)
	{
	aMessage.Complete(KErrNone);
	}

void CRndisNetworkConnectionRefusalNotifier::Cancel()
	{
	}

TPtrC8 CRndisNetworkConnectionRefusalNotifier::UpdateL(const TDesC8& aBuffer)
	{
	return aBuffer;
	}

CArrayPtr<MEikSrvNotifierBase2>* NotifierArrayL()
	{
	CArrayPtr<MEikSrvNotifierBase2>* array = new(ELeave) CArrayPtrFlat<MEikSrvNotifierBase2>(KNotfierCount);
	CleanupStack::PushL(array);
	array->AppendL(CRndisNetworkConnectionRefusalNotifier::NewLC()); 
	CleanupStack::Pop(2, array);
	return array;
	}

// ECom plugin entry point
const TImplementationProxy ImplementationTable[] =
	{ 		
	IMPLEMENTATION_PROXY_ENTRY(KUidNetworkConnectionRefusalNotifierImp, NotifierArrayL) 
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable)/sizeof(TImplementationProxy);
	return ImplementationTable;
	}
