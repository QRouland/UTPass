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

Page {
    id: importGitClonePage

    property string __repoUrl

    function __loadForm() {
        console.debug("yo");
        switch (combo.selectedIndex) {
        case 0:
            importGitCloneForm.source = Qt.resolvedUrl("../../components/GitCloneHttp.qml");
            break;
        case 1:
            importGitCloneForm.source = Qt.resolvedUrl("../../components/GitCloneHttpAuth.qml");
            break;
        }
    }

    Component.onCompleted: {
        Git.cloneSucceed.connect(function() {
            GitSettings.type = combo.selectedIndex;
            GitSettings.repoUrl = importGitClonePage.__repoUrl;
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
            model: ["HTTP", "HTTP AUTH"]
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
