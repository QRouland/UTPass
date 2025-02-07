import "../../components"
import "../../dialogs"
import "../../settings"
import "../headers"
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Pass 1.0
import QtQuick 2.4

Page {
    id: deleteRepoPage

    Component.onCompleted: {
        Pass.deletePasswordStoreSucceed.connect(function(keys_info) {
            PopupUtils.open(deleteRepoPagePageDeleteSuccess);
        });
        Pass.deletePasswordStoreFailed.connect(function(message) {
            PopupUtils.open(deleteRepoPagePageDeleteError);
        });
    }

    Column {
        id: deleteRepoPageListView

        anchors.top: deleteRepoPageHeader.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: units.gu(2)
        anchors.rightMargin: units.gu(2)
        spacing: units.gu(1)

        Rectangle {
            width: parent.width
            height: units.gu(1)
            color: theme.palette.normal.background
        }

        Button {
            id: buttonDeleteKey

            width: parent.width
            text: i18n.tr("Delete Password Store")
            color: theme.palette.normal.negative
            onClicked: {
                PopupUtils.open(deleteRepoPagePageDeleteValidation, deleteRepoPage);
            }
        }

    }

    Component {
        id: deleteRepoPagePageDeleteValidation

        SimpleValidationDialog {
            text: i18n.tr("You're are about to delete<br>the current Password Store.<br>Continue ?")
            continueText: i18n.tr("Yes")
            continueColor: theme.palette.normal.negative
            onValidated: {
                var status = Pass.deletePasswordStore();
            }
        }

    }

    Component {
        id: deleteRepoPagePageDeleteError

        ErrorDialog {
            textError: i18n.tr("Password Store removal failed !")
        }

    }

    Component {
        id: deleteRepoPagePageDeleteSuccess

        SuccessDialog {
            textSuccess: i18n.tr("Password Store deleted !")
            onDialogClosed: {
                GitSettings.type = 0;
                GitSettings.repoUrl = null;
                pageStack.clear();
                pageStack.push(Qt.resolvedUrl("../PasswordList.qml"));
            }
        }

    }

    header: StackHeader {
        id: deleteRepoPageHeader

        title: i18n.tr('Info Keys')
    }

}
