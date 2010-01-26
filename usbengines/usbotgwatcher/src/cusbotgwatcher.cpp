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
* Description:  Implementation
 *
*/


#include <usbpersonalityids.h>
#include <usbuinotif.h>
#include <UsbWatcherInternalPSKeys.h>

#include "cusbotgwatcher.h"
#include "cusbstate.h"

#include "cusbstatehostainitiate.h"
#include "cusbstatehostahost.h"
#include "cusbstatehostaperipheral.h"
#include "cusbstatehostaidle.h"
#include "cusbstatehosthandle.h"

#include "cusbwarningnotifier.h"

#ifndef STIF
#include "cusbnotifmanager.h"
#else
#include "mockcusbnotifmanager.h"
#endif

#include "errors.h"
#include "debug.h"
#include "panic.h"

_LIT_SECURITY_POLICY_PASS( KAlwaysPassPolicy );
_LIT_SECURITY_POLICY_C1( KLocalServicesPolicy, ECapabilityLocalServices );

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbOtgWatcher::CUsbOtgWatcher(RUsb& aUsbMan) :
    iUsb(aUsbMan), iPersonalityId(KUsbPersonalityIdMTP)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::CUsbOtgWatcher" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::ConstructL()
    {

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::ConstructL" ) );

#ifdef _DEBUG
    SelfTestL();
#endif

#ifndef STIF
    User::LeaveIfError(RProperty::Define( KPSUidUsbWatcher,
            KUsbWatcherIsPeripheralConnected, RProperty::EInt, KAlwaysPassPolicy,
                KLocalServicesPolicy ) );

    User::LeaveIfError( RProperty::Set( KPSUidUsbWatcher,
            KUsbWatcherIsPeripheralConnected,
            KUsbWatcherPeripheralIsNotConnected ) );
#endif
    
    iUsbServiceControl = CUsbServiceControl::NewL(this, iUsb);

    User::LeaveIfError(iStates.Append(CUsbState::NewL(this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAInitiate::NewL(this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAHost::NewL(this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAPeripheral::NewL(this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAIdle::NewL(this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostHandle::NewL(this)));

    iIdPinObserver = CUsbIdPinObserver::NewL();
    iVBusObserver = CUsbVBusObserver::NewL();
    iOtgStateObserver = CUsbOtgStateObserver::NewL();
    iBusActivityObserver = CUsbBusActivityObserver::NewL();
    iHostEventNotificationObserver = CUsbHostEventNotificationObserver::NewL(
            &iUsb);
    iMessageNotificationObserver = CUsbMessageNotificationObserver::NewL(
            &iUsb);
    
    iHostState = iStates[EUsbStateUndefined];
    
    // Notif manager must be created at least after VBus observer and iHostState initialization
    // to allow USb indicator subscribe to its notifications at construction and check their's current states
    iNotifManager = CUsbNotifManager::NewL(this);

    iIdPinObserver->SubscribeL(this);

    if (CUsbIdPinObserver::EIdPinOn == iIdPinObserver->IdPin())
        {
        StartSessionL();
        }

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CUsbOtgWatcher::IsDeviceA()
    {
    __ASSERT_DEBUG(iIdPinObserver != NULL, Panic(EIdPinObserverNULLPointer));
    return (iIdPinObserver->IdPin() == CUsbIdPinObserver::EIdPinOn
                                                                   ? ETrue
                                                                      : EFalse);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CUsbOtgWatcher* CUsbOtgWatcher::NewL(RUsb& aUsbMan)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::NewL" ) );

    CUsbOtgWatcher* self = new (ELeave) CUsbOtgWatcher(aUsbMan);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbOtgWatcher::~CUsbOtgWatcher()
    {
		FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::~CUsbOtgWatcher" ) );
        
#ifndef STIF
    RProperty::Delete( KPSUidUsbWatcher, KUsbWatcherIsPeripheralConnected );
#endif
    
    // delete Notif manager before VBus observer, due to USB indicator observes VBus
    delete iNotifManager;
    
    delete iIdPinObserver;
    delete iVBusObserver;
    delete iOtgStateObserver;
    delete iBusActivityObserver;
    delete iHostEventNotificationObserver;
    delete iMessageNotificationObserver;

    iOtgStateObservers.Close();
    
    // Destroy states
    iStates.ResetAndDestroy();
    iStates.Close();
    
    delete iUsbServiceControl;

    iUsb.Close();

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::SetPersonalityL(TRequestStatus& /*aStatus*/,
        TInt aPersonality)
    {

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SetPersonalityL aPersonality = %d" ), aPersonality));

    // watcher keeps this value, no need to pass it to request object
    // state can read it from watcher when needed
    iPersonalityId = aPersonality;

    // more complex processing has to be here, remember aStatus, etc.
    // pass the request to device state machine
    iState->SetPersonalityL();

    // the request is not passed to host state machine due to host ignores SetPersonality

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::CancelSetPersonalityL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::CancelSetPersonalityL" ) );

    iState->CancelSetPersonalityL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::SetPreviousPersonalityL(TRequestStatus& /*aStatus*/)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SetPreviousPersonalityL" ) );

    // maybe more complex processing needed here
    iState->SetPreviousPersonalityL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::SetPreviousPersonalityL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SetPreviousPersonalityL" ) );

    iState->SetPreviousPersonalityL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::CancelSetPreviousPersonalityL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::CancelSetPreviousPersonalityL" ) );

    iState->CancelSetPreviousPersonalityL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::SetPreviousPreviousPersonalityOnDisconnectL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SetPreviousPreviousPersonalityOnDisconnectL" ) );

    }

// From IdPin observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::StartSessionL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::StartSessionL" ) );

    if (!CanStartSessionL())
        {
        HandleHostProblemL(EUsbWatcherErrorInConnection);
        return;
        }

    TInt err = iUsbServiceControl->StartL(iPersonalityId);
    if (KErrNone != err)
        {
            FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgWatcher::StartSessionL Can not start usb services. reason = %d" ), err));
        HandleHostProblemL(EUsbWatcherCanNotStartUsbServices);
        return;
        }

    // call back from iUsbServiceControl->Start(iPersonalityId) call is UsbServiceControlReqCompletedL(TInt aError)
    // so, continue there if everything is OK

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::HandleHostProblemL(TInt aWhatKindOf)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::HandleProblemL" ) );
    HostHandle()->SetWhat(aWhatKindOf);
    ChangeHostStateL(EUsbStateHostHandle);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::IdPinOnL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::IdPinOnL" ) );
    StartSessionL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::IdPinOffL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::IdPinOffL" ) );

    // for example, USB failed to start    
    if(NULL == iHostState) return;

    iVBusObserver->UnsubscribeL(this);
    iOtgStateObserver->UnsubscribeL(this);
    iBusActivityObserver->UnsubscribeL(this);
    iHostEventNotificationObserver->UnsubscribeL(this);
    iMessageNotificationObserver->UnsubscribeL(this);

    iHostState->JustBeforeLeavingThisStateL();

    iHostState = NULL;

    iNotifManager->CloseAllNotifiers();

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::IdPinOffL Before DisableFunctionDriverLoad " ) );
    Usb().DisableFunctionDriverLoading();

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::IdPinOffL Before TryStop" ) );

    TInt err = iUsbServiceControl->StopL();

    if (KErrNone != err)
        {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::IdPinOnL ErrorStoppingUsbServices" ) );
        Panic(ECanNotStopUsbServices);
        }

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::IdPinOffL iUsbServiceControl->Stop() OK" ) );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::IdPinErrorL(TInt aError)
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::IdPinErrorL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));  
    HandleHostProblemL(EUsbWatcherIdPinError);

    }

// From VBus observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::VBusDownL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::VBusDownL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->VBusDownL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::VBusUpL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::VBusUpL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->VBusUpL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::VBusObserverErrorL(TInt aError)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::VBusObserverErrorL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    HandleHostProblemL(EUsbWatcherVBusObserverError);
    }

// From OTG state observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::AIdleL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::AIdleL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->AIdleL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::AHostL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::AHostL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->AHostL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::APeripheralL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::APeripheralL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->APeripheralL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::AVBusErrorL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::AVBusErrorL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->AVBusErrorL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BIdleL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::BIdleL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->BIdleL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BPeripheralL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::BPeripheralL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->BPeripheralL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BHostL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::BHostL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->BHostL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::OtgStateErrorL(TInt aError)
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::OtgStateErrorL" ) );
__ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));  
HandleHostProblemL(EUsbWatcherOtgStateError);
    }

// From bus activity observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BusIdleL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::BusIdleL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->BusIdleL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BusActiveL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::BusActiveL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->BusActiveL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BusActivityErrorL(TInt aError)
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::BusActivityErrorL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    // no action, continue
    }

