/*
* Copyright (c) 2002 - 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implements OBEX class controller
*
*/


#include "CUsbObexClassController.h"
#include <usb_std.h>
#include <obex.h>
#include <SrcsInterface.h>
#include <mmf/common/mmfcontrollerpluginresolver.h> //for CleanupResetAndDestroyPushL
#include <musbclasscontrollernotify.h>

// Panic category only used in debug builds
#ifdef _DEBUG
_LIT( KObexCcPanicCategory, "OBEXCC" );
#endif

#ifdef __FLOG_ACTIVE
_LIT8(KLogComponent, "UsbObexCc");
#endif

/**
 * Panic codes for the USB OBEX Class Controller.
 */
enum TObexCCPanic
  {
  /** Illigal calling of asynchronous function */
  EBadAsynchronousCall = 0,
  /** Start() called while in an illegal state */
  EBadApiCallStart = 1,
  /** Stop() called while in an illegal state */
  EBadApiCallStop = 2,
  };

// ---------------------------------------------------------------------------
// Constructs a CUsbObexClassController object.
// ---------------------------------------------------------------------------
//
CUsbObexClassController* CUsbObexClassController::NewL(
  MUsbClassControllerNotify& aOwner)
  {
  LOG_STATIC_FUNC_ENTRY

  CUsbObexClassController* self = new (ELeave) CUsbObexClassController(aOwner);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(self);
  return self;
  }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CUsbObexClassController::CUsbObexClassController(
    MUsbClassControllerNotify& aOwner)
  : CUsbClassControllerPlugIn(aOwner, KObexClassPriority)
  {
  LOG_FUNC
  iState = EUsbServiceIdle;
  }

// ---------------------------------------------------------------------------
// Method to perform second phase construction.
// ---------------------------------------------------------------------------
//
void CUsbObexClassController::ConstructL()
  {
  LOG_FUNC
  iObexSM = CObexUSB::NewL();
  iObexSMWatcher = CObexSMWatcher::NewL(*this);
  }

// ---------------------------------------------------------------------------
// From class CUsbClassControllerPlugIn.
// Destructor
// ---------------------------------------------------------------------------
//
CUsbObexClassController::~CUsbObexClassController()
  {
  LOG_FUNC
  Cancel();
  delete iObexSM;
  delete iObexSMWatcher;
  }

// ---------------------------------------------------------------------------
// From class CUsbClassControllerPlugIn.
// Called by UsbMan to start this class.
// ---------------------------------------------------------------------------
//
void CUsbObexClassController::Start(TRequestStatus& aStatus)
  {

  LOG_FUNC
  //Start() should never be called if started, starting or stopping (or in state EUsbServiceFatalError)
  __ASSERT_DEBUG(iState == EUsbServiceIdle, _USB_PANIC(KObexCcPanicCategory, EBadApiCallStart));

  // Start OBEX SM
  iRequestStatus = &aStatus;
  iState = EUsbServiceStarting;
  aStatus = KRequestPending;
  LOGTEXT(_L8("CUsbObexClassController::Start() calling ManageUSBService(ETrue)")); 
  iObexSM->ManageUSBServices(ETrue, iStatus);
  SetActive();
  }

// ---------------------------------------------------------------------------
// From class CUsbClassControllerPlugIn.
// Called by UsbMan to stop this class.
// ---------------------------------------------------------------------------
//
void CUsbObexClassController::Stop(TRequestStatus& aStatus)
  {

  LOG_FUNC
  LOGTEXT2(_L8("CUsbObexClassController::Stop iState = %d"), iState);
  
  //Stop() should never be called if stopping or starting (or in state EUsbServiceFatalError)
  __ASSERT_DEBUG(iState == EUsbServiceStarted || iState == EUsbServiceIdle, _USB_PANIC(KObexCcPanicCategory, EBadApiCallStop));

  //state may be idle after Cancel
  if ( iState == EUsbServiceIdle )
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete(status, KErrNone);
    }
  else
    {
    // Stop OBEX SM
    iRequestStatus = &aStatus;
    iState = EUsbServiceStopping;   
    aStatus = KRequestPending;
    LOGTEXT(_L8("CUsbObexClassController::Stop() calling ManageUSBService(EFalse)")); 
    iObexSM->ManageUSBServices(EFalse, iStatus);
    SetActive();
    }
  }
  
