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
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

ApplicationWindow
{
    id: mainWindow
    width: 1280
    height: 800
    minimumWidth: 1100
    minimumHeight: 600
    visible: true
    title: SysInfo.appName + " " + SysInfo.appVersion

    //---------------------------------------------

    BorderImage { id: fon; anchors.fill: parent; source: Style.theme }

    header: ToolBar
    {
        contentHeight: toolButton.implicitHeight
        Row
        {
            spacing: 10
            ToolButton
            {
                id: toolButton
                text: "\u2630"
                font.pixelSize: Qt.application.font.pixelSize * 1.6
                onClicked: drawer.open()
            }
            Label
            {
                id: id_label
                anchors.verticalCenter: parent.verticalCenter
                text: stackView.currentItem.title
            }
        }

        Image
        {
            id: id_imgLogo
            height: parent.height - 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10
            opacity: 0.9
            fillMode: Image.PreserveAspectFit
            source: Style.logo
        }
    }   // header

    ListModel
    {
        id: id_model
        ListElement
        {
            name: qsTr("Uart")
            path: "qml/pages/Uart.qml"
        }
        ListElement
        {
            name: qsTr("CRC/Hash")
            path: "qml/pages/Crc.qml"
        }
        ListElement
        {
            name: qsTr("Settings")
            path: "qml/pages/Settings.qml"
        }
        ListElement
        {
            name: qsTr("About")
            path: "qml/pages/About.qml"
        }
    }   // ListModel

    Drawer
    {
        id: drawer
        width: mainWindow.width < 650 ? mainWindow.width * 0.4 : mainWindow.width * 0.2
        height: mainWindow.height

        ListView
        {
            id: id_list
            anchors.fill: parent
            clip: true
            model: id_model
            delegate: ItemDelegate {
                text: name
                font.pixelSize: Style.txtHeaderSize
                width: parent.width
                onClicked:
                {
                    var currentTitle = stackView.currentItem.title
                    var selectedName = name
                    currentTitle = currentTitle.replace(/\s/g, '')
                    selectedName = selectedName.replace(/\s/g, '')

                    if (currentTitle === selectedName) { drawer.close(); return }
                    if (stackView.depth > 1) stackView.pop()
                    stackView.push(path)
                    drawer.close()

                }
            }
        }   // ListView
    }   // Drawer

    StackView
    {
        id: stackView
        initialItem: "qml/pages/Uart.qml"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

}
