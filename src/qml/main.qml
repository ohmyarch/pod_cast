/****************************************************************************
**
** Copyright (C) 2016 Michael Yang
** Contact: ohmyarchlinux@gmail.com
**
** This file is part of the pod_cast.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0

ApplicationWindow {
    id: mainWindow
    title: qsTr("pod_cast")
    visible: true
    visibility: Window.Maximized

    header: ToolBar {
        Material.foreground: "white"

        RowLayout {
            spacing: 20 // FIXME
            anchors.fill: parent

            ToolButton {
                contentItem: Image {
                    sourceSize.height: titleLabel.height
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/drawer.svg"
                }
                onClicked: drawer.open()
            }

            Label {
                id: titleLabel
                text: qsTr("Subscriptions")
                font.pointSize: 16
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                contentItem: Image {
                    sourceSize.height: titleLabel.height
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/images/menu.svg"
                }
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    font.pointSize: 11

                    MenuItem {
                        text: qsTr("Add Podcast")
                        onTriggered: {
                            addPodcastPopup.open()
                            addPodcastPopup.forceActiveFocus()
                            podcastUrlField.clear()
                            podcastUrlField.paste()
                            podcastUrlField.ensureVisible(0)
                        }
                    }
                }
            }
        }
    }

    Drawer {
        id: drawer
        width: Math.min(mainWindow.width, mainWindow.height) / 3 * 1
        height: mainWindow.height
    }

    Popup {
        id: addPodcastPopup
        x: (mainWindow.width - width) / 2
        y: (mainWindow.height - height) / 2
        modal: true
        font.pointSize: 16

        contentItem: ColumnLayout {
            id: addPodcastColumn
            spacing: 20 // FIXME

            RowLayout {
                spacing: 10 // FIXME

                Label {
                    text: qsTr("URL: ")
                }

                TextField {
                    id: podcastUrlField
                    focus: true
                    Layout.minimumWidth: Math.min(mainWindow.width,
                                                  mainWindow.height) / 3 * 2
                }
            }

            Button {
                id: okButton
                text: "Ok"
                onClicked: addPodcastPopup.close()

                Material.foreground: Material.primary
                Material.background: "transparent"
                Material.elevation: 0

                Layout.preferredWidth: 0
                Layout.fillWidth: true
            }
        }
    }
}
