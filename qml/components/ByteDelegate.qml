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

Item
{
    id: root
    opacity: _valid ? 1 : 0

    property bool _valid: false
    property bool _selected: false

    property string _char: ""
    property string _hex: ""
    property int    _msec: 0

    signal selectClicked()
    signal unselectClicked()
    signal copyAsCharClicked()
    signal copyAsHexClicked()
    signal copyAsBinClicked()
    signal infoClicked()


    Rectangle
    {
        id: id_rc
        anchors.fill: parent
        color: _selected ? Style.byteDelegate.selectColor : Style.byteDelegate.color
        Column
        {
            id: id_clm
            anchors.centerIn: parent
            spacing: 0
            Text
            {
                visible: _char.length > 0 && _showAscii;
                font.pixelSize: Style.byteDelegate.text.fontSize
                text: _char
                color: Style.byteDelegate.text.color1
            }

            Text
            {
                visible: _hex.length > 0 && _showHex;
                font.pixelSize: Style.byteDelegate.text.fontSize
                text: _hex
                color: _showAscii ? Style.byteDelegate.text.color2 : Style.byteDelegate.text.color1
            }
        }
    }

    MouseArea
    {
        anchors.fill: parent
        cursorShape: _valid ? Qt.PointingHandCursor : Qt.ArrowCursor
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked:
        {
            if (_valid === false) { unselectClicked(); return }
            if (mouse.button === Qt.LeftButton) selectClicked()
            if (mouse.button === Qt.RightButton) id_menu.open()
        }
    }

    Menu
    {
        id: id_menu
        MenuItem
        {
            text: qsTr("Copy as Char")
            onTriggered: copyAsCharClicked()
        }
        MenuItem
        {
            text: qsTr("Copy as Hex")
            onTriggered: copyAsHexClicked()
        }
        MenuItem
        {
            text: qsTr("Copy as Bin")
            onTriggered: copyAsBinClicked()
        }
        MenuItem
        {
            text: qsTr("Info")
            onTriggered: infoClicked()
        }
    }   // Menu


}
