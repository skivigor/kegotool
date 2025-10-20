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
//import QtQuick.Layouts 1.15

import "../components"

PopupView
{
    id: root
    anchors.fill: parent

    property var _info: undefined

    QtObject
    {
        id: set
        property string _signedInt: ""
        property string _unsignedInt: ""

        property var _intTypes: ["int8", "int16", "int32", "int64"]
        property int _intTypeMode: -1

        function checkInteger()
        {
            if (_info === undefined) return

            var len = _info.size
            if (len > 8) return

            var wordLen = 8
            _intTypeMode = 3

            if (len === 1) { _intTypeMode = 0; wordLen = 1 }
            if (len === 2) { _intTypeMode = 1; wordLen = 2 }
            if (len > 2 && len <= 4) { _intTypeMode = 2; wordLen = 4 }

            _signedInt = ByteUtils.signedIntFromHex(_info.hex, id_rbLsb.checked)
            _unsignedInt = ByteUtils.unsignedIntFromHex(_info.hex, id_rbLsb.checked)
        }
    }

    ButtonGroup { id: id_grByteOrder }

    KPopupPane
    {
        id: id_rc
        width: parent.width - 100
        height: parent.height - 200
        anchors.centerIn: parent
        _header: qsTr("Info")
        onButCloseClicked: root.destroy()
    }

    Flickable
    {
        anchors.fill: id_rc
        anchors.margins: 5
        anchors.topMargin: 35
        clip: true
        contentHeight: id_clm.height;

        ScrollBar.vertical: ScrollBar {}

        Column
        {
            id: id_clm
            spacing: 10
            width: parent.width - 300
            anchors.centerIn: parent

            ByteInfoLine
            {
                _header: "Ascii: "
                _text: _info.ascii
            }
            ByteInfoLine
            {
                _header: "Hex: "
                _text: _info.hex
            }
            ByteInfoLine
            {
                _header: "Bin: "
                _text: _info.bin
            }
            Text { text: " " }  // separator
            Row
            {
                spacing: 20
                width: parent.width
                visible: set._intTypeMode >= 0 ? true : false
                Text { width: 150; font.pixelSize: 18; color: Style.text.header2.color; text: qsTr("Byte order") }
                RadioButton
                {
                    id: id_rbLsb
                    text: "LSB"
                    checked: true
                    ButtonGroup.group: id_grByteOrder
                    onCheckedChanged: { set.checkInteger() }
                }
                RadioButton
                {
                    id: id_rbMsb
                    text: "MSB"
                    ButtonGroup.group: id_grByteOrder
                    onCheckedChanged: { set.checkInteger() }
                }
            }

            ByteInfoLine
            {
                visible: set._intTypeMode >= 0 ? true : false
                _headerWidth: 150
                _header: set._intTypeMode >= 0 ? ("Signed " + set._intTypes[set._intTypeMode] + ": ") : ""
                _text: set._signedInt
            }
            ByteInfoLine
            {
                visible: set._intTypeMode >= 0 ? true : false
                _headerWidth: 150
                _header: set._intTypeMode >= 0 ? ("Unsigned " + set._intTypes[set._intTypeMode] + ": ") : ""
                _text: set._unsignedInt
            }

        }   // Column
    }

    Component.onCompleted:
    {
        set.checkInteger()
    }

}
