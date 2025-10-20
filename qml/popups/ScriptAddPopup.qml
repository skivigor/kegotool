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

    signal addScript(string name)

    QtObject
    {
        id: set

        function add()
        {
            var name = id_fldName.text
            name = name.replace(".js", "")
            if (name.length === 0) return

            addScript(name)
            root.destroy()
        }
    }

    KPopupPane
    {
        width: 400
        height: 180
        anchors.centerIn: parent
        _header: qsTr("Add script")
        onButCloseClicked: root.destroy()

        Column
        {
            spacing: 10
            width: parent.width - 50
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 10

            Text
            {
                anchors.horizontalCenter: parent.horizontalCenter
                color: Style.text.label3.color
                font.pixelSize: Style.text.label3.fontSize
                text: qsTr("Enter script name")
            }

            TextField
            {
                id: id_fldName
                width: parent.width
                placeholderText: qsTr("Script name")
                onAccepted: set.add()
            }

            Row
            {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 20
                Button
                {
                    width: 110
                    text: qsTr("Add")
                    enabled: id_fldName.text.length > 0
                    onClicked: set.add()
                }
                Button
                {
                    width: 110
                    text: qsTr("Cancel")
                    onClicked: root.destroy()
                }
            }
        }   // Column
    }    // Rectangle

}
