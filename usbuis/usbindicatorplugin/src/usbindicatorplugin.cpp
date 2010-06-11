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
#include "usbindicatorplugin.h"
#include <QString>
#include <QLocale>
#include <QApplication>
#include "usbindicator.h"
#include "usbaddressedindicator.h"

Q_EXPORT_PLUGIN(UsbIndicatorPlugin)

/*!
   UsbIndicatorPlugin::UsbIndicatorPlugin
*/
UsbIndicatorPlugin::UsbIndicatorPlugin() : mError(0), mTranslatorLoaded(0)
{
}

/*!
   UsbIndicatorPlugin::~UsbIndicatorPlugin
*/
UsbIndicatorPlugin::~UsbIndicatorPlugin()
{
    if (mTranslatorLoaded) {
        QApplication::removeTranslator(&mTranslator); 
    }
}

/*!
   UsbIndicatorPlugin::indicatorTypes
   Return notification types this plugin implements
*/
QStringList UsbIndicatorPlugin::indicatorTypes() const
{
    QStringList types; 
    types << ConnectedIndicator;
    types << AddressedIndicator;
    return types;
}



/*!
   UsbIndicatorPlugin::createIndicator
   Creates and returns the HbIndicatorInterface
*/
HbIndicatorInterface* UsbIndicatorPlugin::createIndicator(const QString &indicatorType)
{  
    if (!mTranslatorLoaded) {
        // add translator for application library
        QString locale = QLocale::system().name();
        QString filename = QString("usbindimenu_") + locale;             
        bool success = mTranslator.load(filename, QString("z:/resource/qt/translations"));
        QApplication::installTranslator(&mTranslator);
        mTranslatorLoaded = true;    
    }
        
    HbIndicatorInterface *indicator = NULL;    
    if (indicatorType == ConnectedIndicator) {
        indicator  = new USBIndicator(indicatorType);
    }
    else if (indicatorType == AddressedIndicator) {
        indicator = new UsbAddressedIndicator(indicatorType); 
    }
    return indicator;
}


