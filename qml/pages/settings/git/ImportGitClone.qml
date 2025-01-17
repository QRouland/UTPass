import "../../../components"
import "../../../dialogs"
import "../../headers"
import Git 1.0
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Pass 1.0
import QtQuick 2.4

Page {
    id: importGitClonePage

    Component.onCompleted: {
        Git.onCloneSucceed.connect(function() {
            PopupUtils.open(dialogGitCloneSuccess);
        });
        Git.onCloneFailed.connect(function() {
            PopupUtils.open(dialogGitCloneError);
        });
        PopupUtils.open(importGitCloneValidation, importGitClonePage);
        importGitCloneForm.source = Qt.resolvedUrl("GitCloneHttp.qml")
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
        }

        GitModeOptionSelector {
            id: combo
        }

        Loader {
            id: importGitCloneForm

            width: parent.width
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
