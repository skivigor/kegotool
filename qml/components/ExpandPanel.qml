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

Item
{
    id: root
    width: parent.width
    height: id_col.height

    property alias _loader: id_ldr
    property alias _header: id_txtHeader.text
    property alias _info: id_txtInfo.text
    property alias _expanded: id_itm.visible

    KPane { anchors.fill: parent }

    Column
    {
        id: id_col
        width: parent.width
        spacing: 10

        KPane
        {
            id: id_rcPanel
            width: parent.width
            height: 40

            KHeader
            {
                id: id_txtHeader
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }

            Row
            {
                spacing: 20
                anchors.right: parent.right
                anchors.rightMargin: 15
                anchors.verticalCenter: parent.verticalCenter

                KLabel
                {
                    id: id_txtInfo
                    textFormat: Text.RichText
                    anchors.verticalCenter: parent.verticalCenter
                }

                KLabel
                {
                    text: _expanded ? String.fromCodePoint(0x1F861) : String.fromCodePoint(0x1F863) //"\u01f83b"
                    font.pixelSize: 22
                    font.bold: true
                }
            }

            MouseArea
            {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: id_rcPanel.color = Qt.lighter(Style.pane.color, 1.5)
                onExited: id_rcPanel.color = Style.pane.color
                onClicked: id_itm.visible = !id_itm.visible
            }
        }   // rc


        Item
        {
            id: id_itm
            visible: false
            width: parent.width - 10
            anchors.horizontalCenter: parent.horizontalCenter
            height: id_ldr.height + 10

            Loader
            {
                id: id_ldr
                width: parent.width
            }
        }   // itm
    }   // Column


    Behavior on height {
        NumberAnimation { duration: 100 }
    }

}
