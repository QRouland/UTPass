import "../components"
import "../dialogs"
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
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
        Pass.onShowSucceed.connect(function(filename, text) {
            pageStack.push(Qt.resolvedUrl("../pages/Password.qml"), {
                "plainText": text,
                "title": filename
            });
        });
        Pass.onShowFailed.connect(function(message) {
            PopupUtils.open(passwordPageDecryptError);
        });
    }

    Column {
        anchors.top: passwordListHeader.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: units.gu(2)
        anchors.rightMargin: units.gu(2)

        Rectangle {
            width: parent.width
            height: units.gu(1)
        }

        Text {
            text: i18n.tr("No password found")
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }

        Rectangle {
            width: parent.width
            height: units.gu(1)
        }

        Text {
            text: i18n.tr("You can import a password store by cloning or")
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            text: i18n.tr("importing a password store zip in the settings")
            width: parent.width
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

    Component {
        id: passwordPageDecryptError

        ErrorDialog {
            textError: i18n.tr("Decryption failed !")
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
                    console.debug(folderModel.folder);
                    if (folderModel.rootFolder === folderModel.folder) {
                        backAction.visible = false;
                        passwordListHeader.title = i18n.tr("UTPass");
                    } else {
                        passwordListHeader.title = folderModel.folder;
                    }
                }
            }
        ]
    }

}
