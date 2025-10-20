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
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Item
{
    id: root
    width: 400
    implicitHeight: id_clmContent.height

    //-----------------------------------------------------------------

    property bool _processing: false

    //-----------------------------------------------------------------

    signal butCalcFieldClicked(data: var)
    signal butCalcFileClicked(path: url)

    //-----------------------------------------------------------------

    Column
    {
        id: id_clmContent
        width: parent.width
        spacing: 10

        FieldChecksumItem
        {
            width: parent.width
            _processing: root._processing
            onButCalcClicked: (data) => { butCalcFieldClicked(data) }
        }

        FileChecksumItem
        {
            width: parent.width
            _processing: root._processing
            onSelectedFirstUrl: (url) => { butCalcFileClicked(url) }
        }

    }   // Column

}

