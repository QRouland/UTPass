import "../../components"
import "../headers"
import Lomiri.Components 1.3
import Pass 1.0
import QtQuick 2.4

Page {
    id: settingsPage

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
            color: theme.palette.normal.backgroundText
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
            color: theme.palette.normal.backgroundText
        }

        PageStackLink {
            page: Qt.resolvedUrl("ImportGitClone.qml")
            text: i18n.tr('Import a Password Store using Git')
        }

        PageStackLink {
            page: Qt.resolvedUrl("ImportZip.qml")
            text: i18n.tr('Import a Password Store Zip')
        }

        PageStackLink {
            page: Qt.resolvedUrl("DeleteRepo.qml")
            text: i18n.tr('Delete Password Store')
        }

        Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width
            height: units.gu(4)
            color: theme.palette.normal.negative
            text: i18n.tr('Warning: importing delete any exiting Password Store')
        }

    }

    header: StackHeader {
        id: settingsHeader

        title: i18n.tr('Settings')
    }

}
