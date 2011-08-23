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

#include "test_sensor_wrapper.h"
#include <private/qsensormanager_p.h>
#include <QDebug>

QTM_BEGIN_NAMESPACE

class WrapperFactory : public QSensorPluginInterface,
                       public QSensorBackendFactory
{
public:
    void registerSensors()
    {
        QList<QByteArray> types;
        types << TestSensor::type;
        foreach (const QByteArray &type, types) {
            foreach (const QByteArray &identifier, NEW_NAMESPACE(QSensor)::sensorsForType(type)) {
                //qDebug() << "Registerbackend" << type << identifier;
                QSensorManager::registerBackend(type, identifier, this);
            }
        }
    }

    QSensorBackend *createBackend(QSensor *sensor)
    {
        if (sensor->identifier() == "test sensor impl") {
            return new TestSensorWrapper(sensor);
        }

        qWarning() << "Can't create backend" << sensor->identifier();
        return 0;
    }
};

REGISTER_STATIC_PLUGIN_V1(WrapperFactory)

IMPLEMENT_WRAPPER(TestSensor, TestSensorReading, {
                  //qDebug() << "fetchData" << sensor->reading()->timestamp();
                  m_reading.setTimestamp(sensor->reading()->timestamp());
                  m_reading.setTest(sensor->reading()->test());
                  newReadingAvailable();
                  })

QTM_END_NAMESPACE
