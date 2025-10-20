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
import "../../components"

Item
{
    id: root
    width: parent.width
    height: id_row.height + 10

    property var _bytesModel
    property var _output

    signal configChanged(var cfg)

    function setConfig(cfg)
    {
        if (Object.keys(cfg).length === 0) return

        set._cfgCompleted = false

        if (cfg.asciiChecked !== undefined) id_chkAscii.checked = cfg.asciiChecked
        if (cfg.hexChecked !== undefined) id_chkHex.checked = cfg.hexChecked
        if (cfg.newLineMode !== undefined) id_boxNewLine.currentIndex = cfg.newLineMode
        if (cfg.timeout !== undefined) _bytesModel.setTimeout(cfg.timeout)
        if (cfg.autoCount !== undefined) id_chkAutoCol.checked = cfg.autoCount
        if (cfg.count !== undefined) _bytesModel.setColumnCount(cfg.count)

        set._cfgCompleted = true
    }

    function updateConfig()
    {
        if (set._cfgCompleted === false) return

        var cfg = {}
        cfg.asciiChecked = id_chkAscii.checked
        cfg.hexChecked = id_chkHex.checked
        cfg.newLineMode = id_boxNewLine.currentIndex
        cfg.timeout = _bytesModel.getTimeout()
        cfg.autoCount = id_chkAutoCol.checked
        cfg.count = _bytesModel.getColumnCount()

        configChanged(cfg)
    }

    QtObject
    {
        id: set
        property bool _cfgCompleted: false
        property int  _timeout: _bytesModel.timeout
        property int  _count: _bytesModel.columnCount
    }

    KPane { anchors.fill: parent }

    Row
    {
        id: id_row
        width: parent.width - 40
        anchors.centerIn: parent
        spacing: 20

        KHeader
        {
            anchors.verticalCenter: parent.verticalCenter
            text: "RX"
        }

        Button
        {
            width: 40
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "qrc:/resources/images/icon/broom2_128.png"
            onClicked: _output.clear()

            ToolTip.delay: 1000
            ToolTip.timeout: 2000
            ToolTip.visible: hovered
            ToolTip.text: qsTr("Clear")
        }

        CheckBox
        {
            id: id_chkAscii
            anchors.verticalCenter: parent.verticalCenter
            text: "ASCII"
            checked: true
            onCheckedChanged:
            {
                if (id_chkHex.checked === false) checked = true
                _output._showAscii = checked
                updateConfig()
            }
        }
        CheckBox
        {
            id: id_chkHex
            anchors.verticalCenter: parent.verticalCenter
            text: "HEX"
            checked: true

            onCheckedChanged:
            {
                if (id_chkAscii.checked === false) checked = true
                _output._showHex = checked
                updateConfig()
            }
        }

        Column
        {
            spacing: 5
            KLabel
            {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("New Line")
            }
            ComboBox
            {
                id: id_boxNewLine
                width: 120
                model: _bytesModel.newLineModes
                onCurrentTextChanged:
                {
                    _bytesModel.setNewLineMode(currentIndex)
                    updateConfig()
                }
            }
        }
        Column
        {
            spacing: 5
            KLabel
            {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Timeout, ms")
            }
            TextField
            {
                id: id_fldTim
                Timer
                {
                    id: id_timTimeout
                    interval: 1000
                    onTriggered:
                    {
                        _bytesModel.setTimeout(parseInt(id_fldTim.text))
                        updateConfig()
                    }
                }
                enabled: id_boxNewLine.currentText === "Timeout" ? true : false

                width: 100
                selectByMouse: true
                text: set._timeout
                onTextChanged:
                {
                    if (set._cfgCompleted) id_timTimeout.restart()
                }
            } // TextField
        }   // Column

        Column
        {
            spacing: 5
            KLabel
            {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Column count")
            }

            Row
            {
                spacing: 5
                TextField
                {
                    id: id_fldCount
                    Timer
                    {
                        id: id_timCount
                        interval: 1000
                        onTriggered:
                        {
                            _bytesModel.setColumnCount(parseInt(id_fldCount.text))
                            updateConfig()
                        }
                    }
                    width: 60
                    enabled: !id_chkAutoCol.checked
                    selectByMouse: true
                    text: set._count
                    onTextChanged:
                    {
                        if (set._cfgCompleted) id_timCount.restart()
                    }
                }   // TextField
                CheckBox
                {
                    id: id_chkAutoCol
                    text: qsTr("Auto")
                    onCheckedChanged: _output._autoColumns = checked
                }
            }    // Row
        }   // Column
    }   // Row

    KLabel
    {
        anchors {
           right: parent.right
           top: parent.top
           rightMargin: 25
           topMargin: 10
        }
        text: _bytesModel.byteCount + " bytes"
    }

    Component.onCompleted:
    {
        set._cfgCompleted = true
    }

}
