import QtQuick 2.4
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Pass 1.0
import "../styles"
import "../dialogs"

Component {
    Rectangle {
        id: fileDir
        property string activePasswordName

        anchors.right: parent.right
        anchors.left: parent.left
        height: units.gu(5)

        Text {
            text: fileBaseName
            anchors.left: parent.left
            anchors.leftMargin: units.gu(2)
            anchors.verticalCenter: parent.verticalCenter
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
                    folderModel.folder = folderModel.folder + "/" + fileName
                    backAction.visible = true
                } else {
                    fileDir.activePasswordName = fileBaseName

                    Pass.onDecrypted.connect(function (text) {
                        pageStack.push(Qt.resolvedUrl("../pages/Password.qml"),
                                       {
                                           "plainText": text,
                                           "title": fileDir.activePasswordName
                                       })
                    })

                    Pass.onDecryptFailed.connect(function () {
                        PopupUtils.open(passwordPageDecryptError)
                    })

                    Pass.decrypt(folderModel.folder + "/" + fileName)
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
