import "../../components"
import "../../dialogs"
import "../headers"
import Git 1.0
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Pass 1.0
import QtQuick 2.4

Page {
    id: importGitClonePage

    Component.onCompleted: {
        PopupUtils.open(importGitCloneValidation, importGitClonePage);
    }

    Flow {
        anchors.top: importGitCloneHeader.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: units.gu(2)
        anchors.rightMargin: units.gu(2)
        spacing: units.gu(1)

        Rectangle {
            width: parent.width
            height: units.gu(1)
        }

        Text {
            id: repoUrlLabel

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width
            text: i18n.tr('Repo Url')
        }

        TextField {
            id: repoUrlInput

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width
        }

        Text {
            id: repoPasswordLabel

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width
            text: i18n.tr('Password')
        }

        TextField {
            id: repoPasswordInput

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width
            echoMode: TextInput.Password
        }

        Button {
            id: buttonAdd

            width: parent.width
            text: i18n.tr('Clone')
            onClicked: {
                var ret = false;
                if(repoPasswordInput.text === "") {
                    ret = Git.clone_http(repoUrlInput.text, Pass.password_store);
                } else {
                    ret = Git.clone_http_pass(repoUrlInput.text, Pass.password_store, repoPasswordInput.text);
                }

                if (ret)
                    PopupUtils.open(dialogImportGitCloneSuccess);
                else
                    PopupUtils.open(importGitCloneError, importGitClonePage);
            }
        }

    }

    Component {
        id: importGitCloneValidation

        SimpleValidationDialog {
            text: i18n.tr("Importing a git repo will delete<br>any existing password store!<br>Continue ?")
            onCanceled: {
                pageStack.pop();
            }
        }

    }

    Component {
        id: importGitCloneError

        ErrorDialog {
            textError: i18n.tr("An error occured during git clone !")
        }

    }

    Component {
        id: dialogImportGitCloneSuccess

        SuccessDialog {
            textSuccess: i18n.tr("Password store sucessfully imported !")
            onDialogClosed: {
                pageStack.pop();
                pageStack.pop();
            }
        }

    }

    header: StackHeader {
        id: importGitCloneHeader

        title: i18n.tr('Git Clone Import')
    }

}
