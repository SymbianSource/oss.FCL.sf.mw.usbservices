#
# ============================================================================
#  Name        : usbapplication.pro
#  Part of     : USB / USB Settings Application       *** Info from the SWAD
#  Description : Project definition file for project USB Settings Application
#  Version     : %version: 3 % << Don't touch! Updated by Synergy at check-out.
#
#  Copyright © 2009 Nokia.  All rights reserved.
#  This material, including documentation and any related computer
#  programs, is protected by copyright controlled by Nokia.  All
#  rights are reserved.  Copying, including reproducing, storing,
#  adapting or translating, any or all of this material requires the
#  prior written consent of Nokia.  This material also contains
#  confidential information which may not be disclosed to others
#  without the prior written consent of Nokia.
# ============================================================================
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
LIBS += -lhbcore
LIBS += -lhbwidgets
LIBS += -lhbtools
FORMS += 
RESOURCES += src/usbicons.qrc \
    src/usbapplication.qrc
CONFIG += qt \
    hb
    
TRANSLATIONS=usbapplication.ts
    
symbian: {

    TARGET.UID3 = 0x2002BCA3
    TARGET.CAPABILITY = LocalServices \
        WriteDeviceData

	# for pkg
	usbuitranslation.sources = src/usbapplication.qm
	usbuitranslation.path = /resource
	DEPLOYMENT += usbuitranslation

    BLD_INF_RULES.prj_exports += "./src/usbapplication.qm z:/resource/usbapplication.qm"

}

# placeholder for creating sis file
createsis.commands += ( makesis -v usbapplication.pkg ); \
    && \
    ( signsis.exe usbapplication.sis usbapplication_signed.sisx Nokia_RnDCert_02.der Nokia_RnDCert_02.key ); \
    && \
    ( del usbapplication.sis );
QMAKE_EXTRA_TARGETS += createsis

BLD_INF_RULES.prj_exports += \
        "rom/usbsettings.iby   CORE_MW_LAYER_IBY_EXPORT_PATH(usbsettings.iby)" \