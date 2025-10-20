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
#ifndef BYTEUTILS_H
#define BYTEUTILS_H

#include <QObject>
#include <QByteArray>
#include <QString>

namespace util
{

class ByteUtils
{
    Q_GADGET
public:
    explicit ByteUtils();

    Q_INVOKABLE static QByteArray byteArrayFromHexString(const QString &hex);
    Q_INVOKABLE static QByteArray byteArrayFromAsciString(const QString &asci);

    Q_INVOKABLE static qint64  signedIntFromHex(const QString &hex, bool lsb = true);
    Q_INVOKABLE static quint64 unsignedIntFromHex(const QString &hex, bool lsb = true);

    Q_INVOKABLE static QByteArray reverseByteArray(const QByteArray &data);

};


}   // namespace util


#endif // BYTEUTILS_H
