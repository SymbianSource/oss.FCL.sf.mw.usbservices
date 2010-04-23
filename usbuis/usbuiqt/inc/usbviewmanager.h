/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef USBVIEWMANAGER_H
#define USBVIEWMANAGER_H

#include "usbmainview.h"


class HbMainWindow;

class UsbViewManager : public QObject
    {
    Q_OBJECT

public:
    explicit UsbViewManager(HbMainWindow* mainWindow, QObject *parent = 0 );
    ~UsbViewManager();
    
    void addView();

private: 
    HbMainWindow* mWindow;
    UsbMainView* mMainView; // owned

    };


#endif /* USBVIEWMANAGER_H */
