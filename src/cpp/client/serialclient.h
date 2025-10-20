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
#ifndef SERIALCLIENT_H
#define SERIALCLIENT_H

#include "iclient.h"
#include <QObject>
//#include <QSerialPort>
#include <QStringList>
#include <QJsonArray>
#include "bytetablemodel.h"

class QSerialPort;
class QByteArray;

namespace client
{

class SerialClient : public QObject, public IClient
{
    Q_OBJECT
    Q_PROPERTY(bool        openned      READ isOpenned       NOTIFY opennedChanged)
    Q_PROPERTY(QString     status       READ getStatus       NOTIFY statusChanged)
    Q_PROPERTY(QJsonArray  portInfoList READ getPortInfoList NOTIFY portInfoListChanged)
    Q_PROPERTY(QStringList dataBitsList READ getDataBitsList CONSTANT)
    Q_PROPERTY(QStringList parityList   READ getParityList   CONSTANT)
    Q_PROPERTY(QStringList stopBitsList READ getStopBitsList CONSTANT)
    Q_PROPERTY(QStringList ctrlList     READ getCtrlList     CONSTANT)

    Q_PROPERTY(QStringList termByteList READ getTermByteList CONSTANT)

public:
    explicit SerialClient(QObject *parent = nullptr);
    ~SerialClient();

    Q_INVOKABLE bool open(const QString &port,
                          int baud,
                          const QString &dataBits,
                          const QString &parity,
                          const QString &stopBits,
                          const QString &flow);
    Q_INVOKABLE void    close();
    Q_INVOKABLE bool    isOpenned() const { return _openned; }
    Q_INVOKABLE QString getStatus() const { return _status; }
    Q_INVOKABLE void    updatePortList();

    Q_INVOKABLE QJsonArray  getPortInfoList() const { return _portInfoList; }
    Q_INVOKABLE QStringList getDataBitsList() const;
    Q_INVOKABLE QStringList getParityList() const;
    Q_INVOKABLE QStringList getStopBitsList() const;
    Q_INVOKABLE QStringList getCtrlList() const;

    Q_INVOKABLE QStringList getTermByteList() const { return _termByteList; }

    Q_INVOKABLE QObject* getRxModel() { return &_rxModel; }
    Q_INVOKABLE QObject* getTxModel() { return &_txModel; }

    //--------------------------------------------

signals:
    void opennedChanged();
    void statusChanged();
    void portInfoListChanged();
    void received(const QByteArray &data) override;

public slots:
    bool send(const QByteArray &data, TerminationByte trmByteMode) override;
    bool send(const QByteArray &data, int trmByteMode) override;
    bool send(const QByteArray &data) override;

private:
    void init();

private slots:
    void onReadyRead();

private:
    QSerialPort *_pSerial = nullptr;
    bool         _openned = false;
    QString      _status = "NoError";
    QJsonArray   _portInfoList;
    QStringList  _termByteList;

    ByteTableModel  _rxModel;
    ByteTableModel  _txModel;

};


}   // namespace client


#endif // SERIALCLIENT_H
