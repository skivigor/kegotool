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
    height: id_row.height

    property int     _headerWidth: 100
    property string  _header: ""
    property string  _text: ""

    QtObject
    {
        id: set
        property int    _pixelSize: Style.text.header2.fontSize
        property string _headerColor: Style.text.header2.color
        property string _labelColor: Style.text.label3.color
    }

    Row
    {
        id: id_row
        spacing: 20
        width: parent.width
        Text { width: _headerWidth; font.pixelSize: set._pixelSize; color: set._headerColor; text: _header }
        TextInput
        {
            id: id_txtInput
            width: (id_txtMetric.width < parent.width - 150) ? (id_txtMetric.width + 30) : (parent.width - 150)
            font.pixelSize: set._pixelSize
            readOnly: true
            color: set._labelColor
            wrapMode: Text.Wrap
            text: _text

            MouseArea
            {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked:
                {
                    id_txtInput.selectAll()
                    id_txtInput.copy()
                    id_txtInput.deselect()
                    id_toolTip.x = mouseX
                    id_toolTip.y = mouseY
                    id_toolTip.show(qsTr("Copied to clipboard"), 2000)
                }
            }   // MouseArea
        }   // TextInput
    }   // row

    TextMetrics
    {
        id: id_txtMetric
        font: id_txtInput.font
        text: id_txtInput.text
    }

    ToolTip
    {
        id: id_toolTip
        parent: id_txtInput
    }

}
