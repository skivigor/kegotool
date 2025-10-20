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
    width: 400
    height: id_clmContent.height + 20

    QtObject
    {
        id: set
        property var    _appConfig: appStyle
        property var    _styleList: _appConfig.styleList
        property string _styleName: _appConfig.styleName
        property bool   _edited: false

        on_StyleListChanged:
        {
            id_styleModel.clear()
            var idx = 0

            for (var i = 0; i < _styleList.length; ++i)
            {
                if (_styleList[i].name === _styleName) idx = i

                var obj = {}
                obj.text = _styleList[i].name
                id_styleModel.append(obj)
            }

            id_boxStyleList.currentIndex = idx
        }
    }   // QtObject

    ListModel { id: id_styleModel }

    KPane
    {
        anchors.fill: parent

        Column
        {
            id: id_clmContent
            width: parent.width - 30
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            KHeader { text: qsTr("General") }

            RowLayout
            {
                id: id_row
                width: parent.width
                spacing: 30

                Image
                {
                    id: id_imgStyleScreen
                    Layout.preferredWidth: 300
                    Layout.preferredHeight: 220
                    fillMode: Image.PreserveAspectFit
                    source: set._styleList[id_boxStyleList.currentIndex].screen
                }

                Column
                {
                    Layout.fillWidth: true
                    spacing: 10

                    Text
                    {
                        color: Style.text.label.color
                        font.pixelSize: Style.text.label.fontSize
                        text: qsTr("Interface style")
                    }
                    ComboBox
                    {
                        id: id_boxStyleList
                        width: 200
                        model: id_styleModel
                        contentItem: Text {
                            leftPadding: 15
                            text: id_boxStyleList.displayText
                            font: id_boxStyleList.font
                            color:
                            {
                                if (id_boxStyleList.currentText === set._styleName)
                                {
                                    set._edited = false
                                    return Style.text.label.color
                                } else
                                {
                                    set._edited = true
                                    return Style.text.label2.color
                                }
                            }
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                    }
                    Text
                    {
                        Layout.fillWidth: true
                        color: Style.text.substring.color
                        font.pixelSize: Style.text.substring.fontSize
                        text: qsTr("Application restart required")
                    }
                }

            }   // RowLayout
            Button
            {
                width: 120
                anchors.right: parent.right
                enabled: set._edited
                text: qsTr("Save")
                onClicked: set._appConfig.setStyle(id_boxStyleList.currentIndex)
            }
        }   // Column
    }   // KPane

}
