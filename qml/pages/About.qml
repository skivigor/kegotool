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
import "../components"

Page
{
    id: id_pageTerm
    title: qsTr("About")

    property string _year: Qt.formatDateTime(new Date(), "yyyy")

    KPane
    {
        anchors.fill: parent
        anchors.margins: 5
        anchors.centerIn: parent
    }

    Column
    {
        width: parent.width - 100
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -10
        spacing: 10
        Image
        {
            width: 250
            fillMode: Image.PreserveAspectFit
            source: Style.logo
        }

        KHeader { text: SysInfo.appName + " " + SysInfo.appVersion }
        KLabel
        {
            text: SysInfo.prettyProductName + " (" + SysInfo.kernelType + " " + SysInfo.kernelVersion + ")"
        }
        KLabel { text: "Used: Qt 5.15.2" }
        KLabel
        {
            width: parent.width
            wrapMode: Text.WordWrap
            text: SysInfo.appName + " is free software; you can redistribute it and/or modify it under the terms of the GNU General Public Licence as published by the Free Software Foundation; either version 3 of the Licence, or (at your option) any later version."
        }
        KLabel
        {
            width: parent.width
            wrapMode: Text.WordWrap
            text: SysInfo.appName + " is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."
        }


        Row
        {
            spacing: 10
            KLabel
            {
                text: qsTr("See the GNU General Public License for more details ")
            }
            TextInput
            {
                id: id_txtGnu
                font.pixelSize: Style.text.link.fontSize
                selectByMouse: true
                readOnly: true
                color: Style.text.link.color
                text: "https://www.gnu.org/licenses/"

                ToolTip { id: id_gnuToolTip }

                MouseArea
                {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        id_txtGnu.selectAll()
                        id_txtGnu.copy()
                        id_txtGnu.deselect()
                        id_gnuToolTip.show("Copied to clipboard", 1000)

                        FsUtils.openWithApp(id_txtGnu.text)
                    }
                }
            }   // TextInput
        }   // row

        Row
        {
            spacing: 20
            KLabel
            {
                text: qsTr("Project:")
            }
            TextInput
            {
                id: id_txtWebProj
                font.pixelSize: Style.text.link.fontSize
                selectByMouse: true
                readOnly: true
                color: Style.text.link.color
                text: SysInfo.webProj

                ToolTip { id: id_webProjToolTip }

                MouseArea
                {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        id_txtWebProj.selectAll()
                        id_txtWebProj.copy()
                        id_txtWebProj.deselect()
                        id_webProjToolTip.show("Copied to clipboard", 1000)

                        FsUtils.openWithApp(id_txtWebProj.text)
                    }
                }
            }   // TextInput
        }   // row

        Row
        {
            spacing: 20
            KLabel
            {
                text: qsTr("Builds:")
            }
            TextInput
            {
                id: id_txtWebBuilds
                font.pixelSize: Style.text.link.fontSize
                selectByMouse: true
                readOnly: true
                color: Style.text.link.color
                text: SysInfo.webBuilds

                ToolTip { id: id_webBuildsToolTip }

                MouseArea
                {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        id_txtWebBuilds.selectAll()
                        id_txtWebBuilds.copy()
                        id_txtWebBuilds.deselect()
                        id_webBuildsToolTip.show("Copied to clipboard", 1000)

                        FsUtils.openWithApp(id_txtWebBuilds.text)
                    }
                }
            }   // TextInput
        }   // row

        Row
        {
            spacing: 20
            KLabel
            {
                text: qsTr("E-mail:")
            }
            TextInput
            {
                id: id_txtEmail
                font.pixelSize: Style.text.link.fontSize
                selectByMouse: true
                readOnly: true
                color: Style.text.link.color
                text: SysInfo.email

                ToolTip { id: id_emailToolTip }

                MouseArea
                {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked:
                    {
                        id_txtEmail.selectAll()
                        id_txtEmail.copy()
                        id_txtEmail.deselect()
                        id_emailToolTip.show("Copied to clipboard", 1000)

                        FsUtils.openWithApp("mailto:" + id_txtEmail.text)
                    }
                }
            }   // TextInput
        }   // row

        KLabel
        {
            text: "© 2018-" + _year + " " + SysInfo.appOrg
        }
    }

    Component.onCompleted: console.log("About ctor")
    Component.onDestruction:  console.log("About dtor")
}
