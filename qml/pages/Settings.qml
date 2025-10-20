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
import "../components"

Page
{
    id: id_pageTerm
    title: qsTr("Settings")

    property var _menu: [
        {
            "name": qsTr("General"),
            "widget": "../widgets/settings/ConfigGeneralSettings.qml"
        }
    ]

    QtObject
    {
        id: set
        property bool _selecting: false
    }

    KPane
    {
        width: 980
        height: parent.height - 40
        anchors.centerIn: parent

        RowLayout
        {
            id: id_row
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            ListView
            {
                id: id_menuList
                Layout.preferredWidth: 150
                Layout.fillHeight: true
                spacing: 10
                clip: true
                model: _menu
                delegate: Text {
                    width: parent.width
                    color: Style.text.label3.color
                    leftPadding: 20
                    font.pixelSize: Style.text.label3.fontSize
                    font.bold: (index === id_menuList.currentIndex)
                    font.underline: (index === id_menuList.currentIndex)
                    text: modelData.name

                    MouseArea
                    {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked:
                        {
                            set._selecting = true
                            id_menuList.currentIndex = index
                            id_contentList.currentIndex = index
                        }
                    }
                }
            }   // ListView

            Rectangle
            {
                Layout.preferredWidth: 2
                Layout.fillHeight: true
                color: "#909090"
            }

            ListView
            {
                id: id_contentList
                Layout.fillWidth: true
                Layout.fillHeight: true
                highlightMoveDuration: 500

                Timer
                {
                    id: id_tim
                    interval: 100
                    repeat: false
                    running: false
                    onTriggered:
                    {
                        if (set._selecting === true) { set._selecting = false; return }

                        var pos = id_contentList.indexAt(id_contentList.contentX, id_contentList.contentY + id_contentList.height / 3)
                        id_menuList.currentIndex = pos
                    }
                }

                onContentYChanged: id_tim.restart()

                spacing: 10
                clip: true
                model: _menu
                delegate: Item {
                    id: id_itm
                    width: id_contentList.width
                    height: id_ldr.height

                    Loader
                    {
                        id: id_ldr
                        width: parent.width
                        height: item.height
                        source: modelData.widget
                    }
                }
            }   // ListView
        }   // RowLayout
    }   // KPane

    Component.onCompleted: console.log("Settings ctor")
    Component.onDestruction:  console.log("Settings dtor")
}
