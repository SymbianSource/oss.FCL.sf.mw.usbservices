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

#include <HbMainWindow.h>
#include "UsbViewManager.h"
#include "UsbMainView.h"


UsbViewManager::UsbViewManager( HbMainWindow* mainWindow, QObject *parent )
:   QObject( parent ),
    mWindow(mainWindow)
    {
    }

UsbViewManager::~UsbViewManager()
    {
    
    }

/*!
    Create views(main view, gadget gallery view and gadget details view).
    Add them to MainWindow.
 */
void UsbViewManager::addView()
    {
    // Create main view and add
    mMainView = new UsbMainView( this );
    mWindow->addView(mMainView);
      
    }

