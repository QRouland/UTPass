import "../dialogs"
import "../styles"
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Lomiri.Components.Themes 1.3
import Pass 1.0
import QtQuick 2.4

Component {
    Rectangle {
        anchors.right: parent.right
        anchors.left: parent.left
        height: units.gu(5)
        color: theme.palette.normal.background

        Text {
            text: fileBaseName
            anchors.left: parent.left
            anchors.leftMargin: units.gu(2)
            anchors.verticalCenter: parent.verticalCenter
            color: theme.palette.normal.backgroundText
        }

        Icon {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: units.gu(2)
            height: units.gu(4)
            name: fileIsDir ? "go-next" : "lock"
            color: LomiriColors.orange
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (fileIsDir) {
                    folderModel.folder = folderModel.folder + "/" + fileName;
                    backAction.visible = true;
                    passwordListHeader.title = fileName;
                } else {
                    Pass.show(folderModel.folder + "/" + fileName);
                }
            }
        }

        CustomBorder {
            commonBorder: false
            lBorderwidth: 0
            rBorderwidth: 0
            tBorderwidth: 0
            bBorderwidth: 1
            borderColor: LomiriColors.warmGrey
        }

        Component {
            id: passwordPageDecryptError

            ErrorDialog {
                textError: i18n.tr("Decryption failed !")
            }

        }

    }

}
