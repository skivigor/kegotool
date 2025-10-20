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
import "../widgets/client"

Page
{
    id: id_page
    title: qsTr("Uart")

    QtObject
    {
        id: set
        property string _type: "uart"
        property string _keyPortSettings: "portSettings"
        property string _keyRxSettings: "rxSettings"
        property string _keyTxSettings: "txSettings"

        property var    _uart: client.createSerialClient(_type) //serial
        property string _uartStatus: _uart.status
        property string _portInfo: ""

        property var _rxModel: _uart.getRxModel()
        property var _txModel: _uart.getTxModel()

        property var _dictionaries: [
            client.createCommandModel("tuartdictascii"),
            client.createCommandModel("tuartdicthex")
        ]
        property var _logs: [
            client.createCommandModel("tuartlogascii"),
            client.createCommandModel("tuartloghex")
        ]

        property var _script: script.createScriptEngine(_type, _uart)
    }

    ExpandPanel
    {
        id: id_wPort
        width: parent.width - 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 5
        _expanded: true
        _info: set._portInfo + " :: " + set._uartStatus
        UartPort
        {
            id: id_cmpUartPort
            _uart: set._uart
        }

        function onConfigChanged(cfg)
        {
            set._portInfo = cfg.portInfo
            id_cmpUartPort._portConfig = cfg

            var ret = cfgstor.write(set._type, set._keyPortSettings, cfg)
        }

        Component.onCompleted:
        {
            _loader.setSource("../widgets/client/UartSettings.qml", { _uart: set._uart })
            var obj = _loader.item
            obj.configChanged.connect(onConfigChanged)

            var cfg = cfgstor.read(set._type, set._keyPortSettings)
            obj.setConfig(cfg)
        }
    }

    Flickable
    {
        anchors
        {
            top: id_wPort.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 5
        }
        clip: true
        contentHeight: id_clm.height;

        ScrollBar.vertical: ScrollBar {}

        RowLayout
        {
            id: id_clm
            spacing: 5
            width: id_page.width - 10
            height: id_page.height > 600 ? (id_page.height - id_wPort.height - 20) : 500

            ColumnLayout
            {
                Layout.fillWidth: true
                spacing: 5

                ClientRxParameters
                {
                    id: id_rxParam
                    Layout.fillWidth: true
                    _bytesModel: set._rxModel
                    _output: id_rxOutput

                    onConfigChanged:
                    {
                        cfgstor.write(set._type, set._keyRxSettings, cfg)
                    }

                    Component.onCompleted:
                    {
                        var cfg = cfgstor.read(set._type, set._keyRxSettings)
                        id_rxParam.setConfig(cfg)
                    }
                }
                BytesOutput
                {
                    id: id_rxOutput
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    _bytesModel: set._rxModel
                }
                ClientTxParameters
                {
                    id: id_txParam
                    Layout.fillWidth: true
                    _iface: set._uart
                    _bytesModel: set._txModel
                    _output: id_txOutput
                    _dictionaries: set._dictionaries
                    _logs: set._logs

                    onConfigChanged:
                    {
                        cfgstor.write(set._type, set._keyTxSettings, cfg)
                    }

                    Component.onCompleted:
                    {
                        var cfg = cfgstor.read(set._type, set._keyTxSettings)
                        id_txParam.setConfig(cfg)
                    }
                }
                BytesOutput
                {
                    id: id_txOutput
                    Layout.fillWidth: true
                    _bytesModel: set._txModel
                }
            }

            Scripts
            {
                Layout.preferredWidth: 250
                Layout.fillHeight: true
                _typeName: set._type
                _engine: set._script
            }

        }    // RowLayout
    }   // Flickable

    Shortcut { sequences: ["Esc"]; onActivated: set._rxModel.unselect() }

    Component.onCompleted:
    {
        console.log("Uart ctor")
        if (cfgstor.isTypeValid(set._type) === false) cfgstor.createType(set._type)
    }

    Component.onDestruction:  console.log("Uart dtor")
}
