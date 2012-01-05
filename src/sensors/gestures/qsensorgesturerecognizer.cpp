/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qsensorgesturerecognizer.h"
#include "qsensorgesture_p.h"
#include "qsensorgesturemanager.h"

QT_BEGIN_NAMESPACE

/*!
  \class QSensorGestureRecognizer
  \ingroup sensorgestures_recognizer
  \inmodule QtSensors

  \brief The QSensorGestureRecognizer class is the base class for a sensor gesture
 recognizer.

    QSensorGesture recognizer developers should sub-class this to implement their own recognizer.

    All sensor gesture recognizers have a detected(QString) signal. Implementors can use this
    and send recognizer specific gestures, such as detected("shake_left") or implement custom signals
    such as shakeLeft().

    These custom signals will be available in the QSensorGesture object at runtime.

    \sa QSensorGestureRecognizer::gestureSignals()

  */

/*!
  \fn void QSensorGestureRecognizer::create()

  Called by QSensorGesture object constructor to create the recognizers backend.

  Implementors would use this to instantiate QSensors and connect signals.

  */

/*!
  \fn QString QSensorGestureRecognizer::id() const
    Returns the identifier for this recognizer.
  */
/*!
  \fn  bool QSensorGestureRecognizer::start()

    Called by QSensorGesture::startDetection() to start this recognizer.
    Implementors should start the sensors.
    Returns true if the operation is successful.

  */
/*!
  \fn bool QSensorGestureRecognizer::stop()

    Called by QSensorGesture::stopDetection() to stop this recognizer.
    Returns true if the call succeeds, otherwise false.

    Implementors should stop the sensors.

  */
/*!
  \fn  bool QSensorGestureRecognizer::isActive()

  Returns true if this recognizer is active, otherwise false.
  */

/*!
  \fn QSensorGestureRecognizer::detected(const QString &gestureId)
  Signals when the \a gestureId gesture has been recognized.
  */

class QSensorGestureRecognizerPrivate
{
public:
    bool initialized;
    int count;
};


/*!
  Constructs the QSensorGestureRecognizer with \a parent as parent.
  */
QSensorGestureRecognizer::QSensorGestureRecognizer(QObject *parent)
    :QObject(parent),
      d_ptr(new QSensorGestureRecognizerPrivate())
{
}

/*!
    Destroy the QSensorGestureRecognizer
*/
QSensorGestureRecognizer::~QSensorGestureRecognizer()
{
   delete d_ptr;
}

/*!
  Returns a list of signals that this recognizer supports.

  Note that all signals declared will be exported to the QSensorGesture
  object. If you need to use signals that are not exported, you should use a private class
  to do so.

  */
QStringList QSensorGestureRecognizer::gestureSignals() const
{
    QStringList list;
    bool ok = false;
    for (int i = 0; i < this->metaObject()->methodCount(); i++) {
        //weed out objectsignals and slots
        if (this->metaObject()->indexOfSignal(this->metaObject()->method(i).signature()) != -1) {
            QString sig( QLatin1String(this->metaObject()->method(i).signature()));
            if (sig.contains(QLatin1String("detected")))
                ok = true;
            if (ok)
                list.append(sig);
        }
    }
    return list;
}

/*!
    Calls QSensorGestureRecognizer::create() if the recognizer is valid.
*/
void QSensorGestureRecognizer::createBackend()
{
    if (d_ptr->initialized) {
        return;
    }
    d_ptr->initialized = true;
    create();
}

/*!
  Calls QSensorGestureRecognizer::start() if the recognizer isn't already running.
*/
void QSensorGestureRecognizer::startBackend()
{
    if (d_ptr->count++ == 0)
        start();
}

/*!
  Calls QSensorGestureRecognizer::stop() if no other clients are using it.
*/
void QSensorGestureRecognizer::stopBackend()
{
    if (--d_ptr->count == 0)
        stop();
}

QT_END_NAMESPACE
