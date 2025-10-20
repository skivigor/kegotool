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

import "../components"

PopupView
{
    id: root
    anchors.fill: parent

    property var _model
    property int _mode: 0

    signal selected(string cmd)
    signal addToDictionary(string cmd)

    KPopupPane
    {
        id: id_rc
        width: parent.width - 100
        height: parent.height - 100
        anchors.centerIn: parent
        _header: qsTr("History") + " (" + (_mode === 0 ? "Ascii" : "Hex") + ")"
        onButCloseClicked: root.destroy()

        KPane
        {
            anchors.top: parent.top
            anchors.topMargin: 30
            width: parent.width
            height: 45

            Row
            {
                spacing: 20
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                CheckBox
                {
                    id: id_chkCloseOnCopy
                    checked: true
                    text: qsTr("Close on Copy")
                }
                Button
                {
                    width: 40
                    icon.source: "qrc:/resources/images/icon/broom2_128.png"
                    onClicked: _model.clear()

                    ToolTip.delay: 1000
                    ToolTip.timeout: 2000
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Clear")
                }

            }
        }   // KPane

        ListView
        {
            id: id_listview
            anchors.fill: parent
            anchors.margins: 5
            anchors.leftMargin: 10
            anchors.topMargin: 80
            model: _model
            spacing: 2
            clip: true

            delegate: Rectangle {
                id: id_rcDelegate
                width: id_listview.width
                height: id_row.height + 10
                color: "transparent"

                MouseArea
                {
                    id: id_mouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: id_rcDelegate.color = Style.popup.hoverColor
                    onExited: id_rcDelegate.color = "transparent"
                }

                RowLayout
                {
                    id: id_row
                    width: parent.width - 50
                    anchors.centerIn: parent
                    spacing: 10
                    Text
                    {
                        Layout.preferredWidth: 200
                        color: Style.text.label2.color
                        font.pixelSize: Style.text.label2.fontSize
                        text: descr
                    }
                    Text
                    {
                        id: id_txtCmd
                        Layout.fillWidth: true
                        color: Style.text.label.color
                        font.pixelSize: Style.text.label.fontSize
                        wrapMode: Text.Wrap
                        text: cmd

                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:
                            {
                                id_toolTipMouse.x = mouseX
                                id_toolTipMouse.y = mouseY
                                id_toolTipMouse.show(qsTr("Copied to command line"), 1000)
                                selected(id_txtCmd.text)
                                if (id_chkCloseOnCopy.checked) root.destroy()
                            }
                        }   // MouseArea

                        ToolTip { id: id_toolTipMouse }
                    }

                    Button
                    {
                        Layout.preferredWidth: 40
                        Layout.preferredHeight: 30
                        icon.source: "qrc:/resources/images/icon/copy_128.png"

                        onClicked:
                        {
                            id_toolTipBut.show(qsTr("Copied to command line"), 1000)
                            selected(id_txtCmd.text)
                            if (id_chkCloseOnCopy.checked) root.destroy()
                        }

                        ToolTip.delay: 1000
                        ToolTip.timeout: 2000
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Copy to command line")

                        ToolTip { id: id_toolTipBut }
                    }
                    Button
                    {
                        Layout.preferredWidth: 40
                        Layout.preferredHeight: 30
                        icon.source: "qrc:/resources/images/icon/dictionary_128.png"
                        onClicked: addToDictionary(id_txtCmd.text)

                        ToolTip.delay: 1000
                        ToolTip.timeout: 2000
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Add to dictionary")
                    }
                    Button
                    {
                        Layout.preferredWidth: 40
                        Layout.preferredHeight: 30
                        icon.source: "qrc:/resources/images/icon/delete2_128.png"
                        onClicked: _model.removeItem(index)

                        ToolTip.delay: 1000
                        ToolTip.timeout: 2000
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Delete")
                    }
                }   // RowLayout
            }   // Rectangle

            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AlwaysOn }

            onCountChanged:
            {
                positionViewAtEnd()
                currentIndex = count - 1
            }

            Component.onCompleted:
            {
                positionViewAtEnd()
                currentIndex = count - 1
            }

        }   // ListView

    }   // Rectangle

    Component.onCompleted: console.log("HistoryPopup ctor")
    Component.onDestruction: console.log("HistoryPopup dtor")


}
