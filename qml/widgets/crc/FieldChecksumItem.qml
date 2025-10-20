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
    implicitHeight: id_clmContent.height + 30

    //-----------------------------------------------------------------

    property bool   _processing: false

    //-----------------------------------------------------------------

    enum InputMode
    {
        InputAsci = 0,
        InputHex
    }

    //-----------------------------------------------------------------

    QtObject
    {
        id: set
        property int _inputMode: FieldChecksumItem.InputMode.InputHex

        property string _defaultHex: "0102030405060708090A0B0C0D0E0F"
        property string _defaultAsci: "123456789"
    }

    //-----------------------------------------------------------------

    signal butCalcClicked(data: var)

    //-----------------------------------------------------------------

    ButtonGroup { id: id_grType }

    // Field
    KPane
    {
        anchors.fill: parent

        Column
        {
            id: id_clmContent
            width: parent.width - 10
            anchors.centerIn: parent
            spacing: 10
            Row
            {
                width: parent.width - 20
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 30
                enabled: !_processing

                RadioButton
                {
                    checked: true
                    text: "Hex"
                    ButtonGroup.group: id_grType
                    onCheckedChanged:
                    {
                        if (checked)
                        {
                            set._inputMode = FieldChecksumItem.InputMode.InputHex
                            id_fldInput.resetError()
                            id_fldInput.text = set._defaultHex
                        }
                    }
                }
                RadioButton
                {
                    text: "ASCII"
                    ButtonGroup.group: id_grType
                    onCheckedChanged:
                    {
                        if (checked)
                        {
                            set._inputMode = FieldChecksumItem.InputMode.InputAsci
                            id_fldInput.resetError()
                            id_fldInput.text = set._defaultAsci
                        }
                    }
                }
            }   // row

            ScrollView
            {
                width: parent.width - 20
                anchors.horizontalCenter: parent.horizontalCenter
                height: 100
                contentWidth: -1
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                clip: true

                TextArea
                {
                    id: id_fldInput

                    property bool _error: false

                    function setError() { _error = true; color = "red" }
                    function resetError() { color = Style.textArea.color; _error = false }

                    readOnly: _processing
                    selectByMouse: true
                    selectByKeyboard: true
                    wrapMode: TextEdit.Wrap
                    color: Style.textArea.color
                    text: set._defaultHex
                    onTextChanged: { if (_error) resetError() }
                }
            }   // ScrollView

            Button
            {
                width: 110
                anchors.right: parent.right
                anchors.rightMargin: 10
                enabled: !_processing
                text: qsTr("Calculate")

                onClicked:
                {
                    var txt = id_fldInput.text

                    if (set._inputMode === FieldChecksumItem.InputMode.InputHex)
                    {
                        // Remove spaces
                        txt = txt.replace(/\s/g, '');

                        // Check hex format
                        var reg = /^[0-9A-Fa-f]*$/
                        var match = reg.exec(txt)
                        console.log("Match " + match)
                        if (match === null) { id_fldInput.setError(); return }

                        // Emit
                        butCalcClicked(ByteUtils.byteArrayFromHexString(txt))
                    } else
                    {
                        // Emit
                        butCalcClicked(ByteUtils.byteArrayFromAsciString(txt))
                    }
                }
            }   // Button
        }   // column
    }   // rc

}
