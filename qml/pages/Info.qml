import QtQuick 2.4
import Ubuntu.Components 1.3
import "headers"
import "../components"
import "../models"

Page {
    header: StackHeader {
        id: infoHeader
        title: i18n.tr('Info')
    }

    Rectangle {
        anchors.top: infoHeader.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        color: "#FFF"

        Flow {
            spacing: units.gu(3)
            anchors.fill: parent
			Rectangle {
                width: parent.width
                height: units.gu(1)
            }
			Text {
                horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
                width: parent.width
                height: units.gu(4)
                text: i18n.tr('<b><font size="6">%1</font></b>').arg(Manifest.title)
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
			Text {
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                height: units.gu(6)
                text: i18n.tr('<font size="4">Maintainer<br>%1</font>').arg(Manifest.maintainer)
            }
            Text {
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                height: units.gu(6)
                text: i18n.tr('<font size="4">Version<br>%1</font><br>%2@%3').arg(Manifest.version).arg(Manifest.framework).arg(Manifest.architecture)
            }

        }
		Flow {
			spacing: 2
            anchors.bottom: parent.bottom
			anchors.right: parent.right
			anchors.left: parent.left
			Link {
                url: "https://github.com/QRouland/UTPass/issues"
                text: i18n.tr('Suggest improvement(s) or report a bug(s)')
            }
			Link {
                url: "https://github.com/QRouland/UTPass"
                text: i18n.tr('Access to the source code')
            }
			Text {
				width: parent.width
                height: units.gu(3)
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
                text: i18n.tr('<font size="2">Released under the terms of the GNU GPL v3</font>')
            }
		}
    }
}