// From Host Event notification observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::DeviceAttachedL(TDeviceEventInformation aTdi)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::DeviceAttachedL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->DeviceAttachedL(aTdi);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::DeviceDetachedL(TDeviceEventInformation aTdi)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::DeviceDetachedL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->DeviceDetachedL(aTdi);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::DriverLoadSuccessL(TDeviceEventInformation aTdi)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::DriverLoadSuccessL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->DriverLoadSuccessL(aTdi);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::DriverLoadPartialSuccessL(TDeviceEventInformation aTdi)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::DriverLoadPartialSuccessL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->DriverLoadPartialSuccessL(aTdi);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::DriverLoadFailureL(TDeviceEventInformation aTdi)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::DriverLoadFailureL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->DriverLoadFailureL(aTdi);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::HostEventNotificationErrorL(TInt aError)
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::HostEventNotificationErrorL" ) );
__ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));  
HandleHostProblemL(EUsbWatcherHostEventNotificationError);
    }

// From message notification observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::MessageNotificationReceivedL(TInt aMessage)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::MessageNotificationReceivedL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->MessageNotificationReceivedL(aMessage);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BadHubPositionL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::BadHubPositionL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->BadHubPositionL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::VBusErrorL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::VBusErrorL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->VBusErrorL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::SrpReceivedL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SrpReceivedL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->SrpReceivedL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::SessionRequestedL()
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SessionRequestedL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->SessionRequestedL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::MessageNotificationErrorL(TInt aError)
    {
    FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::MessageNotificationErrorL" ) );
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));    
    HandleHostProblemL(EUsbWatcherMessageNotificationError);
    }

