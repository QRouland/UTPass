import QtQuick 2.4
import Ubuntu.Components 1.3
import "headers"

Page {
    header: StackHeader {
        id:settingsHeader
        title: i18n.tr('Settings')
    }

    Rectangle {
        anchors.top: settingsHeader.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        color: "#FFF"

        Flow {
            spacing: 1
            anchors.fill: parent
            Text {
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                height: units.gu(6)
                text: "Settings"
            }
         }
    }
}


