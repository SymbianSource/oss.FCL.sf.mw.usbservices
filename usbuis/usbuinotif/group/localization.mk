#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
# Description:
#
MAKE = make

VISUAL_CFG = RELEASE
ifeq "$(CFG)" "UDEB"
VISUAL_CFG = DEBUG
endif

do_nothing :
	@rem do_nothing

MAKMAKE: create_temps pre_targetdeps 

LIB: create_temps pre_targetdeps 

BLD: create_temps pre_targetdeps 

CLEAN: extension_clean deployment_clean

CLEANLIB: do_nothing

RESOURCE: create_qm

create_qm : 
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ar.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_ar.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ar.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ar.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ar.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ar.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_eu.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_eu.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_eu.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_eu.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_eu.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_eu.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_bg.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_bg.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_bg.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_bg.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_bg.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_bg.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ca.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_ca.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ca.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ca.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ca.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ca.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_hr.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_hr.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_hr.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_hr.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_hr.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_hr.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_cs.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_cs.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_cs.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_cs.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_cs.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_cs.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_da.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_da.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_da.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_da.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_da.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_da.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_nl.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_nl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_nl.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_nl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_nl.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_nl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_en.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_en.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_en.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_en.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_en.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_en.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_en_US.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_en_US.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_en_US.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_en_US.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_en_US.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_en_US.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_et.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_et.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_et.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_et.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_et.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_et.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_fi.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_fi.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_fi.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_fi.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_fi.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_fi.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_fr.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_fr.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_fr.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_fr.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_fr.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_fr.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_fr_CA.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_fr_CA.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_fr_CA.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_fr_CA.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_fr_CA.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_fr_CA.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_gl.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_gl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_gl.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_gl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_gl.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_gl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_de.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_de.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_de.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_de.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_de.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_de.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_el.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_el.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_el.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_el.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_el.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_el.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_he.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_he.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_he.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_he.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_he.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_he.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_hi.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_hi.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_hi.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_hi.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_hi.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_hi.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_zh_HK.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_zh_HK.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_zh_HK.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_zh_HK.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_zh_HK.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_zh_HK.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_hu.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_hu.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_hu.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_hu.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_hu.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_hu.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_is.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_is.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_is.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_is.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_is.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_is.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_id.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_id.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_id.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_id.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_id.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_id.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_it.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_it.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_it.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_it.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_it.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_it.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ja.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_ja.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ja.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ja.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ja.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ja.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ko.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_ko.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ko.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ko.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ko.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ko.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_lv.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_lv.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_lv.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_lv.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_lv.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_lv.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_lt.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_lt.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_lt.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_lt.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_lt.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_lt.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ms.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_ms.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ms.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ms.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ms.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ms.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_no.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_no.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_no.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_no.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_no.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_no.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_pl.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_pl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_pl.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_pl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_pl.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_pl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_pt.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_pt.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_pt.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_pt.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_pt.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_pt.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_pt_BR.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_pt_BR.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_pt_BR.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_pt_BR.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_pt_BR.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_pt_BR.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_zh.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_zh.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_zh.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_zh.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_zh.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_zh.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ro.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_ro.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ro.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ro.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ro.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ro.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ru.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_ru.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ru.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ru.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ru.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ru.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_sr.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_sr.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_sr.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_sr.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_sr.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_sr.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_sk.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_sk.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_sk.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_sk.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_sk.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_sk.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_sl.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_sl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_sl.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_sl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_sl.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_sl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_es.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_es.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_es.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_es.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_es.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_es.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_es_419.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_es_419.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_es_419.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_es_419.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_es_419.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_es_419.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_sv.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_sv.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_sv.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_sv.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_sv.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_sv.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_tl.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_tl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_tl.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_tl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_tl.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_tl.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_zh_TW.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_zh_TW.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_zh_TW.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_zh_TW.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_zh_TW.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_zh_TW.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_th.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_th.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_th.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_th.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_th.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_th.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_tr.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_tr.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_tr.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_tr.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_tr.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_tr.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_uk.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_uk.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_uk.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_uk.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_uk.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_uk.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ur.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_ur.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ur.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ur.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_ur.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ur.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_vi.ts -qm /epoc32/data/z/resource/qt/translations/usbdialogs_vi.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_vi.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_vi.qm
	lrelease -silent -idbased /epoc32/include/platform/qt/translations/usbdialogs_vi.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_vi.qm

FREEZE: do_nothing

SAVESPACE: do_nothing

RELEASABLES: list_qm

list_qm : 
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_ar.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ar.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ar.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_eu.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_eu.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_eu.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_bg.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_bg.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_bg.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_ca.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ca.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ca.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_hr.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_hr.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_hr.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_cs.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_cs.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_cs.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_da.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_da.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_da.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_nl.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_nl.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_nl.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_en.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_en.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_en.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_en_US.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_en_US.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_en_US.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_et.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_et.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_et.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_fi.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_fi.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_fi.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_fr.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_fr.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_fr.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_fr_CA.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_fr_CA.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_fr_CA.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_gl.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_gl.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_gl.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_de.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_de.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_de.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_el.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_el.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_el.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_he.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_he.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_he.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_hi.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_hi.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_hi.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_zh_HK.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_zh_HK.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_zh_HK.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_hu.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_hu.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_hu.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_is.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_is.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_is.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_id.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_id.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_id.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_it.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_it.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_it.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_ja.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ja.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ja.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_ko.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ko.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ko.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_lv.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_lv.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_lv.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_lt.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_lt.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_lt.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_ms.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ms.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ms.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_no.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_no.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_no.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_pl.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_pl.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_pl.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_pt.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_pt.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_pt.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_pt_BR.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_pt_BR.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_pt_BR.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_zh.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_zh.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_zh.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_ro.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ro.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ro.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_ru.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ru.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ru.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_sr.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_sr.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_sr.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_sk.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_sk.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_sk.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_sl.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_sl.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_sl.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_es.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_es.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_es.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_es_419.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_es_419.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_es_419.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_sv.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_sv.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_sv.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_tl.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_tl.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_tl.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_zh_TW.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_zh_TW.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_zh_TW.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_th.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_th.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_th.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_tr.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_tr.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_tr.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_uk.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_uk.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_uk.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_ur.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_ur.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_ur.qm
	@echo /epoc32/data/z/resource/qt/translations/usbdialogs_vi.qm
	@echo /epoc32/release/winscw/udeb/z/resource/qt/translations/usbdialogs_vi.qm
	@echo /epoc32/release/winscw/urel/z/resource/qt/translations/usbdialogs_vi.qm

extension_clean :

deployment_clean :

winscw_deployment_clean:

pre_targetdeps :

create_temps :

deployment : 

finalize : 

FINAL: finalize deployment

