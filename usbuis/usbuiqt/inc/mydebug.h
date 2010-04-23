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


#ifndef MYDEBUG_H
#define MYDEBUG_H

#include <QtDebug>

#if defined(_DEBUG)
    inline QDebug myDebug()
    {
        return qDebug();
    }
#else
    inline QNoDebug myDebug()
    {
        return QNoDebug();
    }
#endif


#endif // MYDEBUG_H
