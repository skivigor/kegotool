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
#include "test_crccalc.h"
#include <QTest>
#include <QJsonObject>

#include <QFile>
#include <QFileInfo>


namespace checksum
{

Test_CrcCalc::Test_CrcCalc(QObject *parent)
    : QObject{parent}
{
    // Test file
    const QString seq("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n");
    QFileInfo fileInfo(_fileName);
    if (fileInfo.exists() == false)
    {
        QByteArray ba;
        for (int i = 0; i < 25000; ++i) ba.append(seq.toLocal8Bit());

        QFile file(_fileName);
        file.open(QIODevice::WriteOnly);
        file.write(ba);
        file.close();
    }

    // Used crc results calculated for hex sequence 0102030405060708090A0B0C0D0E0F and file checksum_test.dat
    //----------------------------
    // CRC8 algorithms
    _crc8Types.append( QJsonObject {
        {"name", "CRC8"},
        {"poly", "07"}, {"init", "0"}, {"xor", "0"},
        {"refin", false}, {"refout", false},
        {"crcSeq", "41"},
        {"crcFile", "B8"}
    });
    _crc8Types.append( QJsonObject {
        {"name", "CRC8/CDMA"},
        {"poly", "9B"}, {"init", "FF"}, {"xor", "0"},
        {"refin", false}, {"refout", false},
        {"crcSeq", "FF"},
        {"crcFile", "3F"}
    });
    _crc8Types.append( QJsonObject {
        {"name", "CRC8/ITU"},
        {"poly", "07"}, {"init", "0"}, {"xor", "55"},
        {"refin", false}, {"refout", false},
        {"crcSeq", "14"},
        {"crcFile", "ED"}
    });

    //----------------------------
    // CRC16 algorithms
    _crc16Types.append( QJsonObject {
        {"name", "CRC16/CCIT_ZERO"},
        {"poly", "1021"}, {"init", "0"}, {"xor", "0"},
        {"refin", false}, {"refout", false},
        {"crcSeq", "513D"},
        {"crcFile", "298A"}
    });
    _crc16Types.append( QJsonObject {
        {"name", "CRC16/CCIT"},
        {"poly", "1021"}, {"init", "0"}, {"xor", "0"},
        {"refin", true}, {"refout", true},
        {"crcSeq", "BC40"},
        {"crcFile", "9030"}
    });
    _crc16Types.append( QJsonObject {
        {"name", "CRC16/CCIT_FALSE"},
        {"poly", "1021"}, {"init", "FFFF"}, {"xor", "0"},
        {"refin", false}, {"refout", false},
        {"crcSeq", "1FFE"},
        {"crcFile", "F234"}
    });
    _crc16Types.append( QJsonObject {
        {"name", "CRC16/AUG_CCIT"},
        {"poly", "1021"}, {"init", "1D0F"}, {"xor", "0"},
        {"refin", false}, {"refout", false},
        {"crcSeq", "96D1"},
        {"crcFile", "A642"}
    });
    _crc16Types.append( QJsonObject {
        {"name", "CRC16/XMODEM"},
        {"poly", "1021"}, {"init", "0"}, {"xor", "0"},
        {"refin", true}, {"refout", true},
        {"crcSeq", "BC40"},
        {"crcFile", "9030"}
    });

    //----------------------------
    // CRC32 algorithms
    _crc32Types.append( QJsonObject {
        {"name", "CRC32"},
        {"poly", "4C11DB7"}, {"init", "FFFFFFFF"}, {"xor", "FFFFFFFF"},
        {"refin", true}, {"refout", true},
        {"crcSeq", "F5A6AA3A"},
        {"crcFile", "3EF140AB"}
    });
    _crc32Types.append( QJsonObject {
        {"name", "CRC32/POSIX"},
        {"poly", "4C11DB7"}, {"init", "0"}, {"xor", "FFFFFFFF"},
        {"refin", false}, {"refout", false},
        {"crcSeq", "03A8227A"},
        {"crcFile", "C55079B0"}
    });
    _crc32Types.append( QJsonObject {
        {"name", "CRC32/XFER"},
        {"poly", "AF"}, {"init", "0"}, {"xor", "0"},
        {"refin", false}, {"refout", false},
        {"crcSeq", "56C38155"},
        {"crcFile", "66C34861"}
    });

    //----------------------------
    // CRC64 algorithms
    _crc64Types.append( QJsonObject {
        {"name", "CRC64/ECMA182"},
        {"poly", "42F0E1EBA9EA3693"}, {"init", "0"}, {"xor", "0"},
        {"refin", false}, {"refout", false},
        {"crcSeq", "F9C42D91ABAF3B55"},
        {"crcFile", "7DE4D33E67432FDA"}
    });
    _crc64Types.append( QJsonObject {
        {"name", "CRC64/WE"},
        {"poly", "42F0E1EBA9EA3693"}, {"init", "FFFFFFFFFFFFFFFF"}, {"xor", "FFFFFFFFFFFFFFFF"},
        {"refin", false}, {"refout", false},
        {"crcSeq", "488658AC79ECB8A9"},
        {"crcFile", "D3ED265E928FCEDE"}
    });
    _crc64Types.append( QJsonObject {
        {"name", "CRC64/XZ"},
        {"poly", "42F0E1EBA9EA3693"}, {"init", "FFFFFFFFFFFFFFFF"}, {"xor", "FFFFFFFFFFFFFFFF"},
        {"refin", true}, {"refout", true},
        {"crcSeq", "AC04197DF15C32B8"},
        {"crcFile", "FD2F1E8B0B500531"}
    });
    //----------------------------

}

//-----------------------------------------------------------------

void Test_CrcCalc::testCrc8()
{
    process(CrcMode::Mode::CRC8, _crc8Types);
}

//-----------------------------------------------------------------

void Test_CrcCalc::testCrc16()
{
    process(CrcMode::Mode::CRC16, _crc16Types);
}

//-----------------------------------------------------------------

void Test_CrcCalc::testCrc32()
{
    process(CrcMode::Mode::CRC32, _crc32Types);
}

//-----------------------------------------------------------------

void Test_CrcCalc::testCrc64()
{
    process(CrcMode::Mode::CRC64, _crc64Types);
}

//-----------------------------------------------------------------

void Test_CrcCalc::process(CrcMode::Mode mode, const QJsonArray &types)
{
    _calc.setCrcMode(mode);

    foreach (auto itm, types)
    {
        QJsonObject cfg = itm.toObject();

        // Sequence
        _calc.calculate(cfg, QByteArray::fromHex(_hexTestSeq.toLocal8Bit()));
        QTest::qWait(_timeout);
        QCOMPARE(_calc.getResult(), cfg.value("crcSeq").toString().toULongLong(nullptr, 16));

        // File
        _calc.calculate(cfg, _fileName);;
        QTest::qWait(_timeout);
        QCOMPARE(_calc.getResult(), cfg.value("crcFile").toString().toULongLong(nullptr, 16));
    }

}

//-----------------------------------------------------------------



}   // checksum

