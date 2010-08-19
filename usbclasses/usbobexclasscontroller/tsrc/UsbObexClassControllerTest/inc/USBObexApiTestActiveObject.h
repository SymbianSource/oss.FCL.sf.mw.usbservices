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

#ifndef USBOBEXAPITESTACTIVEOBJECT_H_
#define USBOBEXAPITESTACTIVEOBJECT_H_

#include <e32base.h>
#include <StifLogger.h>
#include <CUsbClassControllerPlugIn.h>

class CStifLogger;
class CUsbClassControllerPlugIn;

class CUSBObexApiTestActiveObject : public CActive
{
public:
	static CUSBObexApiTestActiveObject* NewL
	    (CStifLogger* aLog, 
	     CUsbClassControllerPlugIn* aUsbClassControllerPlugIn);
	virtual ~CUSBObexApiTestActiveObject();
	
	void Start();
	void Stop();

private:
    CUSBObexApiTestActiveObject
        (CStifLogger* aLog, 
         CUsbClassControllerPlugIn* aUsbClassControllerPlugIn);
    void RunL();
    void DoCancel();
    void ConstructL();
  
    
private:
    CStifLogger* iLog;
    CUsbClassControllerPlugIn* iUsbClassControllerPlugIn;
    TUsbServiceState iUsbServiceState;
};

#endif /*USBOBEXAPITESTACTIVEOBJECT_H_*/
