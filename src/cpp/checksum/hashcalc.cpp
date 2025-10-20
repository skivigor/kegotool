/*
 * KegoTool using Qt and Qml.
 * Copyright (C) 2025  Skiv <skivigor@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "hashcalc.h"

#include <QMetaEnum>
#include <QVariant>

#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

#include <QFile>
#include <QFileInfo>

#include <QDebug>


namespace
{

QString toLocalFile(const QString &path)
{
    return QUrl(path).isLocalFile() ? QUrl(path).toLocalFile() : path;
}

}   // namespace



namespace checksum
{

HashCalc::HashCalc(QObject *parent)
    : QObject{parent}
{
    init();
    qDebug() << "HashCalc::HashCalc: ctor: " << this;
}

HashCalc::~HashCalc()
{
    qDebug() << "HashCalc::~HashCalc: dtor: " << this;
}

//-----------------------------------------------------------------

void HashCalc::resetResult()
{
    if (_processing == true) return;

    _result.clear();
    emit resultChanged();

    _hexResult = "0x0";
    emit hexResultChanged();
}

//-----------------------------------------------------------------

bool HashCalc::calculate(const QString &algorithmName, const QByteArray &data)
{
    QMetaEnum hash = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
    QCryptographicHash::Algorithm alg = static_cast<QCryptographicHash::Algorithm>(hash.keyToValue(algorithmName.toStdString().c_str()));
    if (static_cast<int>(alg) == -1) return false;

    return calculate(alg, data);
}

//-----------------------------------------------------------------

bool HashCalc::calculate(const QString &algorithmName, const QString &path)
{
    QMetaEnum hash = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
    QCryptographicHash::Algorithm alg = static_cast<QCryptographicHash::Algorithm>(hash.keyToValue(algorithmName.toStdString().c_str()));
    if (static_cast<int>(alg) == -1) return false;

    return calculate(alg, path);
}

//-----------------------------------------------------------------

bool HashCalc::calculate(QCryptographicHash::Algorithm algorithm, const QByteArray &data)
{
    if (_processing == true || data.isEmpty()) return false;
    resetResult();

    QFutureWatcher<QByteArray> *watcher = new QFutureWatcher<QByteArray>(this);
    QObject::connect(watcher, &QFutureWatcher<quint64>::finished, this,
                     [watcher, this] ()
                     {
                         watcher->deleteLater();

                         _result = watcher->result();
                         _hexResult = _result.toHex().toUpper();
                         _processing = false;

                         emit resultChanged();
                         emit hexResultChanged();
                         emit processingChanged();
                     });

    _processing = true;
    emit processingChanged();

    watcher->setFuture(QtConcurrent::run([](QCryptographicHash::Algorithm alg, const QByteArray &data) {
        return QCryptographicHash::hash(data, alg);
    }, algorithm, data));

    return true;
}

//-----------------------------------------------------------------

bool HashCalc::calculate(QCryptographicHash::Algorithm algorithm, const QString &path)
{
    if (_processing == true) return false;
    resetResult();

    QString p = toLocalFile(path);
    QFileInfo info(p);
    if (info.isFile() == false || info.isReadable() == false) return false;

    QFutureWatcher<QByteArray> *watcher = new QFutureWatcher<QByteArray>(this);
    QObject::connect(watcher, &QFutureWatcher<quint64>::finished, this,
                     [watcher, this] ()
                     {
                         watcher->deleteLater();

                         _result = watcher->result();
                         _hexResult = _result.toHex().toUpper();
                         _processing = false;

                         emit resultChanged();
                         emit hexResultChanged();
                         emit processingChanged();
                     });

    _processing = true;
    emit processingChanged();

    watcher->setFuture(QtConcurrent::run([](QCryptographicHash::Algorithm alg, const QString &path) {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly) == false) return QByteArray();

        QCryptographicHash hash(alg);
        int blockSize = 1048576;  // 1Mb
        while (file.atEnd() == false) hash.addData(file.read(blockSize));
        file.close();

        return hash.result();
    }, algorithm, p));

    return true;

}

//-----------------------------------------------------------------

void HashCalc::init()
{
    QMetaEnum hashEnum = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
    for (qint32 i = 0, count = hashEnum.keyCount(); i < count; i++) _types.append(hashEnum.key(i));
}

//-----------------------------------------------------------------

}   // namespace checksum

