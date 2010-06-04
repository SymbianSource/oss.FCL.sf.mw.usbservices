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

#ifndef USBUISETTINGMODEL_H
#define USBUISETTINGMODEL_H


#include <e32base.h>
#include <e32property.h>
#include <QStringList>
#include <QAbstractItemModel>
#include <XQSettingsManager>

const int KDefaultColumn = 0;
typedef QList< QMap< int, QVariant > > UsbUiModelDataSource;

class RUsb;
class UsbUiModelActive;

class UsbUiSettingModel : public QAbstractItemModel
{
    Q_OBJECT
    friend class TestUsbUiModelActive;
 
public:
enum usbSettingsIndex {
        DefaultMode,
        Description,
        UsbModeList,
        EndOfSettings
    };
enum SettingsRoleValues {
        SettingType = Qt::UserRole,
        SettingsModeNameList,
    };
public:
    UsbUiSettingModel( QObject *parent = 0 );
    virtual ~UsbUiSettingModel();

    /*
     *  Returns the index of the item in the model specified by the given row, column and parent index.
     *  @param row is the row number of the specified item
     *  @param column is the column number of the specified item
     *  @param parent QModelIndex parent
     */   
    virtual QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    /*
     * Returns QModelIndex() because this model item has no parent
     * @param child is the index of item model, the parent of which is going to be returned
     * @param child is not used in here
     */
    virtual QModelIndex parent( const QModelIndex &child ) const;
    /*
     * Returns the number of rows under the given parent
     * @param parent QModelIndex parent
     */
    virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    /*
     * Returns the number of columns for the children of the given parent.
     * @param parent QModelIndex parent
     */
    virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    /*
     *Returns the data stored under the given role for the item referred to by the index. 
     */
    virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    /*
     * Sets the role data for the item at index to value. Returns true if successful; otherwise returns false.
     */
    virtual bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );

    const QModelIndex* sourceData() const;

public slots:
    /*
     * Provides notification of changes in selected usb mode
     * @param key is not used 
     * @param value is the value read from cenrep key 
     */
    void usbModeChanged( const XQSettingsKey &key, const QVariant &value );
    /*
     * it checks the response from usbwatcher to see if the new mode change has been successful
     * it will go back to the previous personality if it has not been successfull
     * @param status is the error code returned from usbwatcher
     */
    void setPersonalityCompleted (int status );
    
private:
	/*
	 * creats the logical name of the modename string for the list
	 * @param modeId is the usb personality id
	 */   
    QString modeListName( int modeId );
    /*
     * Get the current USB personality ID
     * Returns 0, if the key is missing or the value cannot be converted 
     * to integer.
     * @return The current personality id
     */
    int currentMode(); 
    /*
     * sets the value to different roles of the model's first row
     */
    void setDefaultModeSetting();
    /*
     * sets the value to different roles of the model's description row
     */
    void setDescriptionSetting();
    /**
     * Set the USB mode list and the current selection in the list.
     * The personality IDs are stored to mPersonalityIds and the personality names
     * to mSettingsList.
     * @param modeId The current mode ID
     */
    bool setUsbModelistSetting( int modeId );
    /**
     * calls the model's set functions
     * @param modeId The current mode ID
     */
    void setUsbSettings( int modeId );
    /**
     * updates the model rows and emits signal datachanged
     * @param newPersonality is the new personality id
     */
    void setNewUsbMode(int newPersonality);
	
    /**
     * Check from the USB Manager if the USB personality is hidden.
     * @param usbman Already opened USB Manager session
     * @param personalityId The ID of the personality to be checked.
     * @return For hidden personality, true is returned.
     */
    bool isPersonalityHidden(RUsb &usbman, TInt personalityId);
    
    /**
     * Check if the USB personality is hidden.
     * The personality is hidden, if it does not exist in mPersonalityIds.
     * @param personalityId The ID of the personality to be checked.
     * @return For hidden personality, true is returned.
     */
    bool isPersonalityHidden(TInt personalityId);
 private:
    
    UsbUiModelDataSource mSettingsList;
    int mCurrentMode;
    // stored, so that we can show the correct personality when it fails to set the new one
    int mPreviousMode; 
    
    XQSettingsManager mSettingsManager;
    QList<int> mPersonalityIds;
    UsbUiModelActive *mModelActive;
};

#endif // USBUISETTINGMODEL_H
