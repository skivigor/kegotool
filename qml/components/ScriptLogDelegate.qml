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
import QtQuick.Layouts 1.15

Item
{
    id: root
    width: parent.width
    height: id_txtLine.height + 10

    property string _time: ""
    property string _mes: ""
    property bool   _selected: false

    signal selectClicked()
    signal copyClicked()
    signal copyMessageClicked()

    Rectangle
    {
        anchors.fill: parent
        color: _selected ? Style.script.log.selectColor : Style.script.log.color
    }

    RowLayout
    {
        width: parent.width - 20
        anchors.centerIn: parent
        spacing: 10

        Text
        {
            Layout.preferredWidth: 110
            font.pixelSize: Style.text.label2.fontSize
            color: Style.text.label2.color
            text: _time
        }
        Text
        {
            id: id_txtLine
            Layout.fillWidth: true
            font.pixelSize: Style.text.label.fontSize
            color: Style.text.label.color
            text: _mes
            wrapMode: Text.Wrap
        }
    }   // RowLayout

    MouseArea
    {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked:
        {
            if (mouse.button === Qt.LeftButton) selectClicked()
            if (mouse.button === Qt.RightButton)
            {
                id_menu.x = mouse.x
                id_menu.y = mouse.y
                id_menu.open()
            }
        }
    }

    Menu
    {
        id: id_menu
        MenuItem
        {
            text: qsTr("Copy")
            onTriggered: copyClicked()
        }
        MenuItem
        {
            text: qsTr("Copy message")
            onTriggered: copyMessageClicked()
        }

    }   // Menu

}
