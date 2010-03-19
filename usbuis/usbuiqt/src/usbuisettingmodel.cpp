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

#include "usbuisettingmodel.h"
#include "usbuimodelactive.h"
#include "mydebug.h"
#include <QStringList>
#include <UsbWatcherInternalCRKeys.h>
#include <XQSettingsKey>
#include <usbman.h>

const QString TextIdPrefix = ("txt_usb_");
const QString ModeIconNamePrefix = (":/icons/usb_icon_mode_");

/*!
    Constructor.
 */
UsbUiSettingModel::UsbUiSettingModel( QObject *parent )
    : QAbstractItemModel( parent )
{
    mModelActive = new UsbUiModelActive();
    
    for ( int i = 0; i < UsbUiSettingModel::EndOfSettings; i++ ) {
        // Initialize the list with empty values.
        mSettingsList.append( QMap< int, QVariant >() );
    }
    
    mCurrentMode = currentMode();
    mPreviousMode = mCurrentMode;
    setUsbSettings(mCurrentMode);
  	bool ret = mSettingsManager.startMonitoring( 
   	        XQSettingsKey( XQSettingsKey::TargetCentralRepository, 
   	                KCRUidUsbWatcher.iUid, KUsbWatcherPersonality ) );
    myDebug() << ">>> UsbUiSettingModel::startMonitoring value=" 
              << ret; 
                              
   	// signal: personality changed in the central repository                
   	connect( &mSettingsManager, 
   	        SIGNAL( valueChanged( const XQSettingsKey&, const QVariant& ) ), 
   	        this, 
   	        SLOT( usbModeChanged( const XQSettingsKey&, const QVariant& ) ) );
   	        
   // signal: response from usbwatcher to our attempt to set the personality 	        
   	connect( mModelActive, 
   	        SIGNAL( requestCompleted( int status ) ), 
   	        this, 
   	        SLOT( setPersonalityCompleted (int status )));

}
/*!
 * calls the model's set functions to initialize the model's data
 */
void UsbUiSettingModel::setUsbSettings( int aModeId )
    {
    setUsbModelistSetting( aModeId ); 
    setDefaultModeSetting();
    setDescriptionSetting();
    }

/*!
    Destructor.
 */
UsbUiSettingModel::~UsbUiSettingModel()
{
    myDebug() << ">>> UsbUiSettingModel::~UsbUiSettingModel";
    mSettingsManager.stopMonitoring( 
            XQSettingsKey( XQSettingsKey::TargetCentralRepository, 
                    KCRUidUsbWatcher.iUid, KUsbWatcherPersonality ) );
    delete mModelActive;
    myDebug() << "<<< UsbUiSettingModel::~UsbUiSettingModel";
}

/*!
    Provides notification of changes in selected usb mode
 */
void UsbUiSettingModel::usbModeChanged( const XQSettingsKey &key,  
        const QVariant &value )  
{
Q_UNUSED(key);
    myDebug() << ">>> UsbUiSettingModel::usbModeChanged";
    // key is not checked, as we monitor only one key
 
    setNewUsbMode(value.toInt());
                                  
    myDebug() << "<<< UsbUiSettingModel::usbModeChanged"; 
}
/*!
 * updates the model rows and emits signal datachanged
 */
void UsbUiSettingModel::setNewUsbMode(int newPersonality)
    
    {
    myDebug() << ">>> UsbUiSettingModel::setNewUsbMode value=" 
            << newPersonality; 
            
    mCurrentMode = newPersonality;
    setUsbSettings(mCurrentMode);
   	
    emit dataChanged( createIndex( DefaultMode, KDefaultColumn ), 
            createIndex( UsbModeList, KDefaultColumn ) );   
         
    myDebug() << "<<< UsbUiSettingModel::setNewUsbMode"; 
    }
/*!
 * Returns the index of the item in the model specified by the given row, column and parent index.
 */
QModelIndex UsbUiSettingModel::index( int row, int column, const QModelIndex &parent ) const
{
    return hasIndex( row, column, parent ) ? createIndex( row, column ) : QModelIndex();
}


