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
            text: fileIsDir ? fileName : fileName.slice(0, -4) // remove .gpg if it's a file
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
                var path = folderModel.folder + "/" + fileName;
                if (fileIsDir) {
                    folderModel.folder = path;
                    backAction.visible = true;
                    passwordListHeader.title = fileName;
                } else {
                    console.debug("pass show %1".arg(path));
                    Pass.show(path);
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

    }

}
