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
    width: 400
    height: id_clmContent.height

    property var _hashCalc

    //-----------------------------------------------------------------

    QtObject
    {
        id: set
        property bool   _busy: _hashCalc.processing
        property string _result: _hashCalc.hexResult
        property var    _types: _hashCalc.hashTypes
        property int    _toolTipTimeout: 2000
        property string _algorithmName: "..."

        property color  _labelColor: !_busy ? Style.text.label.color : Style.text.substring.color
        property int    _labelFontSize: Style.text.label.fontSize
        property int    _labelWidth: 120

        on_ResultChanged:
        {
            if (_result === "0x0") return
            _algorithmName = id_boxHashType.currentText
        }

    }

    //-----------------------------------------------------------------

    function currentConfig()
    {
        return id_boxHashType.currentText
    }

    //-----------------------------------------------------------------


    ButtonGroup { id: id_grProfile }
    ButtonGroup { id: id_grMode }
    ListModel { id: id_modelCrcPredef; }

    Column
    {
        id: id_clmContent
        width: parent.width
        spacing: 5
        enabled: !set._busy

        KPane
        {
            width: parent.width
            height: id_rowAlgorithm.height + 20

            RowLayout
            {
                id: id_rowAlgorithm
                width: parent.width - 20
                anchors.centerIn: parent
                spacing: 20
                Text
                {
                    Layout.preferredWidth: set._labelWidth
                    text: qsTr("Algorithm")
                    color: set._labelColor
                    font.pixelSize: set._labelFontSize
                }
                ComboBox
                {
                    id: id_boxHashType
                    Layout.fillWidth: true
                    model: set._types
                }
            }
        }   // rc

        KPane
        {
            width: parent.width
            height: id_clmResult.height + 20

            Column
            {
                id: id_clmResult
                width: parent.width - 10
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 10
                spacing: 20

                Row
                {
                    spacing: 10
                    Image { width: 24; height: 24; source: "qrc:/resources/images/icon/hash_512_2.png" }
                    Text
                    {
                        font.pixelSize: Style.text.header2.fontSize
                        color: Style.text.header2.color
                        text: qsTr("Result") + " (" + set._algorithmName + "), Hex: "
                    }
                }

                TextInput
                {
                    id: id_txtResult
                    width: parent.width
                    font.pixelSize: Style.checksum.fontSize
                    selectByMouse: true
                    readOnly: true
                    color: Style.checksum.color1
                    wrapMode: Text.WrapAnywhere
                    text: set._result

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
                            id_resToolTip.show("Copied to clipboard", set._toolTipTimeout)
                        }
                    }
                }   // TextInput
            }   // column
        }   // rc
    }    // ColumnLayout

}
