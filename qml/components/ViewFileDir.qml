import QtQuick 2.4
import Ubuntu.Components 1.3

Component {
    Rectangle {
        width: page.width
        height: units.gu(5)
        Text {
            text: fileBaseName
            anchors.left: parent.left
            anchors.leftMargin: units.gu(1)
            anchors.verticalCenter: parent.verticalCenter
        }
        Icon {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            width: units.gu(4)
            height: units.gu(4)
            name:  fileIsDir ? "go-next" : "lock"
            color: UbuntuColors.orange
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked: {  if (fileIsDir){ folderModel.folder = folderModel.folder + "/" + fileName ; backAction.visible = true;} }
        }
        CustomBorder
        {
            commonBorder: false
            lBorderwidth: 0
            rBorderwidth: 0
            tBorderwidth: 0
            bBorderwidth: 1
            borderColor: UbuntuColors.warmGrey
        }
    }
}