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

import CrcMode 1.0

Item
{
    id: root
    width: 400
    height: id_clmContent.height

    property var _crcCalc

    //-----------------------------------------------------------------

    QtObject
    {
        id: set
        property bool  _busy: _crcCalc.processing
        property var   _types: _crcCalc.crcTypes
        property int   _crcProfile: CrcSettings.CrcProfiles.CrcPredefined
        property color _labelStandartColor: _crcProfile === CrcSettings.CrcProfiles.CrcPredefined && !set._busy ?
                                              Style.text.label.color : Style.text.substring.color
        property color _labelCustomColor: _crcProfile === CrcSettings.CrcProfiles.CrcCustom && !set._busy ?
                                              Style.text.label.color : Style.text.substring.color
        property int   _labelFontSize: Style.text.label.fontSize
        property int   _labelWidth: 120

        on_TypesChanged:
        {
            id_modelCrcPredef.clear()
            for (var i = 0; i < _types.length; ++i)
            {
                var obj = {}
                obj.text = _types[i].name
                id_modelCrcPredef.append(obj)
            }
            id_boxCrcPredef.currentIndex = 0
        }
    }

    //-----------------------------------------------------------------

    enum CrcProfiles
    {
        CrcPredefined = 0,
        CrcCustom
    }

    //-----------------------------------------------------------------

    function currentConfig()
    {
        if (set._crcProfile === CrcSettings.CrcProfiles.CrcPredefined)
        {
            return set._types[id_boxCrcPredef.currentIndex]
        } else
        {
            var cfg = {}
            cfg.name = "Custom"
            cfg.poly = id_fldPoly.text
            cfg.init = id_fldInit.text
            cfg.xor = id_fldXor.text
            cfg.refin = id_chkRefIn.checked
            cfg.refout = id_chkRefOut.checked

            return cfg
        }
    }

    //-----------------------------------------------------------------


    ButtonGroup { id: id_grProfile }
    ButtonGroup { id: id_grMode }
    ListModel { id: id_modelCrcPredef; }

    ColumnLayout
    {
        id: id_clmContent
        width: parent.width
        spacing: 5
        enabled: !set._busy

        KPane
        {
            Layout.fillWidth: true
            height: id_rowCrc.height + 10
            RowLayout
            {
                id: id_rowCrc
                width: parent.width - 20
                anchors.centerIn: parent
                RadioButton
                {
                    id: id_rbCrc8
                    text: "CRC-8"
                    checked: true
                    ButtonGroup.group: id_grMode
                    onCheckedChanged: { if (checked) _crcCalc.setCrcMode(CrcMode.CRC8) }
                }
                RadioButton
                {
                    id: id_rbCrc16
                    text: "CRC-16"
                    ButtonGroup.group: id_grMode
                    onCheckedChanged: { if (checked) _crcCalc.setCrcMode(CrcMode.CRC16) }
                }
                RadioButton
                {
                    id: id_rbCrc32
                    text: "CRC-32"
                    ButtonGroup.group: id_grMode
                    onCheckedChanged: { if (checked) _crcCalc.setCrcMode(CrcMode.CRC32) }
                }
                RadioButton
                {
                    id: id_rbCrc64
                    text: "CRC-64"
                    ButtonGroup.group: id_grMode
                    onCheckedChanged: { if (checked) _crcCalc.setCrcMode(CrcMode.CRC64) }

                }

            }   // RowLayout
        }    // Rectangle

        KPane
        {
            Layout.fillWidth: true
            height: id_rowType.height + 10
            RowLayout
            {
                id: id_rowType
                width: parent.width - 20
                anchors.centerIn: parent
                RadioButton
                {
                    checked: true
                    text: qsTr("Predefined")
                    ButtonGroup.group: id_grProfile
                    font.pixelSize: 16
                    onCheckedChanged: { if (checked) set._crcProfile = CrcSettings.CrcProfiles.CrcPredefined }
                }
                RadioButton
                {
                    text: qsTr("Custom")
                    ButtonGroup.group: id_grProfile
                    font.pixelSize: 16
                    onCheckedChanged: { if (checked) set._crcProfile = CrcSettings.CrcProfiles.CrcCustom }
                }
            }   // RowLayout
        }    // Rectangle

        KPane
        {
            Layout.fillWidth: true
            height: id_rowPredefined.height + 20

            RowLayout
            {
                id: id_rowPredefined
                width: parent.width - 20
                anchors.centerIn: parent
                spacing: 20
                enabled: set._crcProfile === CrcSettings.CrcProfiles.CrcPredefined ? true : false
                Text
                {
                    Layout.preferredWidth: set._labelWidth
                    text: qsTr("Predefined")
                    color: set._labelStandartColor
                    font.pixelSize: set._labelFontSize
                }
                ComboBox
                {
                    id: id_boxCrcPredef
                    Layout.fillWidth: true
                    model: id_modelCrcPredef
                }
            }
        }

        KPane
        {
            Layout.fillWidth: true
            height: id_colCustom.height + 20

            Column
            {
                id: id_colCustom
                width: parent.width - 20
                anchors.centerIn: parent
                spacing: 5
                enabled: set._crcProfile === CrcSettings.CrcProfiles.CrcCustom ? true : false

                RowLayout
                {
                    width: parent.width
                    spacing: 5
                    Text
                    {
                        Layout.preferredWidth: set._labelWidth
                        text: qsTr("Polynom")
                        color: set._labelCustomColor
                        font.pixelSize: set._labelFontSize
                    }
                    Text
                    {
                        text: "0x"
                        color: set._labelCustomColor
                        font.pixelSize: set._labelFontSize
                    }
                    TextField
                    {
                        id: id_fldPoly
                        Layout.fillWidth: true
                        selectByMouse: true
                        placeholderText: "0"
                        validator: id_regExpHex
                        text: id_boxCrcPredef.currentIndex < 0 ? "": set._types[id_boxCrcPredef.currentIndex].poly
                    }
                }   // RowLayout
                RowLayout
                {
                    width: parent.width
                    spacing: 5
                    Text
                    {
                        Layout.preferredWidth: set._labelWidth
                        text: qsTr("Init value")
                        color: set._labelCustomColor
                        font.pixelSize: set._labelFontSize
                    }
                    Text
                    {
                        text: "0x"
                        color: set._labelCustomColor
                        font.pixelSize: set._labelFontSize
                    }
                    TextField
                    {
                        id: id_fldInit
                        Layout.fillWidth: true
                        selectByMouse: true
                        placeholderText: "0"
                        validator: id_regExpHex
                        text: id_boxCrcPredef.currentIndex < 0 ? "": set._types[id_boxCrcPredef.currentIndex].init
                    }
                }   // RowLayout

                RowLayout
                {
                    width: parent.width
                    spacing: 5
                    Text
                    {
                        Layout.preferredWidth: set._labelWidth
                        text: qsTr("XOR value")
                        color: set._labelCustomColor
                        font.pixelSize: set._labelFontSize
                    }
                    Text
                    {
                        text: "0x"
                        color: set._labelCustomColor
                        font.pixelSize: set._labelFontSize
                    }
                    TextField
                    {
                        id: id_fldXor
                        Layout.fillWidth: true
                        selectByMouse: true
                        placeholderText: "0"
                        validator: id_regExpHex
                        text: id_boxCrcPredef.currentIndex < 0 ? "": set._types[id_boxCrcPredef.currentIndex].xor
                    }
                }   // RowLayout

                RowLayout
                {
                    width: parent.width
                    spacing: 20
                    CheckBox
                    {
                        id: id_chkRefIn;
                        text: "RefIn";
                        checked: id_boxCrcPredef.currentIndex < 0 ? false: set._types[id_boxCrcPredef.currentIndex].refin
                    }
                    CheckBox
                    {
                        id: id_chkRefOut;
                        text: "RefOut";
                        checked: id_boxCrcPredef.currentIndex < 0 ? false: set._types[id_boxCrcPredef.currentIndex].refout
                    }
                }   // RowLayout

            }  // Column
        }   // rc
    }    // ColumnLayout

    RegExpValidator
    {
        id: id_regExpHex
        regExp: /[0-9A-Fa-f]{1,16}/
    }

    Component.onCompleted:
    {
        if (_crcCalc.mode === CrcMode.CRC8) id_rbCrc8.checked = true
        if (_crcCalc.mode === CrcMode.CRC16) id_rbCrc16.checked = true
        if (_crcCalc.mode === CrcMode.CRC32) id_rbCrc32.checked = true
        if (_crcCalc.mode === CrcMode.CRC64) id_rbCrc64.checked = true
    }

}
