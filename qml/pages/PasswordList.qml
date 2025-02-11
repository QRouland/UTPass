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

    property string __passwordStorePath
    property var __passwords

    function __searchPasswords(filter) {
        var ret = [];
        if (__passwords) {
            for (var i = 0; i < __passwords.length; i++) {
                if (__passwords[i].toUpperCase().indexOf(filter.toUpperCase()) > -1)
                    ret.push(__passwords[i]);
            }
        }
        return ret;
    }

    function __searchUpdateModel() {
        const filter = passwordListHeader.searchBar.text;
        console.info("filter : %1".arg(filter));
        var ret = __searchPasswords(filter);
        passwordListSearch.model.clear();
        for (var i = 0; i < ret.length; i++) {
            if (ret[i])
                passwordListSearch.model.append({
                "fileName": ret[i]
            });

        }
    }

    anchors.fill: parent
    Component.onCompleted: {
        passwordListPage.__passwordStorePath = "file:" + Pass.password_store;
        Pass.onShowSucceed.connect(function(filename, text) {
            pageStack.push(Qt.resolvedUrl("../pages/Password.qml"), {
                "plainText": text,
                "title": filename
            });
        });
        Pass.onShowFailed.connect(function(message) {
            PopupUtils.open(passwordPageDecryptError);
        });
        Pass.onLsSucceed.connect(function(passwords) {
            passwordListPage.__passwords = passwords;
            __searchUpdateModel();
        });
        Pass.ls();
    }

    Column {
        id: passwordListEmpty

        anchors.top: passwordListHeader.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: units.gu(2)
        anchors.rightMargin: units.gu(2)
        visible: passwordListNav.model.count === 0

        Rectangle {
            width: parent.width
            height: units.gu(1)
            color: theme.palette.normal.background
        }

        Text {
            text: i18n.tr("No password found")
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            color: theme.palette.normal.backgroundText
        }

        Rectangle {
            width: parent.width
            height: units.gu(1)
            color: theme.palette.normal.background
        }

        Text {
            text: i18n.tr("You can import a password store by cloning or")
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            color: theme.palette.normal.backgroundText
        }

        Text {
            text: i18n.tr("importing a password store zip in the settings")
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            color: theme.palette.normal.backgroundText
        }

    }

    ListView {
        id: passwordListNav

        anchors.top: passwordListHeader.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        spacing: 1
        visible: passwordListNav.model.count !== 0 && !passwordListHeader.searchBar.visible

        model: FolderListModel {
            nameFilters: ["*.gpg"]
            rootFolder: passwordListPage.__passwordStorePath
            folder: passwordListPage.__passwordStorePath
            showDirs: true
        }

        delegate: Component {
            FileDir {
                fName: fileName
                fIsDir: fileIsDir
                onClicked: {
                    var path = passwordListNav.model.folder + "/" + fileName;
                    if (fileIsDir) {
                        passwordListNav.model.folder = path;
                        backAction.visible = true;
                        passwordListHeader.title = fileName;
                    } else {
                        console.debug("pass show %1".arg(path));
                        Pass.show(path);
                    }
                }
            }

        }

    }

    ListView {
        id: passwordListSearch

        anchors.top: passwordListHeader.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        visible: passwordListNav.model.count !== 0 && passwordListHeader.searchBar.visible

        model: ListModel {
        }

        delegate: Component {
            FileDir {
                fName: fileName
                fIsDir: false
                onClicked: {
                    var path = passwordListPage.__passwordStorePath + "/" + fileName;
                    console.debug("pass show %1".arg(path));
                    Pass.show(path);
                }
            }

        }

    }

    Component {
        id: passwordPageDecryptError

        ErrorDialog {
            textError: i18n.tr("Decryption failed !")
        }

    }

    Timer {
        id: searchTimer
        interval: 500
        onTriggered: __searchUpdateModel()
    }

    header: MainHeader {
        id: passwordListHeader

        searchBar.onTextChanged: searchTimer.restart()
        leadingActionBar.height: units.gu(4)
        leadingActionBar.actions: [
            Action {
                id: backAction

                iconName: "back"
                text: i18n.tr("Back")
                visible: false
                onTriggered: {
                    passwordListNav.model.folder = passwordListNav.model.parentFolder;
                    console.debug(passwordListNav.model.folder);
                    if (passwordListNav.model.rootFolder === passwordListNav.model.folder) {
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
