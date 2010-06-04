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
// Implements a AT extension plugin
// 
//

#include "pamplugin.h"
#include "pamplugindebug.h"
#include "pamengine.h"

#include <badesca.h>
#include <etel3rdparty.h>


_LIT8(KNqapString, "*NQAP: ");
_LIT8(KNqapTestString, "AT*NQAP=?");
_LIT8(KNqapInternetString, "AT*NQAP=0");
_LIT8(KNqapWapString, "AT*NQAP=1");
_LIT8(KNqapInternetAndWapString, "AT*NQAP=2");
_LIT8(KErrorString, "\r\nERROR\r\n");
_LIT8(KOkString, "\r\nOK\r\n");
_LIT8(KCrlfString, "\r\n");
_LIT8(KNcnnString, "AT*NCNN");

/**
 Create a new instance
 */
CPamPlugin* CPamPlugin::NewL()
    {
    CPamPlugin* self = new (ELeave) CPamPlugin();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


/**
 Destructor
 */
CPamPlugin::~CPamPlugin()
    {
    TRACE_FUNC_ENTRY
    delete iPAMEngine;
    TRACE_FUNC_EXIT
    }

TBool CPamPlugin::IsCommandSupported( const TDesC8& aCmd )
    {
    TRACE_FUNC_ENTRY
    TBool ret = EFalse;
    if (aCmd.FindF(KNqapTestString) == 0)
        {
        ret = ETrue;
        }
    else if (aCmd.FindF(KNqapInternetString) == 0)
        {
        ret = ETrue;
        }
    else if (aCmd.FindF(KNqapWapString) == 0)
        {
        ret = ETrue;
        }
    else if (aCmd.FindF(KNqapInternetAndWapString) == 0)
        {
        ret = ETrue;
        }
    else if (aCmd.FindF(KNcnnString) == 0)
        {
        ret = ETrue;
        }
    return ret;
    }


void CPamPlugin::HandleCommand( const TDesC8& aCmd, RBuf8& aReply, TBool aReplyNeeded )
	{
	TRACE_FUNC_ENTRY
	TRACE_INFO((_L8("CPamPlugin::HandleCommand cmd '%S' reply '%S'"), &aCmd, &aReply))

	if ( !aReplyNeeded )
    {
    return;
    }

    const TInt KInternet = 0;  
    const TInt KWap = 1;  
    const TInt KWapInternet = 2;  

	TInt err(KErrNotSupported);

    if (aCmd.FindF(KNqapTestString) >= 0)
        {
        SupportedAccessPointL( aReply );
        err = KErrNone;
        }
        
    else if (aCmd.FindF(KNqapInternetString) >= 0)
        {
        ReadAccessPointL( KInternet, aReply );
        err = KErrNone;
        }
    else if (aCmd.FindF(KNqapWapString) >= 0)
        {
        ReadAccessPointL( KWap, aReply );
        err = KErrNone;
        }
    else if (aCmd.FindF(KNqapInternetAndWapString) >= 0)
        {
        ReadAccessPointL( KWapInternet, aReply );
        err = KErrNone;
        }
    else if (aCmd.FindF(KNcnnString) >= 0)
        {
        iReply = &aReply;
        iPAMEngine->GetServiceProviderName();
        return;
        }
    else
        {
        err = KErrNotSupported;
        }

    if(err != KErrNone)
        HandleCommandCompleted(err, EReplyTypeUndefined);
    else
        HandleCommandCompleted(KErrNone, EReplyTypeOk);
        
    TRACE_FUNC_EXIT
    }


void CPamPlugin::NameReqCallback( const TDesC8& aName )
    {
    TRACE_FUNC_ENTRY
    if ( aName == KNullDesC8() )
        {
        iReply->Zero();
        iReply->Create( KErrorString );
        }
    else
        {
        TBuf8<32> buf;
        buf.Zero();
        buf.Append( KCrlfString );
        buf.Append( aName );
        buf.Append( KCrlfString );
        buf.Append( KOkString );
        iReply->Zero();
        iReply->Create( buf );
        }
    HandleCommandCompleted(KErrNone, EReplyTypeOk);
	TRACE_FUNC_EXIT
    }

/**
 Used by AT*NQAP=?
 */
void CPamPlugin::SupportedAccessPointL( RBuf8& aReply )
	{
	TRACE_FUNC_ENTRY

    const TInt Kbufsize = 16;
    const TInt Kresponcebufsize = 64;
	
    HBufC16* supportedAp = HBufC16::NewL( Kbufsize );
    iPAMEngine->SupportedAccessPoints( supportedAp );
    TBuf8<Kresponcebufsize> buf;
    
	buf.Zero();
    buf.Append( KCrlfString );
    buf.Append( KNqapString );
    buf.Append( *supportedAp );
    buf.Append( KCrlfString );
    buf.Append( KOkString );

    aReply.Zero();
    aReply.Create( buf ); 

	TRACE_FUNC_EXIT
 	return;
    }           

/**
 Used by AT*NQAP=<>
 */
void CPamPlugin::ReadAccessPointL(TInt aAccessPoint, RBuf8& aReply )
	{
	TRACE_FUNC_ENTRY

    const TInt KreplyBufferLength = 1024;
	
   	CDesC8ArrayFlat* apArray = new(ELeave) CDesC8ArrayFlat(4);
    CleanupStack::PushL( apArray );
   	
    iPAMEngine->ReadAccessPointTableL( aAccessPoint, apArray );

    TInt apArrayLength = apArray->Count();
    if ( apArrayLength > 0)
        {
        HBufC8* buf = HBufC8::NewL( KreplyBufferLength );
        buf->Des().Zero();
        buf->Des().Append( KCrlfString );
        for (TInt i=0; i < apArrayLength; i++ )
            {
            // AT*NQAP=<n> responce
            buf->Des().Append( KNqapString );
            buf->Des().Append( (*apArray)[i] );
            buf->Des().Append( KCrlfString );
            }
        buf->Des().Append( KOkString );
        aReply.Zero();
        aReply.Create( *buf );
        }
    else
        {
        aReply.Zero();
        aReply.Create( KOkString );
        }
    CleanupStack::PopAndDestroy( apArray );
    TRACE_FUNC_EXIT
	return;
    }           
        

TInt CPamPlugin::HandleRunError(TInt /*aErr*/)
    {
     return KErrNone;
    }


CPamPlugin::CPamPlugin() : CATExtPluginBase()
    {
    TRACE_FUNC
    }
     

void CPamPlugin::ConstructL()
    {
    iPAMEngine = CPamEngine::NewL( this );
    TRACE_FUNC
   	}

void CPamPlugin::ReportConnectionName( const TDesC8& /*aName*/ )
    {
    }

void CPamPlugin::HandleCommandCancel()
    {
    }

TInt CPamPlugin::NextReplyPartLength()
    {
    return 0;
    }
TInt CPamPlugin::GetNextPartOfReply(RBuf8& /*aNextReply*/)
    {
    return 0;
    }

void CPamPlugin::ReceiveUnsolicitedResult()
    {
    }

void CPamPlugin::ReceiveUnsolicitedResultCancel()
    {
    }

void CPamPlugin::ReportNvramStatusChange( const TDesC8& /*aNvram*/ )
    {
    }

void CPamPlugin::ReportExternalHandleCommandError()
    {
    }

//  End of File  
