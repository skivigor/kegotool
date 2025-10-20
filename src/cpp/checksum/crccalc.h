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
#ifndef CRCCALC_H
#define CRCCALC_H

#include <QObject>
#include <QMap>
#include <QJsonArray>

#include "crcengine.h"

namespace checksum
{

class CrcCalc : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool       processing READ isProcessing NOTIFY processingChanged)
    Q_PROPERTY(QJsonArray crcTypes   READ getCrcTypes  NOTIFY crcTypesChanged)
    Q_PROPERTY(quint64    result     READ getResult    NOTIFY resultChanged)
    Q_PROPERTY(QString    hexResult  READ getHexResult NOTIFY hexResultChanged)
    Q_PROPERTY(int        mode       READ getCrcMode   NOTIFY crcModeChanged)

public:
    explicit CrcCalc(QObject *parent = nullptr);
    ~CrcCalc();

    bool       isProcessing() const  { return _processing; }
    quint64    getResult() const     { return _result; }
    QString    getHexResult() const  { return _hexResult; }
    QJsonArray getCrcTypes() const;

    Q_INVOKABLE bool setCrcMode(CrcMode::Mode mode);
    Q_INVOKABLE bool setCrcMode(unsigned int mode);
    Q_INVOKABLE CrcMode::Mode getCrcMode() const  { return _mode; }

public slots:
    bool calculate(unsigned int typeIdx, const QByteArray &data);
    bool calculate(unsigned int typeIdx, const QString &path);
    bool calculate(const QJsonObject &crcCfg, const QByteArray &data);
    bool calculate(const QJsonObject &crcCfg, const QString &path);

signals:
    void processingChanged();
    void crcTypesChanged();
    void resultChanged();
    void hexResultChanged();
    void crcModeChanged();

private:
    void init();
    void resetResult();

private:
    const QString  _algorithmsPath;
    QMap<CrcMode::Mode, QJsonArray>  _types;

    CrcMode::Mode  _mode = CrcMode::CRC8;
    bool           _processing = false;
    quint64        _result = 0;
    QString        _hexResult = "0x0";

};


}   // namepace checksum


#endif // CRCCALC_H