/*
    This model does not support hierarchy, so this returns an empty model index. 
 */
QModelIndex UsbUiSettingModel::parent( const QModelIndex &child ) const
{
    Q_UNUSED( child );
    return QModelIndex();
}
/*!
 * Returns the number of rows under the given parent
 */
int UsbUiSettingModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return mSettingsList.count();
}


/*!
 * Returns the number of columns for the children of the given parent.
 * This model is one-dimensional, so this returns 1.
 */
int UsbUiSettingModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return 1;
}
/*!
 * Returns the data stored under the given role for the item referred to by the index.
 */
QVariant UsbUiSettingModel::data( const QModelIndex &index, int role ) const
{
    return mSettingsList.value( index.row() ).value( role );
}


/*!
 * This is called when usb mode is changed, it only sets the personality when the given role is EditRole 
 */
bool UsbUiSettingModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    myDebug() << ">>>  UsbUiSettingModel::setData";
    bool success = false;
    if ( role == Qt::EditRole ) {
        // Only allow changes on the value with role EditRole. Others are managed here.
        switch ( index.row() ) {
        case UsbModeList: {            
            mModelActive->SetUsbPersonality( mPersonalityIds.at( value.toInt() ) );
            success = true;
            // also update the rest of the model with the about to be selected mode right away
            setNewUsbMode(mPersonalityIds.at( value.toInt() ));
            break;
            }
        case DefaultMode:
            //no break
        case Description:
            //no break
        default:
            myDebug() << "    UsbUiSettingModel::setData not supported row " << index.row();
            break;
        } 
    }   
    myDebug() << "<<<  UsbUiSettingModel::setData return " << success;
    return success;
}

/*!
 * Getter for the source data.
 */
const QModelIndex* UsbUiSettingModel::sourceData() const
{
    return new QModelIndex( createIndex( 0, 0 ) );
}

/*!
 * creats the logical name of the modename string for the list
 */
QString UsbUiSettingModel::modeListName( int modeId )
{
    QString textId = TextIdPrefix + "modelistname_" + QString::number( modeId );
    QString modeName = qtTrId( textId.toAscii() );
    return modeName;
}

/*!
 * Get the current USB mode (personality) ID
 */
int UsbUiSettingModel::currentMode()
{
    myDebug() << ">>>  UsbUiSettingModel::CurrentMode";
    int currentMode = mSettingsManager.readItemValue(
                XQSettingsKey( XQSettingsKey::TargetCentralRepository, 
                KCRUidUsbWatcher.iUid, KUsbWatcherPersonality ) ).toInt();
    myDebug() << "<<< UsbUiSettingModel::CurrentMode " << currentMode;
    return currentMode;
}

/*!
 * sets the value to different roles of the model's first row
 */
void UsbUiSettingModel::setDefaultModeSetting()
{    
    QString iconName;
    QString modeName;
    //empty iconName and modeName are used for a hidden personality
    if ( !isPersonalityHidden(mCurrentMode) ) {
        iconName = ModeIconNamePrefix + QString::number( mCurrentMode ) + ".svg";
        QString textId = TextIdPrefix + "modename_" + QString::number( mCurrentMode );
        modeName = qtTrId( textId.toAscii() );
    }

    myDebug() << ">>> UsbUiSettingModel::setDefaultModeSetting iconName=" 
            << iconName << " modeName=" << modeName;
    QMap< int, QVariant > val = mSettingsList.at( DefaultMode );
    if ( val.isEmpty() ) {
        val[ SettingType ] = QVariant( DefaultMode );
    }    
    val[ Qt::DecorationRole ] = QVariant(iconName) ;
    val[ Qt::DisplayRole ]= QVariant(modeName) ;
    mSettingsList.replace( DefaultMode, val );
    myDebug() << "<<< UsbUiSettingModel::setDefaultModeSetting";
}

/*!
    Updates all values related to the mode description.
 */
