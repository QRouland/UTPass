import QtQuick 2.4
import Ubuntu.Components 1.3
import "headers"
import "../components"

Page {
    header: StackHeader {
        id : infoHeader
        title: i18n.tr('Info')
    }

    Rectangle {
        anchors.top: infoHeader.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        color: "#FFF"

        Flow {
            spacing: 2
            anchors.fill: parent
            Rectangle {
               width: parent.width
               height: units.gu(4)
            }
            Rectangle {
               width: parent.width
               height: units.gu(12)
               Image {
                   source: "../../assets/logo.svg"
                   width: units.gu(12)
                   height: units.gu(12)
                   anchors.horizontalCenter: parent.horizontalCenter
                }
            }
            Rectangle {
               width: parent.width
               height: units.gu(4)
            }
            Text {
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                height: units.gu(6)
                text: i18n.tr('UTPass')
            }
            Text {
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                height: units.gu(6)
                text: "Version" //i18n.tr("Version %1").arg(Manifest.get("version"))
            }
            Link {
                url: "https://github.com/QRouland/UTPass/issues"
                text: i18n.tr('Suggest improvements or report a bug')
            }
        }
    }
}


