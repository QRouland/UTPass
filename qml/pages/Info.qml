import "../components"
import Lomiri.Components 1.3
import QtQuick 2.4
import "headers"

Page {
    id: infoPage

    Component.onCompleted: {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "../../manifest_.json", false);
        xhr.send();
        var mJson = JSON.parse(xhr.responseText);
        manifestTitle.text = "<b>" + mJson.title + "</b>";
        manifestVersion.text = mJson.version + "<br>" + mJson.framework + "@" + mJson.architecture;
        manifestMaintener.text = mJson.maintainer;
    }

    Flow {
        anchors.top: infoHeader.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        spacing: units.gu(3)

        Rectangle {
            width: parent.width
            height: units.gu(1)
        }

        Text {
            id: manifestTitle

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width
            height: units.gu(8)
            fontSizeMode: Text.Fit
            font.pixelSize: 144
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
            height: units.gu(3)
            text: i18n.tr("<b>Version</b>")
            fontSizeMode: Text.Fit
            font.pixelSize: 72
        }

        Text {
            id: manifestVersion

            horizontalAlignment: Text.AlignHCenter
            width: parent.width
            height: units.gu(4)
            fontSizeMode: Text.Fit
            font.pixelSize: 72
        }

        Text {
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
            height: units.gu(3)
            text: i18n.tr("<b>Maintainer</>")
            fontSizeMode: Text.Fit
            font.pixelSize: 72
        }

        Text {
            id: manifestMaintener

            horizontalAlignment: Text.AlignHCenter
            width: parent.width
            height: units.gu(2)
            fontSizeMode: Text.Fit
            font.pixelSize: 72
        }

    }

    Flow {
        spacing: 2
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left

        ExternalLink {
            url: "https://github.com/QRouland/UTPass/issues"
            text: i18n.tr("Suggest improvement(s) or report a bug(s)")
        }

        ExternalLink {
            url: "https://github.com/QRouland/UTPass"
            text: i18n.tr("Access to the source code")
        }

        Text {
            width: parent.width
            height: units.gu(2)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: i18n.tr("Released under the terms of the GNU GPL v3")
        }

    }

    header: StackHeader {
        id: infoHeader

        title: i18n.tr('Info')
    }

}
