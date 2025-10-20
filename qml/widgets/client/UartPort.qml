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

Item
{
    id: root
    width: parent.width
    height: id_row.height + 5

    property var _uart
    property var _portConfig

    QtObject
    {
        id: set
        property var _portInfoList: _uart.portInfoList

        on_PortInfoListChanged:
        {
            portModel.clear()

            for (var i = 0; i < _portInfoList.length; ++i)
                portModel.append({ "name": _portInfoList[i].name })

            id_boxPort.currentIndex = 0
        }
    }

    ListModel { id: portModel }

    Row
    {
        id: id_row
        width: parent.width - 40
        anchors.centerIn: parent
        spacing: 20

        Image
        {
            source: _uart.openned ? "qrc:/resources/images/icon/net_online_32.png"
                                  : "qrc:/resources/images/icon/net_offline_32.png"
        }

        ComboBox
        {
            id: id_boxPort
            width: 140
            model: portModel //_uart.portList
            enabled: !_uart.openned
        }

        Button
        {
            width: 40
            icon.source: "qrc:/resources/images/icon/refresh_dark2_128.png"
            enabled: !_uart.openned
            onClicked: _uart.updatePortList()

            ToolTip.delay: 1000
            ToolTip.timeout: 2000
            ToolTip.visible: hovered
            ToolTip.text: qsTr("Update")
        }

        Button
        {
            width: 40
            icon.source: "qrc:/resources/images/icon/info_128.png"
            enabled: !_uart.openned

            onClicked:
            {
                var port = set._portInfoList[id_boxPort.currentIndex]
                console.log("Port: " + JSON.stringify(port))
                var portName = port.name
                var manuf = port.manufcturer
                var pid = port.pid + " (0x" + port.pid.toString(16) + ")"
                var vid = port.vid + " (0x" + port.vid.toString(16) + ")"
                Qt.createComponent("../../popups/UartInfoPopup.qml").createObject(id_page,
                                  { _portName: portName,
                                    _manufacturer: manuf,
                                    _pid: pid,
                                    _vid: vid })

            }

            ToolTip.delay: 1000
            ToolTip.timeout: 2000
            ToolTip.visible: hovered
            ToolTip.text: qsTr("Information")
        }

        Button
        {
            width: 110
            text: _uart.openned ? qsTr("Close") : qsTr("Open")
            onClicked:
            {
                if (_portConfig === undefined || JSON.stringify(_portConfig) === "({})" ) return

                if (_uart.openned === true)
                {
                    _uart.close()
                } else
                {
                    _uart.open(id_boxPort.currentText,
                               _portConfig.baud,
                               _portConfig.dataBits,
                               _portConfig.parity,
                               _portConfig.stopBits,
                               _portConfig.ctrl)
                }
            }
        }

    }   // row

}
