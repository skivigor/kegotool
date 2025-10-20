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

    property var   _model
    property alias _text: id_cmpCmdLine._text
    property alias _mode: id_cmpCmdLine._validator

    signal selected(string cmd)

    QtObject
    {
        id: set

        function add()
        {
            if (id_fldDescr.text.length === 0 || id_cmpCmdLine._text.length === 0) return
            if (_mode === 1 && (id_cmpCmdLine._text.replace(/\s/g, '').length % 2) !== 0) return

            _model.addItem(id_fldDescr.text, id_cmpCmdLine._text)
        }
    }

    KPopupPane
    {
        id: id_rc
        width: parent.width - 100
        height: parent.height - 100
        anchors.centerIn: parent
        _header: qsTr("Dictionary") + " (" + (_mode === 0 ? "Ascii" : "Hex") + ")"
        onButCloseClicked: root.destroy()

        KPane
        {
            anchors.top: parent.top
            anchors.topMargin: 30
            width: parent.width
            height: 90

            Column
            {
                width: parent.width - 20
                anchors.centerIn: parent
                spacing: 5

                Row
                {
                    spacing: 20
                    anchors.left: parent.left
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
                }   // row

                RowLayout
                {
                    width: parent.width
                    spacing: 10

                    TextField
                    {
                        id: id_fldDescr
                        Layout.preferredWidth: 250
                        selectByMouse: true
                        placeholderText: qsTr("Description")
                        onAccepted: set.add()
                    }
                    CommandLine
                    {
                        id: id_cmpCmdLine
                        Layout.fillWidth: true
                        onApplied: set.add()
                    }
                    Button
                    {
                        Layout.preferredWidth: 110
                        text: qsTr("Add")
                        onClicked: set.add()
                        Keys.onEnterPressed: set.add()
                        Keys.onReturnPressed: set.add()

                    }
                }   // RowLayout
            }   // Column
        }   // Rectangle

        ListView
        {
            id: id_listview
            anchors.fill: parent
            anchors.margins: 5
            anchors.leftMargin: 10
            anchors.topMargin: 125
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
                        Layout.preferredWidth: 250
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
                        icon.source: "qrc:/resources/images/icon/delete2_128.png"
                        onClicked: _model.removeItem(index)

                        ToolTip.delay: 1000
                        ToolTip.timeout: 2000
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Delete")
                    }
                    Button
                    {
                        Layout.preferredWidth: 30
                        enabled: index < (id_listview.count - 1)
                        icon.source: "qrc:/resources/images/icon/arrow_down_128.png"
                        onClicked: _model.moveDown(index)

                        ToolTip.delay: 1000
                        ToolTip.timeout: 2000
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Move down")
                    }
                    Button
                    {
                        Layout.preferredWidth: 30
                        enabled: index !== 0
                        icon.source: "qrc:/resources/images/icon/arrow_up_128.png"
                        onClicked: _model.moveUp(index)

                        ToolTip.delay: 1000
                        ToolTip.timeout: 2000
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Move up")
                    }
                }   // RowLayout
            }   // Rectangle delegate

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

    Component.onCompleted: console.log("DictionaryPopup ctor")
    Component.onDestruction: console.log("DictionaryPopup dtor")


}
