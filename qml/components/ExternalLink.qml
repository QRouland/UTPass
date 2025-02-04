import "../styles"
import Lomiri.Components 1.3
import QtQuick 2.4

Item {
    id: externalLink

    property string url
    property string text
    property bool commonBorder: true
    property int lBorderwidth: 0
    property int rBorderwidth: 0
    property int tBorderwidth: 0
    property int bBorderwidth: 0
    property int commonBorderWidth: 0
    property string borderColor: LomiriColors.warmGrey

    width: parent.width
    height: units.gu(6)

    Rectangle {
        anchors.fill: parent
        color: theme.palette.normal.background

        Text {
            text: externalLink.text
            anchors.left: parent.left
            anchors.leftMargin: units.gu(2)
            anchors.verticalCenter: parent.verticalCenter
            color: theme.palette.normal.backgroundText
        }

        Icon {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: units.gu(2)
            width: units.gu(4)
            height: units.gu(4)
            name: "go-next"
            color: LomiriColors.orange
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                Qt.openUrlExternally(externalLink.url);
            }
        }

        CustomBorder {
            id: cb

            commonBorder: externalLink.commonBorder
            lBorderwidth: externalLink.lBorderwidth
            rBorderwidth: externalLink.rBorderwidth
            tBorderwidth: externalLink.tBorderwidth
            bBorderwidth: externalLink.bBorderwidth
            borderColor: externalLink.borderColor
        }

    }

}
