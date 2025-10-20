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

import "../components"

PopupView
{
    id: root
    anchors.fill: parent

    property string _portName
    property string _manufacturer
    property string _pid
    property string _vid

    KPopupPane
    {
        width: 400
        height: 200
        anchors.centerIn: parent
        _header: qsTr("Port info")
        onButCloseClicked: root.destroy()

        Column
        {
            spacing: 10
            width: parent.width - 100
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 10

            Row
            {
                spacing: 20
                KLabel { font.pixelSize: 18; text: qsTr("Port:") }
                KLabel { font.pixelSize: 18; text: _portName }
            }
            Row
            {
                spacing: 20
                KLabel { font.pixelSize: 18; text: qsTr("Manuf:") }
                KLabel { font.pixelSize: 18; text: _manufacturer }
            }
            Row
            {
                spacing: 20
                KLabel { font.pixelSize: 18; text: "PID:" }
                KLabel { font.pixelSize: 18; text: _pid }
            }
            Row
            {
                spacing: 20
                KLabel { font.pixelSize: 18; text: "VID:" }
                KLabel { font.pixelSize: 18; text: _vid }
            }
        }   // Column

    }   // KPopupPane


}
