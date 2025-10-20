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
#ifndef ICLIENT_H
#define ICLIENT_H

//#include <QByteArray>
#include <QObject>
#include <QQmlEngine>

class QByteArray;

namespace client
{

class IClient
{
    Q_GADGET

public:
    enum class TerminationByte
    {
        None = 0,
        CR,
        LF,
        CRLF
    };
    Q_ENUM(TerminationByte)

public:
    virtual ~IClient() {}

public slots:
    virtual bool send(const QByteArray &data, TerminationByte trmByteMode) = 0;
    virtual bool send(const QByteArray &data, int trmByteMode) = 0;
    virtual bool send(const QByteArray &data) = 0;

signals:  // moc ignore
    virtual void received(const QByteArray &data) = 0;

};

}   // namespace client

#endif // ICLIENT_H
