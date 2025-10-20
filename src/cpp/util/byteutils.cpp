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
#include "byteutils.h"

namespace util
{

ByteUtils::ByteUtils()
{
}

//-----------------------------------------------------------------

QByteArray ByteUtils::byteArrayFromHexString(const QString &hex)
{
    return QByteArray::fromHex(hex.toLocal8Bit());
}

//-----------------------------------------------------------------

QByteArray ByteUtils::byteArrayFromAsciString(const QString &asci)
{
    return asci.toLocal8Bit();
}

//-----------------------------------------------------------------

qint64 ByteUtils::signedIntFromHex(const QString &hex, bool lsb)
{
    QByteArray ba;

    if (lsb == true)
        ba = QByteArray::fromHex(hex.toLocal8Bit());
    else
        ba = reverseByteArray(QByteArray::fromHex(hex.toLocal8Bit()));

    if (ba.isEmpty() || ba.size() > 8) return 0;

    int len = ba.size();
    qint64 res = 0;
    memcpy((qint8 *)&res, ba.data(), len);

    if (len == 1) res = static_cast<qint8>(res);
    if (len == 2) res = static_cast<qint16>(res);
    if (len > 2 && len <= 4) res = static_cast<qint32>(res);

    return res;
}

//-----------------------------------------------------------------

quint64 ByteUtils::unsignedIntFromHex(const QString &hex, bool lsb)
{
    QByteArray ba;

    if (lsb == true)
        ba = QByteArray::fromHex(hex.toLocal8Bit());
    else
        ba = reverseByteArray(QByteArray::fromHex(hex.toLocal8Bit()));

    if (ba.isEmpty() || ba.size() > 8) return 0;

    quint64 res = 0;
    memcpy((quint8 *)&res, ba.data(), ba.size());

    return res;
}

//-----------------------------------------------------------------

QByteArray ByteUtils::reverseByteArray(const QByteArray &data)
{
    QByteArray ba;
    ba.reserve(data.size());

    for(int i = data.size() - 1; i >= 0; --i)
        ba.append(data.at(i));

    return ba;
}

//-----------------------------------------------------------------

}   // namespace util

