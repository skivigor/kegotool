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
import QtQuick.Layouts 1.15

import "../components"

Window
{
    id: root
    width: 500
    height: 600
    visible: true
    title: "Kego - Script log: " + _modeName.toUpperCase()

    property string _modeName: "undefined"
    property var    _logModel

    onVisibilityChanged:
    {
        if (set._posX < 0 || set._posY < 0) return

        root.x = set._posX
        root.y = set._posY
    }

    onXChanged: { set._posX = root.x }
    onYChanged: { set._posY = root.y }

    QtObject
    {
        id: set
        property int _posX: -1
        property int _posY: -1
    }

    BorderImage { anchors.fill: parent; source: Style.theme }

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 5

        Rectangle
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: Style.pane.color

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    console.log("Unselect clicked on top")
                    _logModel.unselect()
                }
            }

            RowLayout
            {
                width: parent.width - 20
                anchors.centerIn: parent
                spacing: 20
                KHeader
                {
                    Layout.fillWidth: true
                    text: qsTr("Log")
                }

                Button
                {
                    Layout.preferredWidth: 40
                    icon.source: "qrc:/resources/images/icon/select_all_128.png"
                    onClicked:
                    {
                        _logModel.select(0)
                        _logModel.select(id_list.count - 1)
                    }

                    ToolTip.delay: 1000
                    ToolTip.timeout: 2000
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Select all")
                }
                Button
                {
                    Layout.preferredWidth: 40
                    icon.source: "qrc:/resources/images/icon/broom2_128.png"
                    onClicked: _logModel.clear()

                    ToolTip.delay: 1000
                    ToolTip.timeout: 2000
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Clear")
                }
            }   // RowLayout
        }  // Rectangle

        Rectangle
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Style.pane.color

            ListView
            {
                id: id_list
                anchors.fill: parent
                spacing: 5
                clip: true
                model: _logModel

                delegate: ScriptLogDelegate {
                    width: id_list.width
                    _time: time
                    _mes: message
                    _selected: sel

                    onSelectClicked: _logModel.select(index)

                    onCopyClicked:
                    {
                        var data = _logModel.getData(index)
                        if (data.length === 0) return

                        var str = ""
                        for (var i = 0; i < data.length; ++i)
                            str += (data[i].time + " | " + data[i].message + "\n")

                        FsUtils.copyToClipboard(str)
                    }

                    onCopyMessageClicked:
                    {
                        var data = _logModel.getData(index)
                        if (data.length === 0) return

                        var str = ""
                        for (var i = 0; i < data.length; ++i)
                            str += (data[i].message + "\n")

                        FsUtils.copyToClipboard(str)
                    }
                }   // ScriptLogDelegate

                MouseArea
                {
                    anchors.bottom: parent.bottom
                    width: id_list.width
                    enabled: id_list.contentHeight < id_list.height
                    height: id_list.height - id_list.contentHeight
                    onClicked: _logModel.unselect()
                }

                onCountChanged:
                {
                    id_list.positionViewAtEnd()
                    id_list.currentIndex = id_list.count - 1
                }

                ScrollBar.vertical: ScrollBar { policy: ScrollBar.AlwaysOn }
            }   // ListView
        }   // Rectangle
    }   // ColumnLayout

}
