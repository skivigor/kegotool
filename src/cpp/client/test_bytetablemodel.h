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
#ifndef TEST_BYTETABLEMODEL_H
#define TEST_BYTETABLEMODEL_H

#include <QObject>
#include "bytetablemodel.h"

namespace client
{

class Test_ByteTableModel : public QObject
{
    Q_OBJECT
public:
    explicit Test_ByteTableModel(QObject *parent = nullptr);

signals:

private slots:
    void testColumnCount();
    void testNewLineCR();
    void testNewLineLF();
    void testNewLineCRLF();
    void testNewLineNull();
    void testNewLineTimeout();

private:
    void processNewLine(ByteTableModel::NewLineMode mode);

private:
    ByteTableModel  _model;

};


}   // client


#endif // TEST_BYTETABLEMODEL_H
