import "../../components"
import "../../dialogs"
import "../headers"
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Pass 1.0
import QtQuick 2.4

Page {
    id: infoKeysPage

    property string currentKey

    ListView {
        id: infoKeysListView

        anchors.top: infoKeysHeader.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        model: Pass.gpgGetAllKeysModel()

        delegate: Grid {
            columns: 1
            width: parent.width
            horizontalItemAlignment: Grid.AlignHCenter
            verticalItemAlignment: Grid.AlignVCenter

            Rectangle {
                width: parent.width
                height: units.gu(1)
                color: theme.palette.normal.background
            }

            Text {
                id: uidKey

                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: i18n.tr('Key id : %1').arg(model.modelData.uid)
                color: theme.palette.normal.backgroundText
            }

            Rectangle {
                width: parent.width
                height: units.gu(1)
                color: theme.palette.normal.background
            }

            Button {
                id: buttonDeleteKey

                text: i18n.tr("Delete this key")
                color: theme.palette.normal.negative
                onClicked: {
                    infoKeysPage.currentKey = model.modelData.uid;
                    PopupUtils.open(infoKeysPageDeleteValidation, infoKeysPage);
                }
            }

            Rectangle {
                width: parent.width
                height: units.gu(1)
                color: theme.palette.normal.background
            }

        }

    }

    Component {
        id: infoKeysPageDeleteValidation

        SimpleValidationDialog {
            text: i18n.tr("You're are about to delete<br>%1<br>Continue ?").arg(infoKeysPage.currentKey)
            continueText: i18n.tr("Yes")
            continueColor: theme.palette.normal.negative
            onValidated: {
                var status = Pass.gpgDeleteKeyId(infoKeysPage.currentKey);
                if (status)
                    PopupUtils.open(infoKeysPageDeleteSuccess);
                else
                    PopupUtils.open(infoKeysPageDeleteError);
            }
        }

    }

    Component {
        id: infoKeysPageDeleteError

        ErrorDialog {
            textError: i18n.tr("Key removal failed !")
        }

    }

    Component {
        id: infoKeysPageDeleteSuccess

        SuccessDialog {
            textSuccess: i18n.tr("Key successfully deleted !")
            onDialogClosed: {
                infoKeysListView.model = Pass.gpgGetAllKeysModel();
            }
        }

    }

    header: StackHeader {
        id: infoKeysHeader

        title: i18n.tr('Info Keys')
    }

}
