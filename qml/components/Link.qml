import QtQuick 2.4
import Ubuntu.Components 1.3
import "../styles"

Item {
     property string url
     property string text

     property bool commonBorder : true
     property int lBorderwidth : 0
     property int rBorderwidth : 0
     property int tBorderwidth : 0
     property int bBorderwidth : 0
     property int commonBorderWidth : 0
     property string borderColor : UbuntuColors.warmGrey

     width: parent.width
     height: units.gu(6)

    Rectangle {
        width: parent.width
        height: parent.height

        Text {
            text: parent.parent.text
            anchors.left: parent.left
            anchors.leftMargin: units.gu(1)
            anchors.verticalCenter: parent.verticalCenter
        }

        Icon {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            width: units.gu(4)
            height: units.gu(4)
            name:  "go-next"
            color: Theme.raisin_black
        }

        MouseArea {
            anchors.fill: parent
            onClicked: { Qt.openUrlExternally(parent.parent.url); }
        }

        CustomBorder {
            id : cb
            commonBorder: parent.parent.commonBorder
            lBorderwidth: parent.parent.lBorderwidth
            rBorderwidth: parent.parent.rBorderwidth
            tBorderwidth: parent.parent.tBorderwidth
            bBorderwidth: parent.parent.bBorderwidth
            borderColor: parent.parent.borderColor
        }
    }
}