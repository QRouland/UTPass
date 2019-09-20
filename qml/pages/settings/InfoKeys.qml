import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Pass 1.0
import "../headers"
import "../../components"
import "../../dialogs"

Page {
    id: infoKeysPage

    property string currentKey

    header: StackHeader {
        id: infoKeysHeader
        title: i18n.tr('Info Keys')
    }

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
            }

            Text {
                id: uidKey
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: i18n.tr('Key id : %1').arg(model.modelData.uid)
            }

            Button {
                id: buttonDeleteKey
                text: i18n.tr("Delete this key")
                color: UbuntuColors.red
                onClicked: {
                    infoKeysPage.currentKey = model.modelData.uid
                    PopupUtils.open(infoKeysPageDeleteValidation, infoKeysPage)
                }
            }

            Rectangle {
                width: parent.width
                height: units.gu(1)
            }
        }
    }

    Component {
        id: infoKeysPageDeleteValidation
        DoubleValidationDialog {
            text1: i18n.tr(
                       "You're are about to delete<br>%1<br>Continue ?").arg(
                       infoKeysPage.currentKey)
            text2: i18n.tr(
                       "%1<br>will be definitively removed.<br>Continue ?").arg(
                       infoKeysPage.currentKey)
            onDoubleValidated: {
                var status = Pass.gpgDeleteKeyId(infoKeysPage.currentKey)
                if (status) {
                    PopupUtils.open(infoKeysPageDeleteSuccess)
                } else {
                    PopupUtils.open(infoKeysPageDeleteError)
                }
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
                infoKeysListView.model = Pass.gpgGetAllKeysModel()
            }
        }
    }
}
