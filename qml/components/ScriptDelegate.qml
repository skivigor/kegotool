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
import QtQuick.Controls.Universal 2.15
import QtQuick.Layouts 1.15

Item
{
    id: root
    width: parent.width - 20
    height: 50

    property string _name
    property var    _view
    property bool   _processing: false

    property var _style: Style.script.delegate

    signal editClicked()
    signal deleteClicked()

    Rectangle
    {
        id: id_rc
        anchors.fill: parent
        color: _style.color
        border.width: _style.borderWidth
        border.color: _style.borderColor

        Image
        {
            width: 16
            height: 16
            anchors.right: parent.right
            visible: _view.currentIndex === index ? true : false
            source: "qrc:/resources/images/icon/apply_128.png"
        }

        RowLayout
        {
            width: parent.width - 20
            anchors.centerIn: parent
            spacing: 10

            KLabel
            {
                text: _name
                Layout.fillWidth: true
                wrapMode: Text.Wrap
                color: _view.currentIndex === index ? Style.text.label2.color : Style.text.label.color
            }
            BusyIndicator
            {
                Layout.preferredWidth: 32
                Layout.preferredHeight: 32
                running: _view.currentIndex === index && _processing === true
            }
        }

        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true
            enabled: !_processing
            onEntered: id_rc.color = _style.hoverColor
            onExited: id_rc.color = _style.color
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onClicked:
            {
                if (mouse.button === Qt.LeftButton)
                {
                    if (_view.currentIndex === index) _view.currentIndex = -1
                    else _view.currentIndex = index
                }
                if (mouse.button === Qt.RightButton)
                {
                    _view.currentIndex = index
                    id_menu.open()
                }
            }
        }

    }   // Rectangle

    Menu
    {
        id: id_menu
        MenuItem
        {
            text: qsTr("Edit")
            onTriggered: editClicked()
        }
        MenuItem
        {
            text: qsTr("Delete")
            onTriggered: deleteClicked()
        }

    }   // Menu


}
