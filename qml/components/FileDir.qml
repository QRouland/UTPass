import "../dialogs"
import "../styles"
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Lomiri.Components.Themes 1.3
import Pass 1.0
import QtQuick 2.4

Item {
    //property string folder

    id: fileDir

    property string fName
    property bool fIsDir
    property bool commonBorder: true
    property int lBorderwidth: 0
    property int rBorderwidth: 0
    property int tBorderwidth: 0
    property int bBorderwidth: 0
    property int commonBorderWidth: 0
    property string borderColor: LomiriColors.warmGrey

    signal clicked()

    anchors.right: parent.right
    anchors.left: parent.left
    height: units.gu(5)

    Rectangle {
        anchors.fill: parent
        color: theme.palette.normal.background

        Text {
            text: fileDir.fIsDir ? fileDir.fName : fileDir.fName.slice(0, -4) // remove .gpg if it's a file
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
            name: fileDir.fIsDir ? "go-next" : "lock"
            color: LomiriColors.orange
        }

        MouseArea {
            // onClicked: {
            //     var path = fileDir.fdfolder + "/" + fileName;
            //     if (fileIsDir) {
            //         fileDir.fdfolder = path;
            //         //backAction.visible = true;
            //         // passwordListHeader.title = fileName;
            //     } else {
            //         console.debug("pass show %1".arg(path));
            //         Pass.show(path);
            //     }
            // }

            anchors.fill: parent
            onClicked: fileDir.clicked()
        }

        CustomBorder {
            id: cb

            commonBorder: fileDir.commonBorder
            lBorderwidth: fileDir.lBorderwidth
            rBorderwidth: fileDir.rBorderwidth
            tBorderwidth: fileDir.tBorderwidth
            bBorderwidth: fileDir.bBorderwidth
            borderColor: fileDir.borderColor
        }

    }

}
