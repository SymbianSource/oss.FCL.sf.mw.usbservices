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
* Description:  Header file for RNDIS uplink authorization notifier
*
*/

/**
@file
@internalComponent
*/

#ifndef RNDISAUTHORIZATIONNOTIFIER_H
#define RNDISAUTHORIZATIONNOTIFIER_H

#include <e32base.h>	// CBase
#include <e32std.h>		// TBuf
#include <eiknotapi.h>	// For MEikSrvNotifierBase2

NONSHARABLE_CLASS(CRndisAuthorizationNotifier)
	: public CBase
	, public MEikSrvNotifierBase2
	{
public:
	// Create CRndisAuthorizationNotifier.
	static CRndisAuthorizationNotifier* NewLC();
	// Destructor
	virtual ~CRndisAuthorizationNotifier();

public:
	// virtual functions from MEikSrvNotifierBase2
	virtual void Release();
	virtual TNotifierInfo RegisterL();
	virtual TNotifierInfo Info() const;
	virtual void StartL(const TDesC8& aBuffer, TInt aReplySlot, const RMessagePtr2& aMessage);
	virtual TPtrC8 StartL(const TDesC8& aBuffer);
	virtual void Cancel();
	virtual TPtrC8 UpdateL(const TDesC8& aBuffer);	

private:
	CRndisAuthorizationNotifier();
	void ConstructL();

private:	
	TNotifierInfo iInfo;
	};

#endif //RNDISAUTHORIZATIONNOTIFIER_H
