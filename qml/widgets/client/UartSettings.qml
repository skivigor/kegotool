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
import "../../components"

Item
{
    id: root
    width: parent.width
    height: id_flow.height

    property var _uart

    signal configChanged(var cfg)

    function setConfig(cfg)
    {
        if (Object.keys(cfg).length === 0)  // empty config
        {
            id_boxBaud.currentIndex = id_boxBaud.count - 2   // set 115200
            return
        }

        if (cfg.baudIdx !== undefined) id_boxBaud.currentIndex = cfg.baudIdx
        if ((id_boxBaud.currentIndex === (id_boxBaud.count - 1)) && (cfg.baud !== undefined))
            id_boxBaudTxt._customTxt = cfg.baud
        if (cfg.dataIdx !== undefined) id_boxDataBits.currentIndex = cfg.dataIdx
        if (cfg.parityIdx !== undefined) id_boxParity.currentIndex = cfg.parityIdx
        if (cfg.stopIdx !== undefined) id_boxStopBits.currentIndex = cfg.stopIdx
        if (cfg.ctrlIdx !== undefined) id_boxCtrl.currentIndex = cfg.ctrlIdx
    }

    function updateConfig()
    {
        var baud = id_boxBaudTxt.text
        var baudIdx = id_boxBaud.currentIndex
        var data = id_boxDataBits.currentText
        var dataIdx = id_boxDataBits.currentIndex
        var parity = id_boxParity.currentText
        var parityIdx = id_boxParity.currentIndex
        var stop = id_boxStopBits.currentText
        var stopIdx = id_boxStopBits.currentIndex
        var ctrl = id_boxCtrl.currentText //.substring(0, 4)
        var ctrlIdx = id_boxCtrl.currentIndex

        var cfg = {}
        cfg.portInfo = baud + " / " + data + parity.charAt(0) + stop + " / " + ctrl
        cfg.baud = parseInt(baud)
        cfg.baudIdx = baudIdx
        cfg.dataBits = data
        cfg.dataIdx = dataIdx
        cfg.parity = parity
        cfg.parityIdx = parityIdx
        cfg.stopBits = stop
        cfg.stopIdx = stopIdx
        cfg.ctrl = ctrl
        cfg.ctrlIdx = ctrlIdx

        configChanged(cfg)
    }

    Flow
    {
        id: id_flow
        width: parent.width - 40
        spacing: 20
        anchors.centerIn: parent
        enabled: !_uart.openned

        // Baudrate
        Column
        {
            spacing: 5
            KLabel
            {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Baudrate")
            }
            ComboBox
            {
                id: id_boxBaud
                width: 100
                model: ["1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200", "Custom"]

                contentItem: TextInput {
                    id: id_boxBaudTxt
                    property string _customTxt: ""

                    leftPadding: 10
                    readOnly: !id_boxBaud.editable
                    selectByMouse: true
                    validator: IntValidator { bottom: 1; top: 999999 }
                    text: readOnly ? id_boxBaud.displayText : _customTxt
                    font: id_boxBaud.font
                    color: (id_boxBaud.pressed || !id_flow.enabled) ? Style.baud.text.color : Style.baud.text.selectColor
                    verticalAlignment: Text.AlignVCenter

                    onTextChanged:
                    {
                        if (!readOnly) _customTxt = text
                        updateConfig()
                    }
                }

                onCurrentIndexChanged:
                {
                    if (currentIndex === (count - 1)) editable = true
                    else editable = false
                }

                popup: Popup {
                    y: id_boxBaud.height - 1
                    width: id_boxBaud.width
                    implicitHeight: contentItem.implicitHeight
                    padding: 1

                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight
                        model: id_boxBaud.popup.visible ? id_boxBaud.delegateModel : null
                        currentIndex: id_boxBaud.highlightedIndex

                        ScrollIndicator.vertical: ScrollIndicator { }
                    }

                    background: Rectangle {
                        border.color: Style.baud.borderColor //"#909090"
                        radius: 2
                        color: Style.baud.color //"#303030"
                    }
                }   // Popup
            }   // ComboBox
        }   // column Baudrate

        // Data bits
        Column
        {
            spacing: 5
            KLabel
            {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Data bits")
            }
            ComboBox
            {
                id: id_boxDataBits
                width: 60
                model: _uart.dataBitsList
                onCurrentTextChanged: updateConfig()
            }
        }
        // Parity
        Column
        {
            spacing: 5
            KLabel
            {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Parity")
            }
            ComboBox
            {
                id: id_boxParity
                width: 90
                model: _uart.parityList
                onCurrentTextChanged: updateConfig()
            }
        }
        // Stop bits
        Column
        {
            spacing: 5
            KLabel
            {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Stop bits")
            }
            ComboBox
            {
                id: id_boxStopBits
                width: 70
                model: _uart.stopBitsList
                onCurrentTextChanged: updateConfig()
            }
        }
        // Control
        Column
        {
            spacing: 5
            KLabel
            {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Control")
            }
            ComboBox
            {
                id: id_boxCtrl
                width: 120
                model: _uart.ctrlList
                onCurrentTextChanged: updateConfig()
            }
        }

    }   // Flow

}
