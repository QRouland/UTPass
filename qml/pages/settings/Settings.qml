import QtQuick 2.4
import Ubuntu.Components 1.3
import Pass 1.0
import "../headers"
import "../../components"

Page {
    id: settingsPage

    property string gpgKeyId: ""

    header: StackHeader {
        id: settingsHeader
        title: i18n.tr('Settings')
    }

    Flow {
        anchors.top: settingsHeader.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        spacing: 1

        Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width
            height: units.gu(4)
            text: i18n.tr('GPG')
        }
        PageStackLink {
            page: Qt.resolvedUrl("ImportKeyFile.qml")
            text: i18n.tr('Import a GPG key file')
        }
        PageStackLink {
            page: Qt.resolvedUrl("InfoKeys.qml")
            text: i18n.tr('Show GPG keys')
        }
        Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width
            height: units.gu(4)
            text: i18n.tr('Password Store')
        }
        PageStackLink {
            page: Qt.resolvedUrl("ImportZip.qml")
            text: i18n.tr('Import a Password Store Zip')
        }
        Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width
            height: units.gu(4)
            color: UbuntuColors.red
            text: i18n.tr(
                      'Warning: importing delete any exiting Password Store')
        }
    }
}
