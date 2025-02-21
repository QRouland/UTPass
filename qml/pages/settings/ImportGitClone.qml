import "../../components"
import "../../dialogs"
import "../../settings"
import "../headers"
import Git 1.0
import Lomiri.Components 1.3
import Lomiri.Components.Pickers 1.3
import Lomiri.Components.Popups 1.3
import Pass 1.0
import QtQuick 2.4
import Utils 1.0
Page {
    id: importGitClonePage

    property int __gitModeHTTP : 0
    property int __gitModeHTTP_AUTH : 1
    property int __gitModeSSH_KEY : 2


    property string __repoUrl

    function __loadForm() {
        switch (combo.selectedIndex) {
            case __gitModeHTTP:
                importGitCloneForm.source = Qt.resolvedUrl("../../components/GitCloneHttp.qml");
                break;
            case __gitModeHTTP_AUTH:
                importGitCloneForm.source = Qt.resolvedUrl("../../components/GitCloneHttpAuth.qml");
                break;
            case __gitModeSSH_KEY:
                importGitCloneForm.source = Qt.resolvedUrl("../../components/GitCloneSshKey.qml");
                break;
        }
    }

    Component.onCompleted: {
        Git.cloneSucceed.connect(function() {
            GitSettings.type = combo.selectedIndex;
            GitSettings.repoUrl = importGitClonePage.__repoUrl;
            if(GitSettings.type != __gitModeSSH_KEY) { // ensure there no ssh key is kept if swicthing to another git mode
                Utils.rmFile(Git.privKey);
                Utils.rmFile(Git.pubKey);
            }
            PopupUtils.open(dialogGitCloneSuccess);
        });
        Git.cloneFailed.connect(function() {
            PopupUtils.open(dialogGitCloneError);
        });
        if (GitSettings.repoUrl)
            __repoUrl = GitSettings.repoUrl;

        if (GitSettings.type < combo.count && GitSettings.type > 0)
            combo.selectedIndex = GitSettings.type;
        else
            combo.selectedIndex = 0;
        __loadForm();
        PopupUtils.open(importGitCloneValidation, importGitClonePage);
    }

    Column {
        anchors.top: importGitCloneHeader.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: units.gu(2)
        anchors.rightMargin: units.gu(2)
        spacing: units.gu(1)

        Rectangle {
            width: parent.width
            height: units.gu(1)
            color: theme.palette.normal.background
        }

        OptionSelector {
            id: combo

            width: parent.width
            model: ["HTTP", "HTTP AUTH", "SSH KEY"]
            onDelegateClicked: function(i) {
                timer.setTimeout(function() {
                    __loadForm();
                }, 500);
            }

            Timer {
                id: timer

                function setTimeout(cb, delayTime) {
                    timer.interval = delayTime;
                    timer.repeat = false;
                    timer.triggered.connect(cb);
                    timer.triggered.connect(function release() {
                        timer.triggered.disconnect(cb); // This is important
                        timer.triggered.disconnect(release); // This is important as well
                    });
                    timer.start();
                }

            }

        }

        Loader {
            id: importGitCloneForm

            width: parent.width
            onLoaded: {
                importGitCloneForm.item.repoUrlChanged.connect(function(url) {
                    importGitClonePage.__repoUrl = url;
                });
                importGitCloneForm.item.setRepoUrl(importGitClonePage.__repoUrl);

                switch (combo.selectedIndex) {
                    case __gitModeHTTP:
                        break;
                    case __gitModeHTTP_AUTH:
                        break;
                    case __gitModeSSH_KEY:
                        importGitCloneForm.item.importSshPrivKeyButton.clicked.connect(function() {
                            pageStack.push(Qt.resolvedUrl("ImportSSHkey.qml"), {
                                "isPrivateKey": true
                            });
                        });
                        importGitCloneForm.item.importSshPubKeyButton.clicked.connect(function() {
                            pageStack.push(Qt.resolvedUrl("ImportSSHkey.qml"), {
                                "isPrivateKey": false
                            });
                        });
                        break;
                    }
            }
        }

    }

    Component {
        id: importGitCloneValidation

        SimpleValidationDialog {
            text: i18n.tr("Importing a git repo will delete<br>any existing password store!<br>Continue ?")
            continueText: i18n.tr("Yes")
            continueColor: theme.palette.normal.negative
            onCanceled: {
                pageStack.pop();
            }
        }

    }

    Component {
        id: dialogGitCloneError

        ErrorDialog {
            textError: i18n.tr("An error occured during git clone !")
        }

    }

    Component {
        id: dialogGitCloneSuccess

        SuccessDialog {
            textSuccess: i18n.tr("Password store sucessfully imported !")
            onDialogClosed: {
                pageStack.clear();
                pageStack.push(Qt.resolvedUrl("../PasswordList.qml"));
            }
        }

    }

    header: StackHeader {
        id: importGitCloneHeader

        title: i18n.tr('Git Clone Import')
    }

}
