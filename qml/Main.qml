import QtQuick 2.4
import QtQuick.Layouts 1.1
import Qt.labs.folderlistmodel 2.1
import Ubuntu.Components 1.3

import Pass 1.0
import "components" as MyComponents

MainView {
    id: root
    objectName: "mainView"
    applicationName: "utpass.qrouland"
    automaticOrientation: true

    width: units.gu(45)
    height: units.gu(75)

    Page {
        id: page
        anchors.fill: parent

        header: PageHeader {
            id: header
            width: parent.width
            height: units.gu(6)
            title: i18n.tr("UTPass")
            flickable: navigation
            leadingActionBar.height: units.gu(4)
            leadingActionBar.actions: [
                Action {
                    id: backAction
                    iconName: "back"
                    text: "Back"
                    visible:false
                    onTriggered: {
                            folderModel.folder = folderModel.parentFolder;
                            if(folderModel.rootFolder === folderModel.folder) {
                                backAction.visible = false;
                            }
                    }
                }
            ]
        }

        Flickable {
            id: navigation
            anchors.fill: parent

            ListView {
                id : listViewDirs
                anchors.fill: parent
                spacing: 1
                model: FolderListModel {
                    id: folderModel
                    nameFilters: ["*.gpg"]
                    rootFolder: "file:assets/lol"
                    folder: "file:assets/lol"
                }
                delegate: MyComponents.ViewFileDir {
                    id: fileDelegate
                }
            }
        }

    }

    //Component.onCompleted: Pass.speak()
}
