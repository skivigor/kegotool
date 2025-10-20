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
    height: id_clmContent.height + 30

    QtObject
    {
        id: set
        property var    _crcCalc: checksum.createCrcCalc("crc")
        property string _algorithmName: "..."
    }

    KPane
    {
        anchors.fill: parent
        anchors.margins: 5
        anchors.centerIn: parent

        Column
        {
            id: id_clmContent
            width: parent.width - 20
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            KHeader { text: "CRC" }

            RowLayout
            {
                width: parent.width
                spacing: 10

                CrcSettings
                {
                    id: id_cmpSett
                    Layout.preferredWidth: 400
                    Layout.alignment: Qt.AlignTop
                    _crcCalc: set._crcCalc
                }

                ChecksumData
                {
                    id: id_cmpData
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop
                    _processing: set._crcCalc.processing

                    onButCalcFieldClicked: (data) =>
                    {
                        var cfg = id_cmpSett.currentConfig()
                        set._algorithmName = cfg.name
                        set._crcCalc.calculate(cfg, data)
                    }

                    onButCalcFileClicked: (url) =>
                    {
                        var cfg = id_cmpSett.currentConfig()
                        set._algorithmName = cfg.name
                        set._crcCalc.calculate(cfg, url)
                    }
                }   // CrcData
            }   // RowLayout
            KPane
            {
                width: parent.width
                height: 50

                Row
                {
                    anchors.centerIn: parent
                    spacing: 30
                    Text
                    {
                        font.pixelSize: Style.text.header2.fontSize
                        anchors.verticalCenter: parent.verticalCenter
                        color: Style.text.header2.color
                        text: qsTr("Result (") + set._algorithmName + "), Hex: "
                    }
                    TextInput
                    {
                        id: id_txtResult
                        font.pixelSize: Style.checksum.fontSize
                        selectByMouse: true
                        readOnly: true
                        color: Style.checksum.color1
                        text: set._crcCalc.hexResult

                        ToolTip { id: id_resToolTip; parent: id_txtResult }

                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked:
                            {
                                id_txtResult.selectAll()
                                id_txtResult.copy()
                                id_txtResult.deselect()
                                id_resToolTip.show("Copied to clipboard", 2000)
                            }
                        }
                    }
                }   // row
            }   // pane
        }   // column
    }   // rc


}