void UsbUiSettingModel::setDescriptionSetting()
{
    QString description;
    //the description will be empty for a hidden personality
    if ( !isPersonalityHidden(mCurrentMode) ) {
        QString textId = TextIdPrefix + "description_" 
                + QString::number( mCurrentMode );
        description = qtTrId( textId.toAscii() );
    }
    myDebug() << ">>> UsbUiSettingModel::setDescriptionSetting description=" 
            << description;
    QMap< int, QVariant > val = mSettingsList.at( Description );
    if ( val.isEmpty() ) {
       
        val[ SettingType ] = QVariant( Description );
     
    }
    // The display role stores the string representation of the actual value.
    val[ Qt::DisplayRole ] = QVariant( description );
    	 mSettingsList.replace( Description, val );
    myDebug() << "<<< UsbUiSettingModel::setDescriptionSetting";     
}

/*!
    Updates all values related to the visibility setting.
    Updates the selectable USB modes only in the 1st call.
 */
bool UsbUiSettingModel::setUsbModelistSetting( int aModeId )
{
    myDebug() << ">>> UsbUiSettingModel::setUsbModelistSetting aModeIndex="
            << aModeId;
   bool ret = true;
    QMap< int, QVariant > val = mSettingsList.at(UsbModeList);
    if ( val.isEmpty() ) {
        val[ SettingType ] = QVariant( UsbModeList );

        RUsb iUsbman;
        if ( iUsbman.Connect() == KErrNone ) {
            
            RArray<TInt> personalityIds;
            mPersonalityIds.clear();
            if ( iUsbman.GetPersonalityIds( personalityIds ) == KErrNone ) {
                QStringList modeList;
                for ( int i = 0; i < personalityIds.Count(); i++ ) {
                    if ( !isPersonalityHidden(iUsbman, personalityIds[i]) ) {
                        mPersonalityIds.append( personalityIds[i] );
                        modeList.append( modeListName( mPersonalityIds[i] ) );                    
                    }
                }
                val[ SettingsModeNameList ] = QVariant( modeList ); 
            }
        }
        else{
        ret = false;
        }
        iUsbman.Close();
    }
    //index will be -1 for hidden personality
    val[ Qt::EditRole ] = QVariant( mPersonalityIds.indexOf(aModeId) );
    mSettingsList.replace( UsbModeList, val );
    myDebug() << "<<< UsbUiSettingModel::setUsbModelistSetting";
    return ret;
}

/*!
 * it checks the response from usbwatcher to see if the new mode change has been successful
 * it will go back to the previous personality if it has not been successfull
 */
void UsbUiSettingModel::setPersonalityCompleted (int status )
{
  myDebug() << ">>> UsbUiSettingModel::setPersonalityCompleted status= "
    << status;    
  // status contains Symbian error code from usbwatcher
  // if the status is KErrNone, we are ready to process the next request
  if (status != KErrNone) 
    {
    // changing the personality failed, so we need to set back the previous personality
    // the value will be read from central repository and also updates mCurrentMode  
    mPreviousMode = mSettingsManager.readItemValue(
                   XQSettingsKey( XQSettingsKey::TargetCentralRepository, 
                   KCRUidUsbWatcher.iUid, KUsbWatcherPersonality ) ).toInt();
    setNewUsbMode(mPreviousMode);
    }
   
   // after handling the return code we know the the current personality works
   // and we will not go back to the previous one 
   mPreviousMode = mCurrentMode; 
   myDebug() << "<<< UsbUiSettingModel::setPersonalityCompleted";      
}

bool UsbUiSettingModel::isPersonalityHidden(RUsb &usbman, TInt personalityId)
{
    myDebug() << ">>> UsbUiSettingModel::isPersonalityHidden from USB Manager";
    bool hidden = false;
    TUint32 property = 0;
    TInt ret = usbman.GetPersonalityProperty(personalityId, property);
    if (ret == KErrNone) {
        myDebug() << "property " << property;
        if (property & KUsbPersonalityPropertyHidden) {
            hidden = true;
        }
    } 
    myDebug() << "<<< UsbUiSettingModel::isPersonalityHidden " << hidden;
    return hidden;
}

bool UsbUiSettingModel::isPersonalityHidden(TInt personalityId)
{
    return ( mPersonalityIds.indexOf(personalityId) == -1 );
}