// ---------------------------------------------------------------------------
// getters
// ---------------------------------------------------------------------------
//
CUsbIdPinObserver* CUsbOtgWatcher::IdPinObserver() const
    {
    return iIdPinObserver;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbVBusObserver* CUsbOtgWatcher::VBusObserver() const
    {
    return iVBusObserver;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbOtgStateObserver* CUsbOtgWatcher::OtgStateObserver() const
    {
    return iOtgStateObserver;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbBusActivityObserver* CUsbOtgWatcher::BusActivityObserver() const
    {
    return iBusActivityObserver;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbHostEventNotificationObserver* CUsbOtgWatcher::HostEventNotificationObserver() const
    {
    return iHostEventNotificationObserver;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbMessageNotificationObserver* CUsbOtgWatcher::MessageNotificationObserver() const
    {

    return iMessageNotificationObserver;

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
RUsb& CUsbOtgWatcher::Usb()
    {
    return iUsb;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbState* CUsbOtgWatcher::CurrentState() const
    {
    return iState;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbState* CUsbOtgWatcher::CurrentHostState() const
    {
    return iHostState;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbState* CUsbOtgWatcher::State(TUsbStateIds aStateId) const
    {
    return iStates[aStateId];
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbStateHostHandle* CUsbOtgWatcher::HostHandle() const
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::HostHandle" ) );
    __ASSERT_DEBUG(iStates[EUsbStateHostHandle] != NULL, Panic(EBadState));
    return (CUsbStateHostHandle*) iStates[EUsbStateHostHandle];
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::ChangeStateL(TUsbStateIds aNewStateId)
    {

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgWatcher::ChangeStateL aNewState = %d" ), aNewStateId));

    if (NULL != iState)
        {
        if (aNewStateId == iState->Id())
            {
            return; // we already in the target state
            }
        else
            {

            iState->JustBeforeLeavingThisStateL();
            }
        }

    // sets new state	

    // be careful to add states into States in exactly the same order as they enumerated
    // this method will work right as long as states, which in the enumeration, is added to the list iStates in the same order as they enumerated
    // and no one state is added, if all previous states are not added. no need to add all states, but if added - previous all must be added.
    // For the general case, when some states added, some not, this method has to implement search by state ID. for this CUsbOtgWatcher::Id() is maintained.

    iState = iStates[aNewStateId];

    __ASSERT_DEBUG(iState != NULL, Panic(EBadState));

    iState->JustAdvancedToThisStateL(); // do any initial activity, once advanced to the state

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::ChangeHostStateL(TUsbStateIds aNewStateId)
    {

        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgWatcher::ChangeHostStateL aNewState = %d" ), aNewStateId));

    if (NULL != iHostState)
        {
        if (aNewStateId == iHostState->Id())
            {
            return; // we already in the target state
            }
        else
            {

            iHostState->JustBeforeLeavingThisStateL();
            }
        }

    // set new state	

    // be careful to add states into States in exactly the same order as they enumerated
    // this method will work right as long as states, which in the enumeration, is added to the list iStates in the same order as they enumerated
    // and no one state is added, if all previous states are not added. no need to add all states, but if added - previous all must be added.
    // For the general case, when some states added, some not, this method has to implement search by state Id. for this CUsbOtgWatcher::Id() is maintained.

    iHostState = iStates[aNewStateId];

    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadState));

    iHostState->JustAdvancedToThisStateL(); // checks if there are conditions for advancing to another state(s)
    
    // notify state change to observers
    for (TInt i(0); i < iOtgStateObservers.Count(); ++i)
        {
        iOtgStateObservers[i]->OtgWatcherStateChangedL(iHostState->Id());
        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TBool CUsbOtgWatcher::CanStartSessionL()
    {
    // define policy here
    return ETrue;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::PrintStateToLog()
    {
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgWatcher::PrintStateToLog IdPin       = %d" ), iIdPinObserver->IdPin()));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgWatcher::PrintStateToLog VBus        = %d" ), iVBusObserver->VBus()));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgWatcher::PrintStateToLog OtgState    = %d" ), iOtgStateObserver->OtgState()));
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgWatcher::PrintStateToLog BusActivity = %d" ), iBusActivityObserver->BusActivity()));
        
        TInt isPeripheralConnected(KUsbWatcherPeripheralIsNotConnected);
        
        RProperty::Get( KPSUidUsbWatcher, KUsbWatcherIsPeripheralConnected,
                isPeripheralConnected  );
        
        FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgWatcher::PrintStateToLog IsPeripheralConnected = %d" ), isPeripheralConnected));
        
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CUsbNotifManager* CUsbOtgWatcher::NotifManager()
    {
    return iNotifManager;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::UsbServiceControlReqCompletedL(TInt aError)
    {
    FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgWatcher::UsbServiceControlReqCompleted Error returned by UsbServiceControl = %d" ), aError));    
    
    switch(aError)
    {
    case KErrInUse: 
    // usb services already started (this might happen if more than one idpin on event come)
        {
        return;
        }
        
    case KErrNone:
        {
        break; // do normal routine
        }
        
    default: // handle the issue
        {
        HandleHostProblemL(EUsbWatcherCanNotStartUsbServices);
        return;
        }
    }

    TUsbServiceState serviceState;
    TInt err = iUsb.GetServiceState(serviceState);

    if (KErrNone != err)
        {
            FTRACE( FPrint(_L( "[USBOTGWATCHER]\tCUsbOtgWatcher::UsbServiceControlReqCompleted Error when requesting GetServiceState = %d" ), err));
        HandleHostProblemL(EUsbWatcherCanNotStartUsbServices);
        return;
        }

    switch (serviceState)
        {
        case EUsbServiceIdle: // just stopped usb service
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::UsbServiceControlReqCompletedL UsbServiceState == EUsbServiceIdle" ) );
            // do nothing
            break;
            }

        case EUsbServiceStarted: // just started usb service
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::UsbServiceControlReqCompletedL UsbServiceState == EUsbServiceStarted" ) );

            iHostState = iStates[EUsbStateHostAInitiate];

            iHostState->JustAdvancedToThisStateL(); // do any initial activity, once advanced to the state 

            iVBusObserver->SubscribeL(this);
            iOtgStateObserver->SubscribeL(this);
            iBusActivityObserver->SubscribeL(this);
            iHostEventNotificationObserver->SubscribeL(this);
            iMessageNotificationObserver->SubscribeL(this);

            break;
            }
        case EUsbServiceStarting:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::UsbServiceControlReqCompletedL UsbServiceState == EUsbServiceStarting" ) );
            // should not receive that, due to call back is called when service stopped or started
            // therefore scream
            }
        case EUsbServiceStopping:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::UsbServiceControlReqCompletedL UsbServiceState == EUsbServiceStopping" ) );
            // should not receive that, due to call back is called when service stopped or started
            // therefore scream
            }
        case EUsbServiceFatalError:
            {
                FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::UsbServiceControlReqCompletedL UsbServiceState == EUsbServiceFatalError" ) );
            Panic(EUnexpectedUsbServiceState);
            break;
            }

        default:
            {
            Panic(EUnknownUsbServiceState);
            }
        }

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CUsbOtgWatcher::SelfTestL()
    {
#ifdef _DEBUG

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SelfTestL Creating observers." ) );

    // create all the observers
    iIdPinObserver = CUsbIdPinObserver::NewL();
    iVBusObserver = CUsbVBusObserver::NewL();
    iOtgStateObserver = CUsbOtgStateObserver::NewL();
    iBusActivityObserver =
            CUsbBusActivityObserver::NewL();
    iHostEventNotificationObserver =
            CUsbHostEventNotificationObserver::NewL(&iUsb);
    iMessageNotificationObserver =
            CUsbMessageNotificationObserver::NewL(&iUsb);

         FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SelfTestL Observers getters." ) );

    if (iIdPinObserver != IdPinObserver())
        {
        User::Leave(KErrGeneral);
        }

    if (iOtgStateObserver != OtgStateObserver())
        {
        User::Leave(KErrGeneral);
        }

    if (iBusActivityObserver != BusActivityObserver())
        {
        User::Leave(KErrGeneral);
        }

    if (iHostEventNotificationObserver != HostEventNotificationObserver())
        {
        User::Leave(KErrGeneral);
        }

    if (iMessageNotificationObserver != MessageNotificationObserver())
        {
        User::Leave(KErrGeneral);
        }

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SelfTestL Observers destructors." ) );

    // idpinobserver is deleted later        
    // Vbus observer is deleted later
    
    delete iOtgStateObserver;
    iOtgStateObserver = 0;
    delete iBusActivityObserver;
    iBusActivityObserver = 0;
    delete iHostEventNotificationObserver;
    iHostEventNotificationObserver = 0;
    delete iMessageNotificationObserver;
    iMessageNotificationObserver = 0;

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SelfTestL Creating states." ) );

    User::LeaveIfError(iStates.Append(CUsbState::NewL(this)));    
    User::LeaveIfError(iStates.Append(CUsbStateHostAInitiate::NewL(this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAHost::NewL(this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAPeripheral::NewL(this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAIdle::NewL(this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostHandle::NewL(this)));

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SelfTestL Check State()" ) );

    if (iStates[EUsbStateHostAInitiate] != State(EUsbStateHostAInitiate))
        {
        User::Leave(KErrGeneral);
        }
        
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SelfTestL Check CurrentHostState()" ) );    
        
    iHostState = iStates[EUsbStateHostAInitiate];
		
		if (iStates[EUsbStateHostAInitiate] != CurrentHostState())
        {
        User::Leave(KErrGeneral);
        }
		
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SelfTestL NotifManager and WarningNotifier." ) );

    CUsbNotifManager* usbnotifmanager = CUsbNotifManager::NewL(this);
    RNotifier rnotifier;
    User::LeaveIfError(rnotifier.Connect());
    CUsbWarningNotifier* usbnotifier = CUsbWarningNotifier::NewL(rnotifier,
            usbnotifmanager, EUsbOtgPartiallySupportedDevice);
    usbnotifier->IsFeedbackNeeded();

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SelfTestL NotifManager and WarningNotifier destruction." ) );

    delete usbnotifier;
    rnotifier.Close();
    delete usbnotifmanager; 
    
    // VBus observer is deleted here, due to it is used by usbnotifmanager.usbindicatornotifier
    delete iVBusObserver;
    iVBusObserver = 0;
    
    // id pin observer is deleted here due to it is used by usbnotifmanager.usbindicatornotifier
    delete iIdPinObserver;
    iIdPinObserver = 0;
        
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SelfTestL Destructing states." ) );
        
    iStates.ResetAndDestroy();

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SelfTestL Check UsbServiceControl" ) );

    CUsbServiceControl* usbServiceControl = CUsbServiceControl::NewL(this,
            iUsb);
    usbServiceControl->RunError(KErrNone);
    delete usbServiceControl;

        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SelfTestL All completed OK." ) );
#endif
        return KErrNone;

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//   
void CUsbOtgWatcher::SubscribeL(MUsbOtgWatcherStateObserver* aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::SubscribeL" ) );

    User::LeaveIfError(iOtgStateObservers.Append(aObserver));

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::UnsubscribeL(MUsbOtgWatcherStateObserver* aObserver)
    {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::UnsubscribeL" ) );
    if (0 == iOtgStateObservers.Count()) // no items
        {
        FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::UnsubscribeL No observers" ) );
        return;
        }
    
    TInt i(0);
    while (i < iOtgStateObservers.Count() && aObserver != iOtgStateObservers[i])
        ++i;

    if (aObserver == iOtgStateObservers[i]) // found
        {
        iOtgStateObservers.Remove(i);
        }
    else
        {
            FLOG( _L( "[USBOTGWATCHER]\tCUsbOtgWatcher::UnsubscribeL CanNotGetUsbOtgStateWatcherObserver" ) );
        Panic(ECanNotFindUsbOtgWatcherStateObserver);
        }
    }
