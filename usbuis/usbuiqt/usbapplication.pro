#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:  Project definition file for project USB Settings Application
#

TEMPLATE = app
TARGET = usbapplication
QT += core \
    gui \
    xml
HEADERS += inc/usbuisettingmodel.h \
    inc/usbuimodelactive.h \
    inc/usbviewmanager.h \
    inc/usbmainview.h
SOURCES += src/usbuisettingmodel.cpp \
    src/usbuimodelactive.cpp \
    src/usbviewmanager.cpp \
    src/usbmainview.cpp \
    src/main.cpp \
    src/usbapplication_reg.rss
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
LIBS += -lcentralrepository
LIBS += -lusbwatcher
LIBS += -lxqsettingsmanager
LIBS += -lusbman

FORMS += 
RESOURCES += src/usbicons.qrc \
    src/usbapplication.qrc
CONFIG += qt \
    hb

symbian: {
    TARGET.UID3 = 0x2002BCA3
    TARGET.CAPABILITY = LocalServices \
        WriteDeviceData

	# for pkg
	usbuitranslation.sources = src/usbapplication.qm
	usbuitranslation.path = /resource
	DEPLOYMENT += usbuitranslation

    BLD_INF_RULES.prj_exports += "./src/usbapplication.qm z:/resource/qt/translations/usbapplication.qm
}

# placeholder for creating sis file
createsis.commands += ( makesis -v usbapplication.pkg ); \
    && \
    ( signsis.exe usbapplication.sis usbapplication_signed.sisx Nokia_RnDCert_02.der Nokia_RnDCert_02.key ); \
    && \
    ( del usbapplication.sis );
QMAKE_EXTRA_TARGETS += createsis

BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>" \
        "rom/usbsettings.iby   CORE_MW_LAYER_IBY_EXPORT_PATH(usbsettings.iby)" \
        "rom/usbsettingsresources.iby   LANGUAGE_MW_LAYER_IBY_EXPORT_PATH(usbsettingsresources.iby)"
