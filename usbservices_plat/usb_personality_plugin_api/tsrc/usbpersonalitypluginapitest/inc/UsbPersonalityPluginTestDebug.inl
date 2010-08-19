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

#include <e32debug.h>

NONSHARABLE_CLASS( TOverflowTruncate8 ) : public TDes8Overflow
    {
public:
    void Overflow( TDes8& /*aDes*/ ) {}
    }; 

NONSHARABLE_CLASS( TOverflowTruncate16 ) : public TDes16Overflow
    {
public:
    void Overflow( TDes16& /*aDes*/ ) {}
    };

_LIT8(KStifTestPrefix8, "[STIF_LOG] ");
_LIT( KStifTestPrefix, "[STIF_LOG] ");
_LIT8( KStifEntryPrefix8, ">>> %S" );
_LIT8( KStifExitPrefix8, "<<< %S" );

const TInt KMaxLogLineLength = 512;

#define TRACE_INFO( p ) { Trace p; }
#define TRACE_FUNC_ENTRY { TPtrC8 ptr8( ( TUint8* ) __FUNCTION__ ); Trace( KStifEntryPrefix8, &ptr8 ); }
#define TRACE_FUNC_EXIT { TPtrC8 ptr8( ( TUint8* ) __FUNCTION__ ); Trace( KStifExitPrefix8, &ptr8 ); }

void CUsbPersonalityPluginTest::Trace(TRefByValue<const TDesC8> aFmt, ...)
	{
	VA_LIST list;
	VA_START( list, aFmt );
	TOverflowTruncate8 overflow;   
	
	RBuf8 buf8;
	RBuf8 bufStifLog8;
	
	buf8.Create( KMaxLogLineLength );
	buf8.Append( KStifTestPrefix8 );
	buf8.AppendFormatList( aFmt, list, &overflow );	
	bufStifLog8.Create( KMaxLogLineLength );
	bufStifLog8.AppendFormatList( aFmt, list, &overflow );
			
	RBuf16 buf16;
	RBuf16 bufStifLog16;
	
	buf16.Create( KMaxLogLineLength );
	buf16.Copy( buf8 );
	bufStifLog16.Create( KMaxLogLineLength );
	bufStifLog16.Copy( bufStifLog8 );
	
	RDebug::Print( buf16 );
	iLog->Log( bufStifLog16 );
	buf8.Close();
	bufStifLog8.Close();
	buf16.Close();
	bufStifLog16.Close();
	};

void CUsbPersonalityPluginTest::Trace(TRefByValue<const TDesC16> aFmt, ...)
	{
	VA_LIST list;
	VA_START(list,aFmt);
	TOverflowTruncate16 overflow;
	
	RBuf16 buf;
	RBuf16 bufStifLog;
	
	buf.Create( KMaxLogLineLength );
	buf.Append( KStifTestPrefix );
	buf.AppendFormatList( aFmt, list, &overflow );
	bufStifLog.Create( KMaxLogLineLength );
	bufStifLog.AppendFormatList( aFmt, list, &overflow );
	
	RDebug::Print( buf );
	iLog->Log( bufStifLog );
	buf.Close();
	bufStifLog.Close();
	};
