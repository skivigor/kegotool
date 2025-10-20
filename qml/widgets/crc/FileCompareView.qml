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
    height: id_clmContent.implicitHeight + 30

    QtObject
    {
        id: set

        property var _calc1: checksum.createHashCalc("file1")
        property var _calc2: checksum.createHashCalc("file2")

        property string _result1: _calc1.hexResult
        property string _result2: _calc2.hexResult

        on_Result1Changed: checkResult()
        on_Result2Changed: checkResult()

        function checkResult()
        {
            if (_result1 === "0x0" && _result2 === "0x0") return

            var color = Style.checksum.color1
            var mes = "?"
            if (_result1 === "0x0" || _result2 === "0x0")
            {
                color = Style.checksum.color1
                mes = "?"
            } else if (_result1 === _result2)
            {
                color = Style.checksum.color2 //"lightgreen"
                mes = "=="
            } else
            {
                color = Style.checksum.color3
                mes = "!="
            }

            id_cmpFile1.setColor(color)
            id_cmpFile2.setColor(color)
            id_cmpStat.setStatus(color, mes)
        }

    }   // QtObject

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

            KHeader { text: qsTr("Comparation") }

            RowLayout
            {
                width: parent.width
                spacing: 10

                FileChecksumItem
                {
                    id: id_cmpFile1
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    _title: qsTr("File 1")
                    _processing: set._calc1.processing
                    _result: set._calc1.hexResult
                    _butCalcEnabled: false

                    onSelectedFirstUrl: (url) => { set._calc1.calculate("Sha256", url) }
                    onSelectedSecondUrl: (url) => { id_cmpFile2.setUrl(url) }
                }

                FileCompareStatus
                {
                    id: id_cmpStat
                    Layout.preferredWidth: 70
                    Layout.fillHeight: true
                    _busy: set._calc1.processing || set._calc2.processing

                    onButResetClicked:
                    {
                        if (set._calc1.processing || set._calc2.processing) return

                        set._calc1.resetResult()
                        set._calc2.resetResult()
                        id_cmpFile1.reset()
                        id_cmpFile2.reset()
                        id_cmpStat.reset()
                    }
                }

                FileChecksumItem
                {
                    id: id_cmpFile2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    _title: qsTr("File 2")
                    _processing: set._calc2.processing
                    _result: set._calc2.hexResult
                    _butCalcEnabled: false

                    onSelectedFirstUrl: (url) => { set._calc2.calculate("Sha256", url) }
                    onSelectedSecondUrl: (url) => { id_cmpFile1.setUrl(url) }
                }

            }   // RowLayout
        }   // Column
    }   // Rectangle

}
