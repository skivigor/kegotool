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
#ifndef TEST_CRCCALC_H
#define TEST_CRCCALC_H

#include <QObject>
#include <QJsonArray>
#include "crccalc.h"

namespace checksum
{

class Test_CrcCalc : public QObject
{
    Q_OBJECT
public:
    explicit Test_CrcCalc(QObject *parent = nullptr);

signals:

private slots:
    void testCrc8();
    void testCrc16();
    void testCrc32();
    void testCrc64();

private:
    void process(CrcMode::Mode mode, const QJsonArray &types);

private:
    CrcCalc       _calc;
    QJsonArray    _crc8Types;
    QJsonArray    _crc16Types;
    QJsonArray    _crc32Types;
    QJsonArray    _crc64Types;

    const QString _hexTestSeq = "0102030405060708090A0B0C0D0E0F";
    const QString _fileName = "checksum_test.dat";
    const int     _timeout = 50;

};


}   // namespace checksum


#endif // TEST_CRCCALC_H
