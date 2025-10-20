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
#include "serialclient.h"
//#include "bytetablemodel.h"
#include "util/unsortedmap.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QJsonObject>

#include <QMetaEnum>
//#include <QVariant>

#include <QDebug>

namespace
{

util::UnsortedMap<QString, QSerialPort::DataBits>    MapDataBits;
util::UnsortedMap<QString, QSerialPort::Parity>      MapParity;
util::UnsortedMap<QString, QSerialPort::StopBits>    MapStopBits;
util::UnsortedMap<QString, QSerialPort::FlowControl> MapCtrl;

}   // namespace


namespace client
{

SerialClient::SerialClient(QObject *parent)
    : QObject{parent},
    _pSerial(new QSerialPort(this))
{
    init();

    qDebug() << "SerialClient::SerialClient: after init: "
             << getDataBitsList()
             << getParityList()
             << getStopBitsList()
             << getCtrlList();

    QObject::connect(_pSerial, &QSerialPort::readyRead, this, &SerialClient::onReadyRead);
    QObject::connect(_pSerial, &QSerialPort::errorOccurred,
                     this, [&](QSerialPort::SerialPortError error) {
                         _status = QMetaEnum::fromType<QSerialPort::SerialPortError>().valueToKey(error);
                         emit statusChanged();
                     });

}

SerialClient::~SerialClient()
{
    if (_pSerial->isOpen()) _pSerial->close();
    qDebug() << "SerialClient::~SerialClient: dtor";
}

//-----------------------------------------------------------------

bool SerialClient::open(const QString &port, int baud, const QString &dataBits, const QString &parity,
                        const QString &stopBits, const QString &ctrl)
{
    if (_pSerial->isOpen() || !MapDataBits.contains(dataBits)
        || !MapParity.contains(parity) || !MapStopBits.contains(stopBits)
        || !MapCtrl.contains(ctrl))
    {
        return false;
    }

    _pSerial->setPortName(port);
    _pSerial->setBaudRate(baud);
    _pSerial->setDataBits(MapDataBits.value(dataBits));
    _pSerial->setParity(MapParity.value(parity));
    _pSerial->setStopBits(MapStopBits.value(stopBits));
    _pSerial->setFlowControl(MapCtrl.value(ctrl));

    if (_pSerial->open(QIODevice::ReadWrite) == false) return false;

    _pSerial->clear();
    _openned = true;
    emit opennedChanged();

    return true;
}

//-----------------------------------------------------------------

void SerialClient::close()
{
    _pSerial->close();

    _openned = false;
    emit opennedChanged();
}

//-----------------------------------------------------------------

void SerialClient::updatePortList()
{
    _portInfoList = QJsonArray();

    const auto infos = QSerialPortInfo::availablePorts();
    for (const auto &info : infos)
    {
        QJsonObject obj {
            { "name", info.portName() },
            { "manufcturer", info.manufacturer() },
            { "pid", info.productIdentifier() },
            { "vid", info.vendorIdentifier() }
        };
        _portInfoList.append(obj);
    }

    emit portInfoListChanged();
}

//-----------------------------------------------------------------

QStringList SerialClient::getDataBitsList() const
{
    return MapDataBits.keys();
}

//-----------------------------------------------------------------

QStringList SerialClient::getParityList() const
{
    return MapParity.keys();
}

//-----------------------------------------------------------------

QStringList SerialClient::getStopBitsList() const
{
    return MapStopBits.keys();
}

//-----------------------------------------------------------------

QStringList SerialClient::getCtrlList() const
{
    return MapCtrl.keys();
}



//-----------------------------------------------------------------

bool SerialClient::send(const QByteArray &data, TerminationByte trmByteMode)
{
    if (_pSerial->isOpen() == false) return false;

    QByteArray ba(data);
    if (trmByteMode == TerminationByte::CR) ba.append(0x0d);
    if (trmByteMode == TerminationByte::LF) ba.append(0x0a);
    if (trmByteMode == TerminationByte::CRLF) { ba.append(0x0d); ba.append(0x0a); }

    _pSerial->write(ba);
    _txModel.addBytes(ba);

    return true;
}

//-----------------------------------------------------------------

bool SerialClient::send(const QByteArray &data, int trmByteMode)
{
    TerminationByte mode = static_cast<TerminationByte>(trmByteMode);
    return send(data, mode);
}

//-----------------------------------------------------------------

bool SerialClient::send(const QByteArray &data)
{
    return send(data, TerminationByte::None);
}

//-----------------------------------------------------------------

void SerialClient::init()
{
    updatePortList();

    // DataBits
    if (MapDataBits.isEmpty())
    {
        MapDataBits.append("8", QSerialPort::Data8);
        MapDataBits.append("7", QSerialPort::Data7);
        MapDataBits.append("6", QSerialPort::Data6);
        MapDataBits.append("5", QSerialPort::Data5);
    }

    // Parity
    if (MapParity.isEmpty())
    {
        MapParity.append("None", QSerialPort::NoParity);
        MapParity.append("Even", QSerialPort::EvenParity);
        MapParity.append("Odd", QSerialPort::OddParity);
        MapParity.append("Space", QSerialPort::SpaceParity);
        MapParity.append("Mark", QSerialPort::MarkParity);
    }

    // StopBits
    if (MapStopBits.isEmpty())
    {
        MapStopBits.append("1", QSerialPort::OneStop);
        MapStopBits.append("1.5", QSerialPort::OneAndHalfStop);
        MapStopBits.append("2", QSerialPort::TwoStop);
    }

    // FlowControl
    if (MapCtrl.isEmpty())
    {
        MapCtrl.append("None", QSerialPort::NoFlowControl);
        MapCtrl.append("Hardware", QSerialPort::HardwareControl);
        MapCtrl.append("Software", QSerialPort::SoftwareControl);
    }

    // Termination byte
    QMetaEnum termByteEnum = QMetaEnum::fromType<TerminationByte>();
    for (qint32 i = 0, count = termByteEnum.keyCount(); i < count; i++) _termByteList << termByteEnum.key(i);
}

//-----------------------------------------------------------------

void SerialClient::onReadyRead()
{
    QByteArray ba = _pSerial->readAll();
    if (ba.isEmpty()) return;

    _rxModel.addBytes(ba);
    emit received(ba);
}

//-----------------------------------------------------------------


}   // namespace checksum

