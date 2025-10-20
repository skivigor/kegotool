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
import QtQuick.Dialogs 1.2
import "../../components"

Item
{
    id: root
    width: parent.width
    implicitHeight: id_clmContent.height + 30

    //-----------------------------------------------------------------

    property string _title: "File"
    property bool   _processing: false
    property string _result: ""
    property url    _url: ""
    property bool   _butCalcEnabled: true

    //-----------------------------------------------------------------

    signal selectedFirstUrl(var url)
    signal selectedSecondUrl(var url)

    //-----------------------------------------------------------------

    function setUrl(url)
    {
        if (_processing === true || FsUtils.isFile(url) === false) return

        _url = url
        id_txtFile.text = url
        selectedFirstUrl(url)
    }

    function setColor(color)
    {
        id_txtResult.color = color
    }

    function reset()
    {
        id_txtFile.text = qsTr("Drag here or select a file ...")
        id_txtResult.color = Style.checksum.color1
        _url = ""
    }

    //-----------------------------------------------------------------

    FileDialog
    {
        id: id_fileDialog
        title: "Please choose a file"
        nameFilters: [ "Data files (*)", "All files (*)" ]
        onAccepted: setUrl(fileUrls[0])
        onRejected: id_fileDialog.close()
    }

    KPane
    {
        id: id_rcFile
        anchors.fill: parent

        Column
        {
            id: id_clmContent
            width: parent.width - 10
            anchors.centerIn: parent
            spacing: 10

            KLabel
            {
                font.pixelSize: Style.text.header.fontSize
                font.bold: true
                text: _title
            }

            RowLayout
            {

                width: parent.width - 20
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 20

                KLabel
                {
                    id: id_txtFile
                    Layout.fillWidth: true
                    wrapMode: Text.WrapAnywhere
                    text: qsTr("Drag here or select a file ...")
                }
                Button
                {
                    Layout.preferredWidth: 110
                    enabled: !_processing
                    text: qsTr("Select")
                    onClicked: id_fileDialog.open()
                }
            }   // RowLayout
            RowLayout
            {

                width: parent.width - 20
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 20

                TextInput
                {
                    id: id_txtResult
                    Layout.fillWidth: true
                    font.pixelSize: Style.checksum.fontSize
                    selectByMouse: true
                    readOnly: true
                    color: Style.checksum.color1
                    wrapMode: Text.WrapAnywhere
                    text: _result

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
                            id_resToolTip.show(qsTr("Copied to clipboard"), 2000)
                        }
                    }
                }   // TextInput
                Item
                {
                    Layout.preferredWidth: 110
                    Layout.preferredHeight: 48

                    Button
                    {
                        width: parent.width
                        visible: !_processing && _butCalcEnabled
                        enabled: _url.toString().length > 0 ? true : false
                        text: qsTr("Calculate")
                        onClicked: setUrl(_url)
                    }
                    BusyIndicator
                    {
                        width: height
                        height: parent.height
                        anchors.centerIn: parent
                        running: _processing
                    }
                }   // Item
            }   // RowLayout

        }   // Column

        DropArea
        {
            anchors.fill: parent
            enabled: !_processing
            onEntered: (drag) => {
                           id_rcFile.border.color = Style.pane.dragBorderColor
                           drag.accept (Qt.LinkAction);
                       }
            onDropped: (drop) => {
                           id_rcFile.border.color = Style.pane.borderColor
                           setUrl(drop.urls[0])
                           if (drop.urls.length > 1) selectedSecondUrl(drop.urls[1])
                       }
            onExited: {
                id_rcFile.border.color = Style.pane.borderColor
            }
        }   // DropArea
    }   // Rectangle

    Component.onCompleted: reset()

}
