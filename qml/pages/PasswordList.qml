import "../components"
import Lomiri.Components 1.3
import Pass 1.0
import Qt.labs.folderlistmodel 2.1
import QtQuick 2.4
import "headers"

Page {
    id: passwordListPage

    property string passwordStorePath

    anchors.fill: parent
    Component.onCompleted: {
        passwordStorePath = "file:" + Pass.password_store;
    }

    Rectangle {
        anchors.top: passwordListHeader.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        visible: folderModel.count == 0

        Text {
            text: i18n.tr("No password found<br>You can import a password store by cloning or importing a zip in the settings")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
        }

    }

    ListView {
        anchors.top: passwordListHeader.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        spacing: 1

        model: FolderListModel {
            id: folderModel

            nameFilters: ["*.gpg"]
            rootFolder: passwordStorePath
            folder: passwordStorePath
            showDirs: true
        }

        delegate: FileDir {
            id: fileDelegate
        }

    }

    header: MainHeader {
        id: passwordListHeader

        leadingActionBar.height: units.gu(4)
        leadingActionBar.actions: [
            Action {
                id: backAction

                iconName: "back"
                text: i18n.tr("Back")
                visible: false
                onTriggered: {
                    folderModel.folder = folderModel.parentFolder;
                    if (folderModel.rootFolder === folderModel.folder)
                        backAction.visible = false;

                }
            }
        ]
    }

}