// ---------------------------------------------------------------------------
// From class CActive.
// ---------------------------------------------------------------------------
//
void CUsbObexClassController::RunL()
  {

  LOG_FUNC
  if (iStatus != KErrNone)
    {
    LOGTEXT2(_L8("CUsbObexClassController::RunL() Error = %d"), iStatus.Int());
    User::RequestComplete(iRequestStatus, iStatus.Int());
    return;
    }
  LOGTEXT2(_L8("CUsbObexClassController::RunL() State is %d"), iState);

            switch (iState)
                {
                case EUsbServiceStarting:
                    iState = EUsbServiceStarted;
                    User::RequestComplete(iRequestStatus, KErrNone);
                    break;
                    
                case EUsbServiceStopping:
                    iState = EUsbServiceIdle;
                    User::RequestComplete(iRequestStatus, KErrNone);
                    break;

                case EUsbServiceStarted:
                case EUsbServiceIdle:

                default:
            LOGTEXT(_L8("CUsbObexClassController::RunL() Error or Unknown State"));
                    break;
                }
  }

// ---------------------------------------------------------------------------
// From class CUsbClassControllerPlugIn.
// Returns information about the interfaces supported by this class.
// ---------------------------------------------------------------------------
//
void CUsbObexClassController::GetDescriptorInfo(TUsbDescriptor& aDescriptorInfo) const
  {
  LOG_FUNC
  TRAPD(ret, DoGetDescriptorInfoL(aDescriptorInfo));
        if(ret!=KErrNone)
          {
                 LOGTEXT2(_L8("CUsbObexClassController::GetDescriptorInfo leave with code: %d"), ret);
          }
        }

// ---------------------------------------------------------------------------
// Leave version of GetDescriptor info function for fit in Class Controller framework.
// Returns information about the interfaces supported by this class.
// ---------------------------------------------------------------------------
//
void CUsbObexClassController::DoGetDescriptorInfoL(TUsbDescriptor& aDescriptorInfo) const
  {
  LOG_FUNC
        RImplInfoPtrArray implInfoArray;
        CleanupResetAndDestroyPushL(implInfoArray);
        TEComResolverParams resolverParams;
        resolverParams.SetDataType(KSrcsTransportUSB);
        resolverParams.SetWildcardMatch(EFalse);
        REComSession::ListImplementationsL(KCSrcsInterfaceUid, resolverParams, implInfoArray);

        LOGTEXT2(_L8("CUsbObexClassController::DoGetDescriptorInfoL Number of Interfaces is %d"),
                   implInfoArray.Count());
        aDescriptorInfo.iNumInterfaces = (implInfoArray.Count())*KObexNumInterfaces;
        aDescriptorInfo.iLength = 0;

        CleanupStack::PopAndDestroy(&implInfoArray);    
        }

// ---------------------------------------------------------------------------
// From class CActive.
// Will only be called when an asynchronous request is currently active.
// ---------------------------------------------------------------------------
//
void CUsbObexClassController::DoCancel()
  {
  LOG_FUNC

  switch (iState)
    {
    case EUsbServiceStarting:
    case EUsbServiceStopping:
      iObexSM->CancelManageUSBServices();
      break;

    default:
      __ASSERT_DEBUG( EFalse, _USB_PANIC(KObexCcPanicCategory, EBadAsynchronousCall) );
      break;
    }

  iState = EUsbServiceIdle;        
  User::RequestComplete(iRequestStatus, KErrCancel);                  
  }

// ---------------------------------------------------------------------------
// From class CActive.
// Should return KErrNone to avoid an active scheduler panic. This function
// should never be called as there is another mechanism for catching errors.
// ---------------------------------------------------------------------------
//
TInt CUsbObexClassController::RunError(TInt aError)
  {
  LOG_FUNC
  LOGTEXT2(_L8("CUsbObexClassController::RunError aError=%d"), aError);
  return KErrNone;
  }

void CUsbObexClassController::MosmError(TInt aError)
    {
    LOG_FUNC
    LOGTEXT2(_L8("CUsbObexClassController::MosmError aError=%d"), aError);
    Owner().UccnError(aError);
    }

// End of File
