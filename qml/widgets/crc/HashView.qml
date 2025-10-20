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
        property var    _hashCalc: checksum.createHashCalc("hash") //hashCalc
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

            KHeader { text: qsTr("Hash") }

            RowLayout
            {
                width: parent.width
                spacing: 10

                HashSettings
                {
                    id: id_cmpSett
                    Layout.preferredWidth: 400
                    Layout.alignment: Qt.AlignTop
                    _hashCalc: set._hashCalc
                }

                ChecksumData
                {
                    id: id_cmpData
                    Layout.fillWidth: true
                    _processing: set._hashCalc.processing

                    onButCalcFieldClicked: (data) =>
                    {
                        set._hashCalc.calculate(id_cmpSett.currentConfig(), data)
                    }

                    onButCalcFileClicked: (url) =>
                    {
                        set._hashCalc.calculate(id_cmpSett.currentConfig(), url)
                    }
                }   // CrcData
            }   // RowLayout

        }   // column
    }   // rc


}

