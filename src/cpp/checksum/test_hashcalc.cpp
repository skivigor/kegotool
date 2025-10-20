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
#include "test_hashcalc.h"
#include <QTest>
#include <QJsonObject>

#include <QFile>
#include <QFileInfo>

namespace checksum
{

Test_HashCalc::Test_HashCalc(QObject *parent)
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

    _hashTypes.append( QJsonObject {
        {"name", "Md4"},
        {"hashSeq", "0F3226C409C267486441D4B69EDEABD5"},
        {"hashFile", "3F99AD0A534E20981EF89600FF1D5C37"}
    });
    _hashTypes.append( QJsonObject {
        {"name", "Md5"},
        {"hashSeq", "AB3825D5AEAEC5925B05D44BEB7DDC7D"},
        {"hashFile", "EBD4C4F7AFCC04E680F8F653F4B84794"}
    });
    _hashTypes.append( QJsonObject {
        {"name", "Sha256"},
        {"hashSeq", "36030FCC7E566294905B49A720EB45BF962209D2EE1C9B73E2B7BC7AE8830376"},
        {"hashFile", "ACBC1902B202F9314079CC752E82EDFB61358E8418DAAC608C8490E8915566F6"}
    });
    _hashTypes.append( QJsonObject {
        {"name", "Sha512"},
        {"hashSeq", "E013AC44F4A653A96CF3C120D9A741F9A2F2A56628E93A43E78907BC85C5FFB8C2A7A29885880E31E541CAFC956576089095F090FE721D8A3D5E0B1B07B6D36D"},
        {"hashFile", "B1C731B693EA030C0E66C69A7CE5140F13334F3AD4038E585B9A3460275AE22CCF75A8F36137DE4284869E010EA25358036DB452190C5886C4E58D2BBE5A63FA"}
    });

}

//-----------------------------------------------------------------

void Test_HashCalc::testHash()
{
    foreach (auto itm, _hashTypes)
    {
        QJsonObject cfg = itm.toObject();

        // Sequence
        _calc.calculate(cfg.value("name").toString(), QByteArray::fromHex(_hexTestSeq.toLocal8Bit()));
        QTest::qWait(_timeout);
        QByteArray res = QByteArray::fromHex(cfg.value("hashSeq").toString().toLocal8Bit());
        QCOMPARE(_calc.getResult(), res);

        // File
        _calc.calculate(cfg.value("name").toString(), _fileName);
        QTest::qWait(_timeout);
        res = QByteArray::fromHex(cfg.value("hashFile").toString().toLocal8Bit());
        QCOMPARE(_calc.getResult(), res);
    }

}

//-----------------------------------------------------------------

}   // checksum

