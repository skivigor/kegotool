/*
 * KegoTool using Qt and Qml.
 * Copyright (C) 2025  Skiv <skivigor@gmail.com>
 *
 * The class taken as a basis:
 * https://github.com/KoynovStas/uCRC_t/blob/master/ucrc_t.h
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
#ifndef CRCENGINE_H
#define CRCENGINE_H

#include <QObject>
#include <QQmlEngine>
//#include <QVector>

namespace checksum
{

class CrcMode
{
    Q_GADGET
public:
    enum Mode {
        CRC8 = 8,
        CRC16 = 16,
        CRC32 = 32,
        CRC64 = 64
    };
    Q_ENUM(Mode)

    static void registerQml()
    {
        qmlRegisterUncreatableType<CrcMode>("CrcMode", 1, 0, "CrcMode", "");
    }

private:
    explicit CrcMode();
};


class CrcEngine
{
public:
    explicit CrcEngine(CrcMode::Mode mode,
                       quint64       poly,
                       quint64       init,
                       bool          refin,
                       bool          refout,
                       quint64       bxor);

    // Calculate CRC
    quint64 getCrc(const QByteArray &data) const;

    // Chunks of data
    quint64 crcChunkStart(const QByteArray &data) const;
    quint64 crcChunkProcess(const QByteArray &data, quint64 rawCrc) const;
    quint64 crcChunkFinish(quint64 rawCrc) const;

private:
    quint64 reflect(quint64 data, quint8 bits) const;
    void    engineInit();

private:
    CrcMode::Mode  _mode;
    quint64        _poly;
    quint64        _init;
    bool           _refIn;
    bool           _refOut;
    quint64        _bxor;

    quint64        _table[256] = {0};
    quint64        _topBit = 0;
    quint64        _crcInit = 0;
    quint64        _crcMask = 0;
    quint8         _shift = 0;



};


}   // namespace checksum


#endif // CRCENGINE_H
