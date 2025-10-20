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

    property string _typeName
    property var    _engine

    function addScript(name)
    {
        var path = set._fileModel.addItem(name + ".js")
        if (path.length > 0) FsUtils.openWithApp(path)
    }

    QtObject
    {
        id: set
        property var  _fileModel: _engine.getFileModel()
        property bool _processing: _engine.processing
        property var  _logWindow: null

        function openLogWindow()
        {
            if (_logWindow !== null)
            {
                _logWindow.show()
                _logWindow.requestActivate()
                return
            }

            _logWindow = Qt.createComponent("../../popups/ScriptLogWindow.qml")
                               .createObject(mainWindow, { _modeName: _typeName, _logModel: _engine.getLogModel() })
            _logWindow.show()
        }

    }

    KPane { anchors.fill: parent }

    ColumnLayout
    {
        width: parent.width - 20
        height: parent.height - 10
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 5

        KHeader
        {
            Layout.fillWidth: true
            text: qsTr("Scripts")
        }

        ListView
        {
            id: id_list
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 5
            model: set._fileModel
            delegate: ScriptDelegate {
                _name: fileName
                _view: id_list
                _processing: set._processing

                onEditClicked: FsUtils.openWithApp(filePath)
                onDeleteClicked: set._fileModel.delItem(fileName)
            }

            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AlwaysOn }

            Component.onCompleted: currentIndex = -1
        } /// ListView

        RowLayout
        {
            Layout.fillWidth: true
            spacing: 10
            CheckBox
            {
                id: id_chkAutoLogWindow
                Layout.preferredWidth: 90
                checked: true
                text: qsTr("Auto")

                ToolTip.delay: 1000
                ToolTip.timeout: 2000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Auto open log window")
            }
            Button
            {
                Layout.fillWidth: true
                text: qsTr("Log")
                onClicked: set.openLogWindow()

                ToolTip.delay: 1000
                ToolTip.timeout: 2000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Open log window")
            }
        }

        RowLayout
        {
            Layout.fillWidth: true
            spacing: 10

            Button
            {
                Layout.preferredWidth: 40
                icon.source: "qrc:/resources/images/icon/refresh_dark2_128.png"
                enabled: !set._processing
                onClicked: set._fileModel.updateModel()

                ToolTip.delay: 1000
                ToolTip.timeout: 2000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Update")
            }

            Button
            {
                Layout.preferredWidth: 40
                icon.source: "qrc:/resources/images/icon/add4_128.png"
                enabled: !set._processing
                onClicked:
                {
                    var popup = Qt.createComponent("../../popups/ScriptAddPopup.qml").createObject(id_page)
                    popup.addScript.connect(addScript)
                }

                ToolTip.delay: 1000
                ToolTip.timeout: 2000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Add script")
            }
            Button
            {
                Layout.fillWidth: true
                text: set._processing === false ? qsTr("Run") : qsTr("Stop")
                enabled: id_list.currentIndex !== -1
                onClicked:
                {
                    if (set._processing === true)
                    {
                        _engine.stopScript()
                    } else
                    {
                        _engine.executeScript(id_list.currentIndex)
                        if (id_chkAutoLogWindow.checked === true) set.openLogWindow()
                    }
                }

                ToolTip.delay: 1000
                ToolTip.timeout: 2000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Run/Stop script")
            }

        }   // RowLayout
    }   // ColumnLayout

}
