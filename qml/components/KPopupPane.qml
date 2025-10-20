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

Rectangle
{
    color: Style.popup.color
    border.width: Style.popup.borderWidth
    border.color: Style.popup.borderColor

    property string _header: ""

    signal butCloseClicked()

    Rectangle
    {
        width: parent.width
        height: 30
        border.width: Style.popup.borderWidth
        border.color: Style.popup.borderColor

        gradient: Gradient {
            GradientStop { position: 0.0; color: Style.popup.topBar.color1 }
            GradientStop { position: 0.33; color: Style.popup.topBar.color2 }
            GradientStop { position: 1.0; color: Style.popup.topBar.color3 }
        }

        Text
        {
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: parent.height / 2
            color: Style.text.header.color
            text: _header
        }

        Button
        {
            width: 40
            height: parent.height - 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "qrc:/resources/images/icon/close3_128.png"
            onClicked: butCloseClicked()
        }

    }   // rc
}
