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

#ifndef USBMAINVIEW_H
#define USBMAINVIEW_H

#include <hbview.h>
#include <QObject>

class UsbUiSettingModel;
class QGraphicsLinearLayout;
class HbListWidgetItem;
class HbListWidget;
class HbTextEdit;
class HbRadioButtonList;

class UsbMainView : public HbView
{
    Q_OBJECT
    friend class TestUsbUiModelActive;

public:
    explicit UsbMainView( QObject *parent = 0 );
    virtual ~UsbMainView();
    
public slots: 
    /**
     * Update view item(s) between the selected rows.
     * @param topLeft The top row index
     * @param bottomRight The bottom row index
     */
    void updateSettingItems(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    
    /*
     * Set the personality user has selected in the radio button list
     * @param personalityIndex The selection index in the radio button list
     */
    void setPersonality( int personalityIndex );

signals:
    
private:
    /*
     * creates the main view window row by row
     */
    void createMainView();
    /*
     * creates the first row in the view containing an icon and a mode name
     * @param  localPropertiesGroup main view layout
     */
    void createIconNameRow(QGraphicsLinearLayout *localPropertiesGroup);
    /*
     * creates the second row from the view containing the description of the selected mode
     * @param  localPropertiesGroup main view layout
     */
    void createDescriptionArea(QGraphicsLinearLayout *localPropertiesGroup);
    /*
     * creates the third row from the view containing the radio button list of the existing usb modes
     * @param  localPropertiesGroup main view layout
     */
    void createRadioButtonArea(QGraphicsLinearLayout *localPropertiesGroup);
       
private:
    // main view layout owned by this class
    QGraphicsLinearLayout *mMainLayout; 
    // setting model not owned by the view,
    // has to be deleted
    UsbUiSettingModel *mModel;
    //owned by the view
    HbListWidgetItem *mListItem;
    // owned by the view
    HbListWidget *mlist;
    // owned by the view
    HbTextEdit *mlabel;
    // owned by the view
    HbRadioButtonList *mradio;
    // not owned by the view
    // need to be deleted
    HbIcon *mIcon ;
};

#endif /* USBMAINVIEW_H */
