import "../styles"
import Lomiri.Components 1.3
import QtQuick 2.4

Item {
    id: pageStackLink

    property string page
    property var params: {
    }
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

        Text {
            text: pageStackLink.text
            anchors.left: parent.left
            anchors.leftMargin: units.gu(2)
            anchors.verticalCenter: parent.verticalCenter
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
                pageStack.push(page, params);
            }
        }

        CustomBorder {
            id: cb

            commonBorder: pageStackLink.commonBorder
            lBorderwidth: pageStackLink.lBorderwidth
            rBorderwidth: pageStackLink.rBorderwidth
            tBorderwidth: pageStackLink.tBorderwidth
            bBorderwidth: pageStackLink.bBorderwidth
            borderColor: pageStackLink.borderColor
        }

    }

}
