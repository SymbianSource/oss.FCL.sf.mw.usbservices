rem
rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
rem All rights reserved.
rem This component and the accompanying materials are made available
rem under the terms of "Eclipse Public License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem Description:
rem

rd /S /Q \epoc32\BUILD
del usbuiapitest.sisx
call bldmake bldfiles
call abld reallyclean armv5
call abld test build armv5
call makesis usbuiapitest.pkg
call signsis usbuiapitest.sis usbuiapitest.sisx x:\rd.cer x:\rd-key.pem

del usbuiapitest.sis

call pause