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
// class definition for phone as modem at extension plugin
// 
//

#ifndef PAMPLUGINDEBUGCONFIG_H
#define PAMPLUGINDEBUGCONFIG_H

#include "pampluginprjconfig.h"

/**
 * Custom logging variations.
 */
#ifdef PRJ_FILE_TRACE
_LIT(KLogFile,"pamplugin.txt");
_LIT(KLogDir,"USB");
#endif

#ifdef PRJ_ENABLE_TRACE
_LIT(KTracePrefix16, "[ref_ATEXT] ");
_LIT8(KTracePrefix8, "[ref_ATEXT] ");
_LIT8(KFuncFormat8, "><%S");
_LIT8(KFuncThisFormat8, "><%S, [0x%08X]");
_LIT8(KFuncEntryFormat8, ">%S");
_LIT8(KFuncEntryThisFormat8, ">%S, [0x%08X]");
_LIT8(KFuncExitFormat8, "<%S");

_LIT(KPanicCategory, "Ref_ATExt");    
_LIT8(KPanicPrefix8, "PANIC code ");
_LIT8(KLeavePrefix8, "LEAVE code ");
#endif

const TInt KMaxLogLineLength = 508;  // 512-4, 4 is taken by TBuf structure, 
// otherwise exceeds maximum single use of 1024 bytes for stack in TBuf16 config

#define KPRINTERROR        0x00000001 // Tracing level: error
#define KPRINTINFO        0x00000002 // Tracing level: function trace
#define KPRINTSTATE        0x00000004 // Tracing level: state machine info
#define KPRINTWARNING   0x00000008 // Tracing level: warning

const TInt KTraceMask = KPRINTERROR | KPRINTINFO | KPRINTSTATE | KPRINTWARNING;

#endif // PAMPLUGINDEBUGCONFIG_H
