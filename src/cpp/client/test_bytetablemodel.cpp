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
#include "test_bytetablemodel.h"
#include <QTest>

#include <QDebug>

namespace client
{

Test_ByteTableModel::Test_ByteTableModel(QObject *parent)
    : QObject{parent}
{
}

//-----------------------------------------------------------------

void Test_ByteTableModel::testColumnCount()
{
    // Clear model
    _model.clear();

    // Add 76 bytes 1 + 1 + 60 + 14
    _model.addByte(0x01);
    _model.addByte(0x02);
    for (int i = 0; i < 60; ++i) _model.addByte(i);
    QByteArray ba(14, 0x01);
    _model.addBytes(ba);

    // Set new line None
    _model.setNewLineMode(ByteTableModel::NewLineMode::None);

    // Set count 16
    _model.setColumnCount(16);
    // RowCount == 5 ?
    QCOMPARE(_model.rowCount(QModelIndex()), 5);

    // Set count 32
    _model.setColumnCount(32);
    // RowCount == 3 ?
    QCOMPARE(_model.rowCount(QModelIndex()), 3);

    // Set count 64
    _model.setColumnCount(64);
    // RowCount == 2 ?
    QCOMPARE(_model.rowCount(QModelIndex()), 2);
}

//-----------------------------------------------------------------

void Test_ByteTableModel::testNewLineCR()
{
    processNewLine(ByteTableModel::NewLineMode::CR);
}

//-----------------------------------------------------------------

void Test_ByteTableModel::testNewLineLF()
{
    processNewLine(ByteTableModel::NewLineMode::LF);
}

//-----------------------------------------------------------------

void Test_ByteTableModel::testNewLineCRLF()
{
    processNewLine(ByteTableModel::NewLineMode::CRLF);
}

//-----------------------------------------------------------------

void Test_ByteTableModel::testNewLineNull()
{
    processNewLine(ByteTableModel::NewLineMode::Null);
}

//-----------------------------------------------------------------

void Test_ByteTableModel::testNewLineTimeout()
{
    // Clear model
    _model.clear();

    // Set NewLineMode::Null
    _model.setNewLineMode(ByteTableModel::NewLineMode::Timeout);
    // Set count 16
    _model.setColumnCount(16);
    // Set timeout 50 ms
    _model.setTimeout(50);

    static const char seq[] = {
        '\x30', '\x31', '\x32', '\x33', '\x34'
    };
    QByteArray data = QByteArray::fromRawData(seq, sizeof(seq));

    // Add data
    _model.addBytes(data);
    _model.addBytes(data);
    QTest::qWait(40);
    _model.addBytes(data);
    // RowCount == 1 ?
    QCOMPARE(_model.rowCount(QModelIndex()), 1);

    QTest::qWait(55);
    _model.addBytes(data);
    // RowCount == 2 ?
    QCOMPARE(_model.rowCount(QModelIndex()), 2);

    _model.addBytes(data);
    _model.addBytes(data);
    _model.addBytes(data);
    _model.addBytes(data);
    // RowCount == 3 ?
    QCOMPARE(_model.rowCount(QModelIndex()), 3);

    // Set timeout 50 ms
    _model.setTimeout(30);
    // RowCount == 4 ?
    QCOMPARE(_model.rowCount(QModelIndex()), 4);

    // Set count 32
    _model.setColumnCount(32);
    // Set timeout 100 ms
    _model.setTimeout(100);
    // RowCount == 2 ?
    QCOMPARE(_model.rowCount(QModelIndex()), 2);
}

//-----------------------------------------------------------------

void Test_ByteTableModel::processNewLine(ByteTableModel::NewLineMode mode)
{
    // Clear model
    _model.clear();
    // Set new line mode
    _model.setNewLineMode(mode);
    // Set count 16
    _model.setColumnCount(16);

    static const char seq1[] = {
        '\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37', '\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x00',
        '\x30', '\x31', '\x32', '\x33', '\x00', '\x35', '\x36', '\x37', '\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F',
        '\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37', '\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F',
        '\x30', '\x31', '\x32', '\x00', '\x34', '\x35', '\x36', '\x37', '\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F',
        '\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37', '\x38', '\x00', '\x3A', '\x3B'
    };
    static const char seq2[] = {
        '\x30', '\x31', '\x32', '\x00', '\x34', '\x35', '\x36', '\x00', '\x38', '\x39'
    };
    QByteArray data1 = QByteArray::fromRawData(seq1, sizeof(seq1));
    QByteArray data2 = QByteArray::fromRawData(seq2, sizeof(seq2));

    if (mode == ByteTableModel::NewLineMode::CR) // \r
    {
        data1.replace(0, 0x0d);
        data2.replace(0, 0x0d);
    }
    if (mode == ByteTableModel::NewLineMode::LF) // \n
    {
        data1.replace(0, 0x0a);
        data2.replace(0, 0x0a);
    }
    if (mode == ByteTableModel::NewLineMode::Null) {}
    if (mode == ByteTableModel::NewLineMode::CRLF)
    {
        static const char crlf[] = {
            '\x0d', '\x0a'
        };
        int idx = 0;
        while (1)
        {
            idx = data1.indexOf('\x00', idx + 1);
            if (idx == -1) break;
            data1.replace(idx - 1, 2, QByteArray::fromRawData(crlf, sizeof(crlf)));
        }
        idx = 0;
        while (1)
        {
            idx = data2.indexOf('\x00', idx + 1);
            if (idx == -1) break;
            data2.replace(idx - 1, 2, QByteArray::fromRawData(crlf, sizeof(crlf)));
        }
    }

    // Add part1
    _model.addBytes(data1);
    // RowCount == 7 ?
    QCOMPARE(_model.rowCount(QModelIndex()), 7);

    // Add part2
    _model.addBytes(data2);
    // RowCount == 9 ?
    QCOMPARE(_model.rowCount(QModelIndex()), 9);

    // Set count 32
    _model.setColumnCount(32);
    // RowCount == 7 ?
    QCOMPARE(_model.rowCount(QModelIndex()), 7);

}

//-----------------------------------------------------------------

}   // client

