import QtQuick 2.4
import Ubuntu.Components 1.3

PageHeader {
    id: mainHeader
    width: parent.width
    height: units.gu(6)
    title: i18n.tr("UTPass")

    trailingActionBar.height: units.gu(4)
    trailingActionBar.numberOfSlots: 2
    trailingActionBar.actions: [
        Action {
            iconName: "search"
            text: i18n.tr("Search")
        },
        Action {
            iconName: "settings"
            text: i18n.tr("Settings")
            onTriggered: {
                pageStack.push(Qt.resolvedUrl("../Settings.qml"))
            }
        },
        Action {
            iconName: "info"
            text: i18n.tr("Info")
            onTriggered: {
                pageStack.push(Qt.resolvedUrl("../Info.qml"))
            }
        }
    ]
}