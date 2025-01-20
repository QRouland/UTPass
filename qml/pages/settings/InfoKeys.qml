import "../../components"
import "../../dialogs"
import "../headers"
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Pass 1.0
import QtQuick 2.4

Page {
    id: infoKeysPage

    property QtObject currentKey

    Component.onCompleted: {
        Pass.getAllGPGKeysSucceed.connect(function(keys_info) {
            infoKeysListView.model = keys_info;
        });
        Pass.getAllGPGKeysFailed.connect(function(message) {
            PopupUtils.open(infoKeysPageGetAllError);
        });
        Pass.deleteGPGKeySucceed.connect(function(keys_info) {
            PopupUtils.open(infoKeysPageDeleteSuccess);
        });
        Pass.deleteGPGKeyFailed.connect(function(message) {
            PopupUtils.open(infoKeysPageDeleteError);
        });
        Pass.getAllGPGKeys();
    }

    ListView {
        id: infoKeysListView

        anchors.top: infoKeysHeader.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: units.gu(2)
        anchors.rightMargin: units.gu(2)

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
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: i18n.tr('Key ID :')
                color: theme.palette.normal.backgroundText
            }

            Text {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: model.modelData.uid
                color: theme.palette.normal.backgroundText
            }

            Rectangle {
                width: parent.width
                height: units.gu(1)
                color: theme.palette.normal.background
            }

            ListModel {
                id: userIdsModel

                Component.onCompleted: {
                    for (var i = 0; i < model.modelData.userIds.length; ++i) {
                        userIdsModel.append({
                            "model": model.modelData.userIds[i]
                        });
                    }
                }
            }

            Text {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: i18n.tr('Users IDs : ')
                color: theme.palette.normal.backgroundText
            }

            Repeater {
                model: userIdsModel

                Text {
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: modelData.uid
                    color: theme.palette.normal.backgroundText
                }

            }

            Rectangle {
                width: parent.width
                height: units.gu(1)
                color: theme.palette.normal.background
            }

            Button {
                id: buttonDeleteKey

                width: parent.width
                text: i18n.tr("Delete this key")
                color: theme.palette.normal.negative
                onClicked: {
                    infoKeysPage.currentKey = model.modelData;
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
            text: i18n.tr("You're are about to delete<br>%1<br>.Continue ?").arg(infoKeysPage.currentKey.uid)
            continueText: i18n.tr("Yes")
            continueColor: theme.palette.normal.negative
            onValidated: {
                var status = Pass.deleteGPGKey(infoKeysPage.currentKey);
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
                infoKeysListView.model = Pass.getAllGPGKeys();
            }
        }

    }

    Component {
        id: infoKeysPageGetAllError

        ErrorDialog {
            textError: i18n.tr("An Error occured getting GPG keys !")
        }

    }

    header: StackHeader {
        id: infoKeysHeader

        title: i18n.tr('Info Keys')
    }

}
