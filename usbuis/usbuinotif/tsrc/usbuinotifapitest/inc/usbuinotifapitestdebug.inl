/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32debug.h>

NONSHARABLE_CLASS(TOverflowTruncate8) : public TDes8Overflow
    {
public:
    void Overflow(TDes8& /*aDes*/) {}
    }; 

NONSHARABLE_CLASS(TOverflowTruncate16) : public TDes16Overflow
    {
public:
    void Overflow(TDes16& /*aDes*/) {}
    };


_LIT8(KStifTestPrefix8, "[STIF_LOG] ");
_LIT(KStifTestPrefix, "[STIF_LOG] ");

const TInt KMaxLogLineLength = 512;

#define TRACE_INFO(p) { Trace p; }

void CUsbUiNotifApiTest::Trace(TRefByValue<const TDesC8> aFmt, ...)
	{
	VA_LIST list;
	VA_START(list, aFmt);
	TOverflowTruncate8 overflow;        
	RBuf8 buf8;
	buf8.Create( KMaxLogLineLength );
	buf8.Append( KStifTestPrefix8 );
	buf8.AppendFormatList(aFmt, list, &overflow);
			
	RBuf16 buf16;
	buf16.Create( KMaxLogLineLength );
	buf16.Copy(buf8);    
	TRefByValue<const TDesC> tmpFmt(_L("%S"));
	RDebug::Print(tmpFmt, &buf16);
	iLog->Log(tmpFmt, &buf16);
	buf8.Close();         
	buf16.Close();
	};

void CUsbUiNotifApiTest::Trace(TRefByValue<const TDesC16> aFmt, ...)
	{
	VA_LIST list;
	VA_START(list,aFmt);
	RBuf16 theFinalString;
	theFinalString.Create( KMaxLogLineLength );
	theFinalString.Append( KStifTestPrefix );
	TOverflowTruncate16 overflow;
	theFinalString.AppendFormatList(aFmt,list,&overflow);
	RDebug::Print(theFinalString);
	iLog->Log(theFinalString);
	theFinalString.Close(); 
	};
