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


#include <hbapplication.h>
#include <hbmainwindow.h>
#include <QTranslator>
#include <eikenv.h>
#include "usbviewmanager.h"

int main(int argc, char *argv[])
{
    HbApplication app(argc, argv);

    // add translator for application library
    QString locale = QLocale::system().name();
    QTranslator translator;
    QString filename = QString("usbapplication_") + locale;
#ifdef Q_OS_SYMBIAN    
    // TRAP is must here, otherwise it crashes
TRAP_IGNORE(
    bool loaded(false);
    loaded = translator.load( filename, QString("z:/resource") );
    if (!loaded)
        translator.load(filename, QString("c:/resource") );
);
#else
    translator.load(filename, QString("resource") );
#endif //Q_OS_SYMBIAN    
    app.installTranslator(&translator);
    
    HbMainWindow mainWindow;

    // create the view manager, which handles 3 HbViews and add into mainWindow.
    UsbViewManager* mViewManager = new UsbViewManager( &mainWindow, &app );
    mViewManager->addView();
    
    mainWindow.show();
    return app.exec();
}
