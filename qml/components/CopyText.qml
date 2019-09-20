import QtQuick 2.4
import Ubuntu.Components 1.3
import "../styles"

Item {
    id: copyText
    property string text

    property bool commonBorder: true
    property int lBorderwidth: 0
    property int rBorderwidth: 0
    property int tBorderwidth: 0
    property int bBorderwidth: 0
    property int commonBorderWidth: 0
    property string borderColor: UbuntuColors.warmGrey

    width: parent.width
    height: units.gu(6)

    Rectangle {
        anchors.fill: parent

        Text {
            text: copyText.text
            anchors.left: parent.left
            anchors.leftMargin: units.gu(2)
            anchors.verticalCenter: parent.verticalCenter
        }

        Icon {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: units.gu(2)
            height: units.gu(4)
            name: "edit-copy"
            color: UbuntuColors.orange
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                parent.color = UbuntuColors.warmGrey
            }
            onClicked: {
                var mimeData = Clipboard.newData()
                mimeData.text = copyText.text
                Clipboard.push(mimeData)
            }
            onReleased: {
                parent.color = theme.palette.normal.background
            }
        }

        CustomBorder {
            id: cb
            commonBorder: copyText.commonBorder
            lBorderwidth: copyText.lBorderwidth
            rBorderwidth: copyText.rBorderwidth
            tBorderwidth: copyText.tBorderwidth
            bBorderwidth: copyText.bBorderwidth
            borderColor: copyText.borderColor
        }
    }
}
