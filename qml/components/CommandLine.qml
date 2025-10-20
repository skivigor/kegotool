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

Item
{
    id: root
    width: parent.width
    height: id_fldCmd.height

    property int   _validator: 0   // 0 - Ascii, 1 - Hex
    property alias _text: id_fldCmd.text
    property var   _model: undefined
    property int   _cmdIdx: 0

    signal applied()

    function clear()
    {
        resetCmdIndex()
        id_fldCmd.clear()
    }

    function resetCmdIndex()
    {
        _cmdIdx = (_model === undefined) ? 0 : _model.size()
    }

    TextField
    {
        id: id_fldCmd
        width: parent.width
        selectByMouse: true
        placeholderText: qsTr("Command")
        validator: _validator === 0 ? id_regExpAscii : id_regExpHex

        onTextChanged:
        {
            if (id_fldCmd.validator == id_regExpAscii) return

            var len = id_fldCmd.length
            var pos = id_fldCmd.cursorPosition
            var prevSym = id_fldCmd.text.charAt(pos - 2)

            id_fldCmd.text = id_fldCmd.text.replace(/\s/g, '').replace(/([0-9a-fA-F]{2})/g,"$1" + " ").trim().toUpperCase()

            if (id_fldCmd.length > len && prevSym !== ' ')
                id_fldCmd.cursorPosition = pos + 1
            else
                id_fldCmd.cursorPosition = pos
        }

        onAccepted: applied()

        Keys.onDownPressed:
        {
            if (_model === undefined) return

            if (_cmdIdx < _model.size() - 1) _cmdIdx++
            text = _model.getCommand(_cmdIdx).fcmd
        }
        Keys.onUpPressed:
        {
            if (_model === undefined) return

            if (_cmdIdx > 0) _cmdIdx--
            text = _model.getCommand(_cmdIdx).fcmd
        }
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

    Component.onCompleted: resetCmdIndex()

}
