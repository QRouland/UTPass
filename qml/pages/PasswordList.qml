import QtQuick 2.4
import Ubuntu.Components 1.3
import Qt.labs.folderlistmodel 2.1
import "../components"
import "headers"

Page {
    header: MainHeader {
        flickable: nav

        leadingActionBar.height: units.gu(4)
        leadingActionBar.actions: [
            Action {
                id: backAction
                iconName: "back"
                text: i18n.tr("Back")
                visible: false
                onTriggered: {
                    folderModel.folder = folderModel.parentFolder
                    if (folderModel.rootFolder === folderModel.folder) {
                        backAction.visible = false
                    }
                }
            }
        ]
    }

    Flickable {
        id: nav
        height: parent.height
        width: parent.width

        Rectangle {
            width: parent.width
            visible: folderModel.count == 0
            height: units.gu(5)
            Text {
                text: i18n.tr("No password found in the current folder")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        ListView {
            anchors.fill: parent
            spacing: 1
            model: FolderListModel {
                id: folderModel
                nameFilters: ["*.gpg"]
                rootFolder: "file:password-store"
                folder: "file:password-store"
            }
            delegate: FileDir {
                id: fileDelegate
            }
        }
    }
}
