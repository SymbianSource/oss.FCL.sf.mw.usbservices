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
#include "usbmainview.h"
#include <QModelIndex>
#include <QtGui/QGraphicsLinearLayout>
#include <hblistwidget.h>
#include <hblistwidgetitem.h>
#include <hbradiobuttonlist.h>
#include <hbtextedit.h>

#include "usbuisettingmodel.h"
#include "mydebug.h"
/*!
 * default constructor
 */
UsbMainView::UsbMainView( QObject *parent )
:   HbView(), mModel(NULL)
{
    myDebug() << ">>> UsbMainView::UsbMainView";
    setParent( parent );    
    setTitle( qtTrId("USB") ); 
    mMainLayout = new QGraphicsLinearLayout( Qt::Vertical, this );
    mModel = new UsbUiSettingModel();
    createMainView();
    setLayout( mMainLayout );
    myDebug() << "<<< UsbMainView::UsbMainView";      
}

/*!
 * creates the main view
 */
void UsbMainView::createMainView()
{
    myDebug() << ">>> UsbMainView::createMainView";
    QGraphicsLinearLayout *mainViewLayout = new QGraphicsLinearLayout( Qt::Vertical, mMainLayout );
    mListItem = NULL;
    mlist = NULL;
    mlabel = NULL;
    mradio = NULL;
    mIcon = NULL;
    createIconNameRow( mainViewLayout );
    createDescriptionArea( mainViewLayout );
    createRadioButtonArea( mainViewLayout );
    mMainLayout->addItem( mainViewLayout );
    bool connected = connect( mModel, SIGNAL( dataChanged( QModelIndex, QModelIndex ) ), 
            this, SLOT( updateSettingItems( QModelIndex, QModelIndex ) ) );
    myDebug() << "<<< UsbMainView::createMainView model connected=" << connected;
    connected = connect(mradio, SIGNAL( itemSelected( int ) ), 
            this, SLOT( setPersonality( int ) ) );
    myDebug() << "<<< UsbMainView::createMainView mradio connected=" << connected;
    myDebug() << "<<< UsbMainView::createMainView";
}  
/*!
 * creating the creates the first row in the view containing an icon and a mode name 
 */
void UsbMainView::createIconNameRow( QGraphicsLinearLayout *aLocalPropertiesGroup )
{
    myDebug() << ">>> UsbMainView::createIconNameRow";
    QGraphicsLinearLayout *listLayout = new QGraphicsLinearLayout( Qt::Vertical, mMainLayout );
    mlist = new HbListWidget( this );
    mlist->setMaximumSize( 300, 50 ); //todo
    mListItem= new HbListWidgetItem();
    QModelIndex index = mModel->index( UsbUiSettingModel::DefaultMode, KDefaultColumn );
  //  QVariant iconName = mModel->data( index, Qt::DecorationRole );
    mIcon = new HbIcon( mModel->data( index, Qt::DecorationRole ).toString() );
    mListItem->setDecorationIcon( *mIcon );
    mListItem->setText( mModel->data( index,Qt::DisplayRole ).toString() );
    mListItem->setText( mModel->data( index,Qt::DisplayRole ).toString() );        
    mlist->insertItem( 1, mListItem ); 
    listLayout->addItem( mlist );
    aLocalPropertiesGroup->addItem( listLayout );
    myDebug() << "<<< UsbMainView::createIconNameRow";      
}
/*!
 * creates the second row from the view containing the description of the selected mode
 */
void UsbMainView::createDescriptionArea( QGraphicsLinearLayout *aLocalPropertiesGroup )
{ 
    myDebug() << ">>> UsbMainView::createDescriptionArea";
    QGraphicsLinearLayout *desLayout = new QGraphicsLinearLayout( Qt::Vertical, mMainLayout ); 
    QModelIndex index = mModel->index( UsbUiSettingModel::Description, KDefaultColumn );
    mlabel = new HbTextEdit( mModel->data( index, Qt::DisplayRole ).toString() );
    mlabel->setReadOnly( true );
    mlabel->setCursorVisibility( Hb::TextCursorHidden );
    desLayout->addItem( mlabel );
    desLayout->setAlignment( mlabel, Qt::AlignTop );
    aLocalPropertiesGroup->addItem( desLayout );
    myDebug() << "<<< UsbMainView::createDescriptionArea";      
}

/*!
 * creates the third row from the view containing the radio button list of the existing usb modes
 */
void UsbMainView::createRadioButtonArea(QGraphicsLinearLayout *aLocalPropertiesGroup)
{
    myDebug() << ">>> UsbMainView::createRadioButtonArea";
    QGraphicsLinearLayout *radioLayout = new QGraphicsLinearLayout( Qt::Vertical, mMainLayout ); 
    QModelIndex index = mModel->index( UsbUiSettingModel::UsbModeList, KDefaultColumn );
    QStringList radioList = mModel->data( index, UsbUiSettingModel::SettingsModeNameList ).toStringList();
    int selectedIndex = mModel->data( index, Qt::EditRole ).toInt();
    mradio = new HbRadioButtonList( radioList, selectedIndex );
    radioLayout->addItem( mradio );
    radioLayout->setAlignment( mradio, Qt::AlignBottom );
    aLocalPropertiesGroup->addItem(radioLayout); 
    myDebug() << "<<< UsbMainView::createRadioButtonArea";      
}

/*!
    Slot for receiving notification of data changes from the model.
    Identify the setting changed and update the corresponding UI item.
 */
void UsbMainView::updateSettingItems(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{   
    myDebug() << ">>> UsbMainView::updateSettingItems";
    // update only the part of the view specified by the model row(s)
    for ( int row = topLeft.row(); row <= bottomRight.row(); row++ ) {
        myDebug() << "    UsbMainView::updateSettingItems row=" << row;
        QModelIndex index = mModel->index( row, KDefaultColumn );
        // Distinguish which setting value is changed.
        switch ( row ) {
        case UsbUiSettingModel::DefaultMode :
            // set the selected personality icon and name
            mIcon->setIconName( mModel->data( index, Qt::DecorationRole ).toString() );
            mListItem->setDecorationIcon( *mIcon );
            mListItem->setText( mModel->data( index,Qt::DisplayRole ).toString() );            
            break;
        case UsbUiSettingModel::Description :
            // set the description of the selected personality.
            mlabel->setPlainText( mModel->data( index, Qt::DisplayRole ).toString() );
            mlabel->setReadOnly( true );
            mlabel->setCursorVisibility( Hb::TextCursorHidden );            
            break;
        case UsbUiSettingModel::UsbModeList :
            // only the selection can change, no changes in the list of personalities
            mradio->setSelected( mModel->data( index, Qt::EditRole ).toInt() );
            break;
        default :
            myDebug() << "    UsbMainView::updateSettingItems unknown row " << row;
            break;
        }
    }
    myDebug() << "<<< UsbMainView::updateSettingItems";
}

/*
 * Slot for receiving notification from the change in radio button list
 */
void UsbMainView::setPersonality( int personalityIndex )
{
    myDebug() << ">>> UsbMainView::setPersonality";
    QModelIndex index = mModel->index( UsbUiSettingModel::UsbModeList, KDefaultColumn );
    mModel->setData( index, personalityIndex, Qt::EditRole );
    myDebug() << "<<< UsbMainView::setPersonality";
}

/*!
 * Destructor
 * destructs the items which are not owned by this class
 */
UsbMainView::~UsbMainView()
{ 
    myDebug() << ">>> UsbMainView::~UsbMainView";
    delete mModel;
    delete mIcon;
    myDebug() << "<<< UsbMainView::~UsbMainView";   
}
