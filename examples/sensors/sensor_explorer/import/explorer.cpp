/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "explorer.h"
#include <QtCore/QDebug>
#include <qsensor.h>
#include <QMetaObject>
#include <QMetaProperty>

QT_BEGIN_NAMESPACE

/*!
    \class QSensorExplorer
    \brief The QSensorExplorer element provides an easy access for reading all avalaible sensors installed on the system.
*/

/*!
    Construct a QSensorExplorer object with parent \a parent
*/
QSensorExplorer::QSensorExplorer(QObject* parent)
    : QObject(parent)
    , _selectedSensorItem(0)
{
    loadSensors();
}

/*!
    Destructor of a QSensorExplorer
*/
QSensorExplorer::~QSensorExplorer()
{
}

/*!
    Load all available sensors and store it in a list.
*/
void QSensorExplorer::loadSensors()
{
    //! [0]
    _availableSensors.clear();

    foreach (const QByteArray &type, QSensor::sensorTypes()) {
        qDebug() << "Found type" << type;
        foreach (const QByteArray &identifier, QSensor::sensorsForType(type)) {
            qDebug() << "Found identifier" << identifier;
            // Don't put in sensors we can't connect to
            QSensor* sensor = new QSensor(type, this);
            sensor->setIdentifier(identifier);
            if (!sensor->connectToBackend()) {
                qDebug() << "Couldn't connect to" << identifier;
                continue;
            }

            qDebug() << "Adding identifier" << identifier;
            _availableSensors.append(new QSensorItem(sensor, this));
        }
    }
    emit availableSensorsChanged();
    //! [0]
}

/*!
    \fn QSensorExplorer::availableSensorsChanged()
    Notifies the client if the list of the available sensors was changed
*/

/*!
    \property QSensorExplorer::availableSensors
    Returns a list of all available sensor.
*/
QDeclarativeListProperty<QSensorItem> QSensorExplorer::availableSensors()
{
    return QDeclarativeListProperty<QSensorItem>(this,_availableSensors);
}

/*!
    \fn QSensorExplorer::selectedSensorItemChanged()
    Notifies the client if the selected sensor has been changed
*/
/*!
    \property QSensorExplorer::selectedSensorItem
    Returns the current selected sensor item.
*/
QSensorItem* QSensorExplorer::selectedSensorItem()
{
    return _selectedSensorItem;
}

/*!
    \fn QSensorExplorer::setSelectedSensorItem(QSensorItem* selitem)
    Sets the QSensorItem \a selitem as the current selected QSensorItem.
*/
void QSensorExplorer::setSelectedSensorItem(QSensorItem* selitem)
{
    if (selitem  && _selectedSensorItem != selitem) {
        if (_selectedSensorItem)
            _selectedSensorItem->unSelect();
        _selectedSensorItem = selitem;
        _selectedSensorItem->select();
        emit selectedSensorItemChanged();
    }
}

QT_END_NAMESPACE