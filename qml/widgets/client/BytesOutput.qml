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
import "../../components"

Item
{
    id: root
    width: parent.width
    height: 150

    property var _bytesModel: null
    property bool _autoColumns: false

    property bool _showAscii: true
    property bool _showHex: true

    property int _cellWidth: 25 //60 // 25

    on_AutoColumnsChanged:
    {
        if (_autoColumns === true) autoColumnsNum()
    }

    function clear()
    {
        _bytesModel.clear()
    }

    function autoColumnsNum()
    {
        if (_bytesModel === null) return
        _bytesModel.setColumnCount(Math.floor((parent.width - 20) / (_cellWidth + 1) ))
    }

    Timer
    {
        id: id_timColumns
        interval: 200
        repeat: false
        onTriggered: autoColumnsNum()
    }

    KPane { anchors.fill: parent }

    Rectangle
    {
        anchors.fill: parent
        anchors.margins: 5
        color: Style.pane.color
        border.width: 1
        border.color: "#909090"

        Item
        {
            width: parent.width - 4
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 2
            height: 20

            HorizontalHeaderView
            {
                model: _bytesModel === null ? 0 : _bytesModel.columnCount // Math.floor((parent.width - 20) / 26)
                clip: true
                delegate: Rectangle {
                    implicitWidth: _cellWidth //60 // 25
                    implicitHeight: 20
                    color: "#707070"
                    Text
                    {
                        anchors.centerIn: parent
                        color: "#c0c0c0"
                        text: index + 1
                    }
                }
                syncView: id_tab
            }
        }   // item
    }    // Rectangle

    TableView
    {
        id: id_tab
        anchors.fill: parent
        anchors.margins: 7
        anchors.topMargin: 30
        bottomMargin: 10
        columnSpacing: 1
        rowSpacing: 1  // 5
        model: _bytesModel
        clip: true
        flickableDirection: Flickable.VerticalFlick

        ScrollBar.vertical: ScrollBar {}
        ScrollBar.horizontal: ScrollBar { /*policy: ScrollBar.AlwaysOn*/ }

        Timer
        {
            id: id_timLayout
            interval: 50
            repeat: false
            onTriggered: id_tab.forceLayout()
        }

        onWidthChanged:
        {
            if (_bytesModel === null || _autoColumns === false) return
            id_timColumns.restart()
        }

        delegate: ByteDelegate {
            implicitWidth: _cellWidth
            implicitHeight: (_showAscii && _showHex) ? 50 : 25

            onImplicitHeightChanged: { id_timLayout.restart() }

            _valid: (model.valid === true && model.valid !== undefined) ? true : false
            _char: (model.chdata === undefined) ? "" : model.chdata
            _hex: (model.hxdata === undefined) ? "" : model.hxdata
            _selected: (model.sel === undefined) ? false : model.sel
            _msec: (model.msec === undefined) ? 0 : model.msec

            onSelectClicked: _bytesModel.select(row, column)
            onUnselectClicked: _bytesModel.unselect()

            onInfoClicked:
            {
                var data = _bytesModel.getData(row, column)
                Qt.createComponent("../../popups/BytesInfoPopup.qml").createObject(id_page, { _info: data })
            }

            onCopyAsCharClicked:
            {
                var data = _bytesModel.getData(row, column)
                FsUtils.copyToClipboard(data.ascii)
            }

            onCopyAsHexClicked:
            {
                var data = _bytesModel.getData(row, column)
                FsUtils.copyToClipboard(data.hex)
            }

            onCopyAsBinClicked:
            {
                var data = _bytesModel.getData(row, column)
                FsUtils.copyToClipboard(data.bin)
            }
        }

        onContentHeightChanged:
        {
            if (id_tab.contentHeight === 0) id_tab.contentY = 0
            if (id_tab.contentHeight > id_tab.height)
                id_tab.contentY = id_tab.contentHeight - id_tab.height + 20

            id_timLayout.restart()
        }

        MouseArea
        {
            width: id_tab.width
            height: id_tab.height
            onClicked: _bytesModel.unselect()
        }


    }   // TableView

//    Shortcut { sequences: ["Esc"]; onActivated: _bytesModel.unselect() }

//    Shortcut
//    {
//        sequences: ["Esc"];
//        onActivated:
//        {
//            console.log("Esc clicked")
//            if (_bytesModel === null) return
//            _bytesModel.unselect()
//        }
//    }

}
