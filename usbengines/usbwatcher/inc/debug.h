/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Debug macros
*
*/


#ifndef DEBUG_H
#define DEBUG_H

//////////////////////////////////////////////////////////////////////////////
// Here are parameters need to be modified. 
// Component name 
#define MODULE_NAME "USBWATCHER"
//////////////////////////////////////////////////////////////////////////////


// For handling unicode strings as L##MACRO is interpreted as LMACRO
#define WIDEN2( x ) L##x
#define WIDEN( x ) WIDEN2( x )

_LIT( KUsbPanicModule, ""WIDEN( MODULE_NAME ) );

#ifdef _DEBUG

// Comment this line out if using real-time debugging
//#define LOG_TO_FILE 
// ===========================================================================
#if defined ( LOG_TO_FILE )
// ===========================================================================
// Add corresponding libabry in mmp file
// LIBRARY    flogger.lib    
// The log file is located at  
// c:\logs\usb\MODULE_NAME.txt

#include <flogger.h>

#define DESC( x ) TPtrC( ( const TText* ) ( x ) )
#define DESC8( x ) TPtrC8( ( const TUint8* ) ( x ) )
_LIT( KUsbLogDir, "USB" );
_LIT( KUsbLogFile, ""WIDEN(MODULE_NAME)L".txt" );

// s: c string. 
#define LOG(s) RFileLogger::WriteFormat( KUsbLogDir, \
                KUsbLogFile, EFileLoggingModeAppend, \
                DESC8( "[%08x] ["MODULE_NAME"] "##s ), TUint(RThread().Id()) )

// s: c string contains "%x"
// v: varaible corresponding to %x
#define LOG1(s, v) RFileLogger::WriteFormat( KUsbLogDir, \
                KUsbLogFile, EFileLoggingModeAppend, \
                DESC8( "[%08x] ["MODULE_NAME"] "##s ), v, \
                TUint(RThread().Id()) )
// s: c string contains "%x%y"
// v1: varaible corresponding to %x
// v2: varaible corresponding to %y
#define LOG2(s, v1, v2) RFileLogger::WriteFormat( KUsbLogDir, \
                KUsbLogFile, EFileLoggingModeAppend, \
                DESC8( "[%08x] ["MODULE_NAME"] "##s ), TUint(RThread().Id()), \
                v1, v2 )

// s: c string contains "%x%y%z"
// v1: varaible corresponding to %x
// v2: varaible corresponding to %y
// v3: varaible corresponding to %z
#define LOG3(s, v1, v2, v3) RFileLogger::WriteFormat( KUsbLogDir, \
            KUsbLogFile, EFileLoggingModeAppend, \
            DESC8( "[%08x] ["MODULE_NAME"] "##s ), \
            TUint(RThread().Id()), v1, v2, v3 )

class TFuncLogger
    {
public:
    TFuncLogger(const TDesC8& aFuncName)
        {
        iFuncName.Set( aFuncName );
        RFileLogger::WriteFormat( KUsbLogDir, KUsbLogFile, 
            EFileLoggingModeAppend, 
            DESC8( "[%08x] ["MODULE_NAME"] >>%s" ), TUint(RThread().Id()),
            iFuncName.Ptr() );
        }
    ~TFuncLogger()
        {
        RFileLogger::WriteFormat( KUsbLogDir, KUsbLogFile,
             EFileLoggingModeAppend, 
             DESC8( "[%08x] ["MODULE_NAME"] <<%s" ), TUint(RThread().Id()),
             iFuncName.Ptr() );
        }
private:
    TPtrC8 iFuncName;
    };
// ===========================================================================
#else //Real-time logging
// ===========================================================================
#include <e32debug.h>

// Paramters same as above.
#define LOG( str ) { RDebug::Printf( "[%08x] ["MODULE_NAME"] %s",\
        TUint(RThread().Id()), str  ); }

#define LOG1( s, v ) { RDebug::Printf( "[%08x] ["MODULE_NAME"] "##s ,\
        TUint(RThread().Id()), v ); }

#define LOG2( s, v1,v2 ) { RDebug::Printf( "[%08x] ["MODULE_NAME"] "##s , \
        TUint(RThread().Id()), v1, v2 ); }

#define LOG3( s, v1, v2, v3 ) { RDebug::Printf( "[%08x] ["MODULE_NAME"] "##s, \
        TUint(RThread().Id()), v1, v2, v3); }

class TFuncLogger
    {
public:
    TFuncLogger(const TDesC8& aFuncName)
        {
        iFuncName.Set( aFuncName );
        RDebug::Printf( "[%08x] ["MODULE_NAME"] >>%s", TUint(RThread().Id()), 
             iFuncName.Ptr());
        }
    ~TFuncLogger()
        {
        RDebug::Printf( "[%08x] ["MODULE_NAME"] <<%s", TUint(RThread().Id()), 
             iFuncName.Ptr());
        }
private:
    TPtrC8 iFuncName;
    };
#endif // LOG_TO_FILE

#define LEAVE( exp )  {volatile TInt err_ = exp; \
        LOG3( "LEAVE(%d) @file: %s, line: %d", err_, __FILE__, __LINE__ );\
        User::Leave( err_ );}

#define LEAVEIFERROR( exp ) {volatile TInt err__ = exp; \
		if(err__ < 0) LEAVE(err__);}

#define PANIC( exp ) {volatile TInt err_ = exp; \
        LOG3( "PANIC(%d) @file: %s, line: %d", err_, __FILE__, __LINE__ );\
        User::Panic( KUsbPanicModule, err_ );}


#define LOG_FUNC TFuncLogger __instrument(TPtrC8((TUint8*)__PRETTY_FUNCTION__));

#else // _DEBUG   
// ===========================================================================

#define LOG( s )
#define LOG1( s, v ) 
#define LOG2( s, v1, v2 )
#define LOG3( s, v1, v2, v3 )
#define LOG_FUNC_ENTRY 
#define LOG_FUNC_EXIT 
#define LEAVE( exp ) User::Leave( exp );
#define LEAVEIFERROR( exp ) User::LeaveIfError( exp );
#define PANIC( err ) User::Panic( KUsbPanicModule, err );
#define LOG_FUNC
// ===========================================================================
#endif // _DEBUG
// ===========================================================================
#endif // DEBUG_H

// End of File
