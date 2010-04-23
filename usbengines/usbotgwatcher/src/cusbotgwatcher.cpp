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
#include "cusbstatehosthandle.h"
#include "cusbstatehostdelayhandle.h"
#include "cusbstatehostdelayattachedhandle.h"
#include "cusbstatehostdelaynotattachedhandle.h"
#include "cusbstatehosthandledropping.h"
#include "cusbstatehostundefined.h"
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
    iUsb(aUsbMan), iPersonalityId(KUsbPersonalityIdMTP), iUsbServiceRequest(
            CUsbServiceControl::ERequestUndefined)
    {
    LOG_FUNC
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::ConstructL()
    {
    LOG_FUNC

#ifdef _DEBUG
    SelfTestL();
#endif

#ifndef STIF
    User::LeaveIfError(RProperty::Define(KPSUidUsbWatcher,
            KUsbWatcherIsPeripheralConnected, RProperty::EInt,
            KAlwaysPassPolicy, KLocalServicesPolicy));

    User::LeaveIfError(RProperty::Set(KPSUidUsbWatcher,
            KUsbWatcherIsPeripheralConnected,
            KUsbWatcherPeripheralIsNotConnected));
#endif

    iUsbServiceControl = CUsbServiceControl::NewL(*this, iUsb);

    User::LeaveIfError(iStates.Append(CUsbStateHostUndefined::NewL(*this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAInitiate::NewL(*this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAHost::NewL(*this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAPeripheral::NewL(*this)));

    User::LeaveIfError(iStates.Append(CUsbStateHostDelayAttachedHandle::NewL(
            *this)));
    User::LeaveIfError(iStates.Append(
            CUsbStateHostDelayNotAttachedHandle::NewL(*this)));
    User::LeaveIfError(iStates.Append(
            CUsbStateHostHandleDropping::NewL(*this)));

    iIdPinObserver = CUsbIdPinObserver::NewL();
    iVBusObserver = CUsbVBusObserver::NewL();
    iOtgStateObserver = CUsbOtgStateObserver::NewL();
    iBusActivityObserver = CUsbBusActivityObserver::NewL();
    iHostEventNotificationObserver = CUsbHostEventNotificationObserver::NewL(
            &iUsb);
    iMessageNotificationObserver = CUsbMessageNotificationObserver::NewL(
            &iUsb);

    iHostState = iStates[EUsbStateHostUndefined];

    // Notif manager must be created at least after VBus observer and iHostState initialization
    // to allow USb indicator subscribe to its notifications at construction and check their's current states
    iNotifManager = CUsbNotifManager::NewL(*this);

    iVBusObserver->SubscribeL(*this);
    iOtgStateObserver->SubscribeL(*this);
    iBusActivityObserver->SubscribeL(*this);
    iHostEventNotificationObserver->SubscribeL(*this);
    iMessageNotificationObserver->SubscribeL(*this);
    iIdPinObserver->SubscribeL(*this);

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
    LOG_FUNC

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
    LOG_FUNC

#ifndef STIF
    RProperty::Delete(KPSUidUsbWatcher, KUsbWatcherIsPeripheralConnected);
#endif

    // delete Notif manager before VBus observer, due to USB indicator observes VBus
    delete iNotifManager;

    if (iIdPinObserver)
        {
        TRAP_IGNORE(iIdPinObserver->UnsubscribeL(*this));
        }

    if (iVBusObserver)
        {
        TRAP_IGNORE(iVBusObserver->UnsubscribeL(*this));
        }
    if (iOtgStateObserver)
        {
        TRAP_IGNORE(iOtgStateObserver->UnsubscribeL(*this));
        }
    if (iBusActivityObserver)
        {
        TRAP_IGNORE(iBusActivityObserver->UnsubscribeL(*this));
        }
    if (iHostEventNotificationObserver)
        {
        TRAP_IGNORE(iHostEventNotificationObserver->UnsubscribeL(*this));
        }
    if (iMessageNotificationObserver)
        {
        TRAP_IGNORE(iMessageNotificationObserver->UnsubscribeL(*this));
        }

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
    LOG_FUNC

    LOG1( "aPersonality = %d" , aPersonality);

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
    LOG_FUNC

    iState->CancelSetPersonalityL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::SetPreviousPersonalityL(TRequestStatus& /*aStatus*/)
    {
    LOG_FUNC

    // maybe more complex processing needed here
    iState->SetPreviousPersonalityL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::SetPreviousPersonalityL()
    {
    LOG_FUNC

    iState->SetPreviousPersonalityL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::CancelSetPreviousPersonalityL()
    {
    LOG_FUNC

    iState->CancelSetPreviousPersonalityL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::SetPreviousPreviousPersonalityOnDisconnectL()
    {
    LOG_FUNC

    }

// From IdPin observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::StartSessionL()
    {
    LOG_FUNC

    if (!CanStartSessionL())
        {
        HandleHostProblemL(EUsbWatcherCanNotStartSession,
                EUsbStateHostHandleDropping);
        return;
        }

    iUsbServiceRequest = CUsbServiceControl::EStartUsbService;
    TInt err = iUsbServiceControl->StartL(iPersonalityId);
    if (KErrNone != err)
        {
        LOG1( "Can not start usb services. err = %d" , err);
        HandleHostProblemL(EUsbWatcherCanNotStartUsbServices,
                EUsbStateHostHandleDropping);
        return;
        }

    // call back from iUsbServiceControl->Start(iPersonalityId) call is UsbServiceControlReqCompletedL(TInt aError)
    // so, continue there if everything is OK

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::HandleHostProblemL(TInt aWhatKindOf,
        TUsbStateIds aInState)
    {
    LOG_FUNC
    HostHandle(aInState)->SetWhat(aWhatKindOf);
    ChangeHostStateL(aInState);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::IdPinOnL()
    {
    LOG_FUNC
    StartSessionL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::IdPinOffL()
    {
    LOG_FUNC

    ChangeHostStateL(EUsbStateHostUndefined);

    iNotifManager->CloseAllNotifiers();

    Usb().DisableFunctionDriverLoading();

    iUsbServiceRequest = CUsbServiceControl::EStopUsbService;
    TInt err = iUsbServiceControl->StopL();

    LOG1( "iUsbServiceControl->Stop() err = %d", err );

    if (KErrNone != err)
        {
        LOG( "ErrorStoppingUsbServices" );
        Panic(ECanNotStopUsbServices);
        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::IdPinErrorL(TInt aError)
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    HandleHostProblemL(EUsbWatcherIdPinError, EUsbStateHostHandleDropping);

    }

// From VBus observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::VBusDownL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->VBusDownL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::VBusUpL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->VBusUpL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::VBusObserverErrorL(TInt aError)
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    HandleHostProblemL(EUsbWatcherVBusObserverError,
            EUsbStateHostHandleDropping);
    }

// From OTG state observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::AIdleL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->AIdleL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::AHostL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->AHostL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::APeripheralL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->APeripheralL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::AVBusErrorL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->AVBusErrorL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BIdleL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->BIdleL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BPeripheralL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->BPeripheralL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BHostL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->BHostL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::OtgStateErrorL(TInt aError)
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    HandleHostProblemL(EUsbWatcherOtgStateError, EUsbStateHostHandleDropping);
    }

// From bus activity observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BusIdleL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->BusIdleL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BusActiveL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->BusActiveL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BusActivityErrorL(TInt aError)
    {
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
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->DeviceAttachedL(aTdi);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::DeviceDetachedL(TDeviceEventInformation aTdi)
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->DeviceDetachedL(aTdi);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::DriverLoadSuccessL(TDeviceEventInformation aTdi)
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->DriverLoadSuccessL(aTdi);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::DriverLoadPartialSuccessL(TDeviceEventInformation aTdi)
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->DriverLoadPartialSuccessL(aTdi);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::DriverLoadFailureL(TDeviceEventInformation aTdi)
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->DriverLoadFailureL(aTdi);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::HostEventNotificationErrorL(TInt aError)
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    HandleHostProblemL(EUsbWatcherHostEventNotificationError,
            EUsbStateHostHandleDropping);
    }

// From message notification observer
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::MessageNotificationReceivedL(TInt aMessage)
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->MessageNotificationReceivedL(aMessage);
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::BadHubPositionL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->BadHubPositionL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::VBusErrorL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->VBusErrorL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::SrpReceivedL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->SrpReceivedL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::SessionRequestedL()
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    iHostState->SessionRequestedL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::MessageNotificationErrorL(TInt aError)
    {
    __ASSERT_DEBUG(iHostState != NULL, Panic(EBadHostState));
    HandleHostProblemL(EUsbWatcherMessageNotificationError,
            EUsbStateHostHandleDropping);
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
CUsbStateHostHandle* CUsbOtgWatcher::HostHandle(TUsbStateIds aInState) const
    {
    __ASSERT_DEBUG(iStates[aInState] != NULL, Panic(EBadState));

    return (CUsbStateHostHandle*) iStates[aInState];
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::ChangeStateL(TUsbStateIds aNewStateId)
    {
    LOG_FUNC

    LOG1( "aNewState = %d" , aNewStateId);

    if (NULL != iState)
        {
        iState->JustBeforeLeavingThisStateL();
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
    LOG_FUNC

    LOG1( "aNewState = %d", aNewStateId);

    if (NULL != iHostState)
        {
        iHostState->JustBeforeLeavingThisStateL();
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
    LOG1( "Current state id  = %d" , iHostState->Id());
    LOG1( "IdPin             = %d" , iIdPinObserver->IdPin());
    LOG1( "VBus              = %d" , iVBusObserver->VBus());
    LOG1( "OtgState          = %d" , iOtgStateObserver->OtgState());
    LOG1( "BusActivity       = %d" , iBusActivityObserver->BusActivity());

    TInt isPeripheralConnected(KUsbWatcherPeripheralIsNotConnected);

    RProperty::Get(KPSUidUsbWatcher, KUsbWatcherIsPeripheralConnected,
            isPeripheralConnected);

    LOG1( "IsPeripheralConnected = %d" , isPeripheralConnected);

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
    LOG_FUNC

    LOG1( "aError = %d" , aError);

    switch (aError)
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
            if (iUsbServiceRequest == CUsbServiceControl::EStartUsbService) // Handle only start issues
                {
                HandleHostProblemL(EUsbWatcherCanNotStartUsbServices,
                        EUsbStateHostHandleDropping);
                }

            iUsbServiceRequest = CUsbServiceControl::ERequestUndefined;
            return;
            }
        }

    iUsbServiceRequest = CUsbServiceControl::ERequestUndefined;

    TUsbServiceState serviceState;
    TInt err = iUsb.GetServiceState(serviceState);

    if (KErrNone != err)
        {
        LOG1( "Error when requesting GetServiceState = %d" , err);
        HandleHostProblemL(EUsbWatcherCanNotStartUsbServices,
                EUsbStateHostHandleDropping);
        return;
        }

    switch (serviceState)
        {
        case EUsbServiceIdle: // just stopped usb service
            {
            LOG("UsbServiceState == EUsbServiceIdle" );
            // do nothing
            break;
            }

        case EUsbServiceStarted: // just started usb service
            {
            LOG( "UsbServiceState == EUsbServiceStarted" );

            ChangeHostStateL(EUsbStateHostAInitiate);

            break;
            }
        case EUsbServiceStarting:
            {
            LOG("UsbServiceState == EUsbServiceStarting" );
            // should not receive that, due to call back is called when service stopped or started
            // therefore scream
            // no break statement here
            }
        case EUsbServiceStopping:
            {
            LOG("UsbServiceState == EUsbServiceStopping" );
            // should not receive that, due to call back is called when service stopped or started
            // therefore scream
            // no break statement here
            }
        case EUsbServiceFatalError:
            {
            LOG( "UsbServiceState == EUsbServiceFatalError" );
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
    LOG_FUNC

    // create all the observers
    iIdPinObserver = CUsbIdPinObserver::NewL();
    iVBusObserver = CUsbVBusObserver::NewL();
    iOtgStateObserver = CUsbOtgStateObserver::NewL();
    iBusActivityObserver = CUsbBusActivityObserver::NewL();
    iHostEventNotificationObserver = CUsbHostEventNotificationObserver::NewL(
            &iUsb);
    iMessageNotificationObserver = CUsbMessageNotificationObserver::NewL(
            &iUsb);

    LOG( "Observers getters" );

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

    LOG( "Observers destructors" );

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

    LOG("Creating states");

    User::LeaveIfError(iStates.Append(CUsbStateHostUndefined::NewL(*this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAInitiate::NewL(*this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAHost::NewL(*this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostAPeripheral::NewL(*this)));
    User::LeaveIfError(iStates.Append(CUsbStateHostDelayAttachedHandle::NewL(
            *this)));
    User::LeaveIfError(iStates.Append(
            CUsbStateHostDelayNotAttachedHandle::NewL(*this)));
    User::LeaveIfError(iStates.Append(
            CUsbStateHostHandleDropping::NewL(*this)));

    LOG("Check State()" );

    if (iStates[EUsbStateHostAInitiate] != State(EUsbStateHostAInitiate))
        {
        User::Leave(KErrGeneral);
        }

    LOG("Check CurrentHostState()" );

    iHostState = iStates[EUsbStateHostAInitiate];

    if (iStates[EUsbStateHostAInitiate] != CurrentHostState())
        {
        User::Leave(KErrGeneral);
        }

    LOG("NotifManager and WarningNotifier." );

    CUsbNotifManager* usbnotifmanager = CUsbNotifManager::NewL(*this);
    RNotifier rnotifier;
    User::LeaveIfError(rnotifier.Connect());
    CUsbWarningNotifier* usbnotifier = CUsbWarningNotifier::NewL(rnotifier,
            *usbnotifmanager, EUsbOtgPartiallySupportedDevice);
    usbnotifier->IsFeedbackNeeded();

    LOG( "NotifManager and WarningNotifier destruction." );

    delete usbnotifier;
    rnotifier.Close();
    delete usbnotifmanager;

    // VBus observer is deleted here, due to it is used by usbnotifmanager.usbindicatornotifier
    delete iVBusObserver;
    iVBusObserver = 0;

    // id pin observer is deleted here due to it is used by usbnotifmanager.usbindicatornotifier
    delete iIdPinObserver;
    iIdPinObserver = 0;

    LOG("Destructing states");

    iStates.ResetAndDestroy();

    LOG( "Check UsbServiceControl" );

    CUsbServiceControl* usbServiceControl = CUsbServiceControl::NewL(*this,
            iUsb);
    usbServiceControl->RunError(KErrNone);
    delete usbServiceControl;

    LOG("All completed OK" );
#endif
    return KErrNone;

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//   
void CUsbOtgWatcher::SubscribeL(MUsbOtgWatcherStateObserver& aObserver)
    {
    LOG_FUNC

    // check if the same observer already exist in a list
    if (KErrNotFound != iOtgStateObservers.Find(&aObserver))
        {
        LOG( "Observer already exists" );
        Panic(EObserverAlreadyExists);
        return;
        }
    iOtgStateObservers.AppendL(&aObserver);

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CUsbOtgWatcher::UnsubscribeL(MUsbOtgWatcherStateObserver& aObserver)
    {
    LOG_FUNC

    TInt i(iOtgStateObservers.Find(&aObserver));
    if (KErrNotFound == i)
        {
        LOG( "Observer not found" );
        Panic(ECanNotFindUsbOtgWatcherStateObserver);
        return;
        }

    iOtgStateObservers.Remove(i);
    }
