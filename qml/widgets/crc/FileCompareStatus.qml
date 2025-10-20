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
import "../../components"

Item
{
    id: root
    width: 70
    height: 150

    property bool _busy: false

    signal butResetClicked()

    function setStatus(color, mes)
    {
        id_rcRes.border.color = color
        id_txtRes.color = color
        id_txtRes.text = mes
    }

    function reset()
    {
        setStatus(Style.checksum.color1, "?")
    }

    KPane
    {
        anchors.fill: parent

        Rectangle
        {
            id: id_rcRes
            width: parent.width / 1.5
            height: width
            anchors.centerIn: parent
            color: "transparent"
            border.width: 2
            radius: 5

            Text
            {
                id: id_txtRes
                anchors.centerIn: parent
                font.pixelSize: id_rcRes.width / 1.5
                font.bold: true
            }
        }

        Button
        {
            width: 40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            icon.source: "qrc:/resources/images/icon/refresh_dark2_128.png"
            enabled: !_busy
            onClicked: butResetClicked()


            ToolTip.delay: 1000
            ToolTip.timeout: 2000
            ToolTip.visible: hovered
            ToolTip.text: qsTr("Reset")
        }
    }   // Rectangle

    Component.onCompleted: reset()

}
