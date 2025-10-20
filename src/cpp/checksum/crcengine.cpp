/*
 * KegoTool using Qt and Qml.
 * Copyright (C) 2025  Skiv <skivigor@gmail.com>
 *
 * * The class taken as a basis:
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
#include "crcengine.h"

#include <QDebug>

namespace checksum
{

CrcEngine::CrcEngine(CrcMode::Mode mode,
                     quint64       poly,
                     quint64       init,
                     bool          refIn,
                     bool          refOut,
                     quint64       bxor)
    : _mode(mode),
      _poly(poly),
      _init(init),
      _refIn(refIn),
      _refOut(refOut),
      _bxor(bxor)
{
    engineInit();
}

//-----------------------------------------------------------------

quint64 CrcEngine::getCrc(const QByteArray &data) const
{
    uint64_t crc = crcChunkProcess(data, _crcInit);

    return crcChunkFinish(crc);
}

//-----------------------------------------------------------------

quint64 CrcEngine::crcChunkStart(const QByteArray &data) const
{
    return crcChunkProcess(data, _crcInit);
}

//-----------------------------------------------------------------

quint64 CrcEngine::crcChunkProcess(const QByteArray &data, quint64 rawCrc) const
{
    quint8 bits = static_cast<quint8>(_mode);

    if (bits > 8)
    {
        foreach (quint8 val, data)
        {
            if (_refIn == true) rawCrc = (rawCrc >> 8) ^ _table[(rawCrc ^ val) & 0xff];
            else rawCrc = (rawCrc << 8) ^ _table[((rawCrc >> _shift) ^ val) & 0xff];
        }
    } else
    {
        foreach (quint8 val, data)
        {
            if (_refIn == true) rawCrc = _table[(rawCrc ^ val) & 0xff];
            else rawCrc = _table[((rawCrc << _shift) ^ val) & 0xff];
        }
    }

    return rawCrc;

}

//-----------------------------------------------------------------

quint64 CrcEngine::crcChunkFinish(quint64 rawCrc) const
{
    quint8 bits = static_cast<quint8>(_mode);
    if (_refOut^_refIn) rawCrc = reflect(rawCrc, bits);

    rawCrc ^= _bxor;
    rawCrc &= _crcMask; //for CRC not power 2

    return rawCrc;
}

//-----------------------------------------------------------------

quint64 CrcEngine::reflect(quint64 data, quint8 bits) const
{
    quint64 reflection = 0;

    while(bits--)
    {
        reflection = (reflection << 1) | (data & 1);
        data >>= 1;
    }

    return reflection;
}

//-----------------------------------------------------------------

void CrcEngine::engineInit()
{
    quint8 bits = static_cast<quint8>(_mode);
    _topBit  = (uint64_t)1 << (bits - 1);
    _crcMask = ( (_topBit - 1) << 1) | 1;

    if(bits > 8) _shift = (bits - 8);
    else _shift = (8 - bits);

    if(_refIn == true) _crcInit = reflect(_init, bits);
    else _crcInit = _init;

    // Create table
    for(int i = 0; i < 256; ++i)
    {
        quint64 crc = 0;

        for(quint8 mask = 0x80; mask; mask >>= 1)
        {
            if ( i & mask ) crc ^= _topBit;

            if (crc & _topBit)
            {
                crc <<= 1;
                crc ^= _poly;
            } else
            {
                crc <<= 1;
            }
        }

        crc &= _crcMask; //for CRC not power 2

        if(_refIn == true) _table[reflect(i, 8)] = reflect(crc, bits);
        else _table[i] = crc;
    }

}

//-----------------------------------------------------------------

}    // namespace checksum

