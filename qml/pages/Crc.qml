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
import "../widgets/crc"

Page
{
    id: id_page
    title: qsTr("CRC/Hash")

    Flickable
    {
        anchors.fill: parent
        anchors.margins: 5
        clip: true
        contentHeight: id_clm.height
        contentWidth: id_clm.width

        Column
        {
            id: id_clm
            width: id_page.width - 10
            spacing: 10

            CrcView
            {
                id: id_vCrc
            }

            HashView
            {
                id: id_vHash
            }

            FileCompareView
            {
                id: id_vCompare
            }

        }
        ScrollBar.vertical: ScrollBar {}
    }

    Component.onCompleted: console.log("CRC ctor")
    Component.onDestruction:  console.log("CRC dtor")
}
