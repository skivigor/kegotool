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
import "../../components"

import NewLineMode 1.0

Item
{
    id: root
    width: parent.width
    height: id_col.height + 10

    property var _iface
    property var _bytesModel
    property var _output

    property var _dictionaries: []
    property var _logs: []

    signal configChanged(var cfg)

    function setConfig(cfg)
    {
        if (Object.keys(cfg).length === 0) return

        set._cfgCompleted = false

        if (cfg.asciiChecked !== undefined) id_chkAscii.checked = cfg.asciiChecked
        if (cfg.hexChecked !== undefined) id_chkHex.checked = cfg.hexChecked
        if (cfg.inputMode !== undefined) id_boxInputMode.currentIndex = cfg.inputMode
        if (cfg.enterMode !== undefined) id_boxSendOnEnter.currentIndex = cfg.enterMode

        set._cfgCompleted = true
    }

    function updateConfig()
    {
        if (set._cfgCompleted === false) return

        var cfg = {}
        cfg.asciiChecked = id_chkAscii.checked
        cfg.hexChecked = id_chkHex.checked
        cfg.inputMode = id_boxInputMode.currentIndex
        cfg.enterMode = id_boxSendOnEnter.currentIndex

        configChanged(cfg)
    }

    QtObject
    {
        id: set
        property bool _cfgCompleted: false

        function sendCommand()
        {
            var txt = id_cmpCmdLine._text
            if (txt.length === 0) return

            var data
            if (id_boxInputMode.currentIndex === 0)  // ascii
            {
                data = ByteUtils.byteArrayFromAsciString(txt)
            } else  // hex
            {
                var hex = txt.replace(/\s/g, '');
                if ((hex.replace(/\s/g, '').length % 2) !== 0) return
                data = ByteUtils.byteArrayFromHexString(hex)
            }

            var ret = _iface.send(data, id_boxSendOnEnter.currentIndex)
            if (ret === false) return

            var dt = Qt.formatDateTime(new Date(), "dd.MM.yy  hh:mm:ss.zzz")
            var model = _logs[id_boxInputMode.currentIndex]
            model.addItem(dt, txt)
            id_cmpCmdLine.clear()
        }
    }

    KPane { anchors.fill: parent }

    Column
    {
        id: id_col
        width: parent.width - 40
        anchors.centerIn: parent
        spacing: 5
        Row
        {
            spacing: 20
            KHeader
            {
                anchors.verticalCenter: parent.verticalCenter
                text: "TX"
            }
            Button
            {
                width: 40
                icon.source: "qrc:/resources/images/icon/broom2_128.png"
                onClicked: _output.clear()

                ToolTip.delay: 1000
                ToolTip.timeout: 2000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Clear")
            }

            Button
            {
                width: 40
                icon.source: "qrc:/resources/images/icon/history_128.png"

                function onSelected(cmd) { id_cmpCmdLine._text = cmd }
                function onAddToDictionary(cmd)
                {
                    console.log("onAddToDictionary: " + cmd)
                    var mode = id_boxInputMode.currentIndex
                    var popup = Qt.createComponent("../../popups/DictionaryPopup.qml").createObject(id_page,
                                                   { _model: _dictionaries[mode],
                                                     _mode: mode,
                                                     _text: cmd })
                    popup.selected.connect(onSelected)
                }

                onClicked:
                {
                    var mode = id_boxInputMode.currentIndex
                    var popup = Qt.createComponent("../../popups/HistoryPopup.qml").createObject(id_page,
                                                  { _model: _logs[mode],
                                                    _mode: mode })
                    popup.selected.connect(onSelected)
                    popup.addToDictionary.connect(onAddToDictionary)
                }

                ToolTip.delay: 1000
                ToolTip.timeout: 2000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("History")
            }   // Button
            Button
            {
                width: 40
                icon.source: "qrc:/resources/images/icon/dictionary_128.png"

                function onSelected(cmd) { id_cmpCmdLine._text = cmd }

                onClicked:
                {
                    var mode = id_boxInputMode.currentIndex
                    var popup = Qt.createComponent("../../popups/DictionaryPopup.qml").createObject(id_page,
                                                   { _model: _dictionaries[mode],
                                                     _mode: mode })
                    popup.selected.connect(onSelected)
                }

                ToolTip.delay: 1000
                ToolTip.timeout: 2000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Dictionary")
            }   // Button
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
        }   // row
        RowLayout
        {
            width: parent.width
            spacing: 20

            ComboBox
            {
                id: id_boxInputMode
                Layout.preferredWidth: 100
                model: ["ASCII", "HEX"]
                onCurrentIndexChanged:
                {
                    id_cmpCmdLine._model = _logs[id_boxInputMode.currentIndex]
                    id_cmpCmdLine.clear()
                    updateConfig()
                }

                ToolTip.delay: 1000
                ToolTip.timeout: 2000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Input mode")
            }
            CommandLine
            {
                id: id_cmpCmdLine
                Layout.fillWidth: true
                _validator: id_boxInputMode.currentIndex
                _model: _logs[id_boxInputMode.currentIndex]

                onApplied: set.sendCommand()
            }

            ComboBox
            {
                id: id_boxSendOnEnter
                Layout.preferredWidth: 90
                model: _iface.termByteList
                onCurrentIndexChanged: updateConfig()

                ToolTip.delay: 1000
                ToolTip.timeout: 2000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Send on enter")
            }
            Button
            {
                Layout.preferredWidth: 110
                text: qsTr("Send")
                onClicked: set.sendCommand()
            }   // Button
        }    // RowLayout
    }    // Column

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

    RegExpValidator
    {
        id: id_regExpHex
        regExp: /[0-9A-Fa-f\s]*/
    }
    RegExpValidator
    {
        id: id_regExpAscii
        regExp: /[ -~]*/
    }

    Component.onCompleted:
    {
        _output._autoColumns = true
        _bytesModel.setNewLineMode(NewLineMode.Timeout)
        set._cfgCompleted = true
    }



}
