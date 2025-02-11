import Lomiri.Components 1.3
import QtQuick 2.4

PageHeader {
    id: mainHeader

    property alias searchBar: searchBar
    //property alias searchBarText: searchBar.text
    //signal searchBarTextChanged(string text)

    width: parent.width
    height: units.gu(6)
    title: i18n.tr("UTPass")
    trailingActionBar.height: units.gu(4)
    trailingActionBar.numberOfSlots: 2
    trailingActionBar.actions: [
        Action {
            iconName: !searchBar.visible ? "search" : "close"
            text: i18n.tr("Search")
            onTriggered: {
                searchBar.visible = !searchBar.visible;
                labelTitle.visible = !searchBar.visible;
                if (searchBar.visible === true) {
                    searchBar.focus = true;
                }
            }
        },
        Action {
            iconName: "settings"
            text: i18n.tr("Settings")
            onTriggered: {
                pageStack.push(Qt.resolvedUrl("../settings/Settings.qml"));
            }
        },
        Action {
            iconName: "info"
            text: i18n.tr("Info")
            onTriggered: {
                pageStack.push(Qt.resolvedUrl("../Info.qml"));
            }
        }
    ]

    contents: Item {
        height: parent.height
        width: parent.width

        Label {
            id: labelTitle

            text: mainHeader.title
            anchors.verticalCenter: parent.verticalCenter
            visible: true
        }

        TextField {
            id: searchBar

            anchors.right: parent.right
            anchors.left: parent.left
            placeholderText: i18n.tr("Search")
            height: units.gu(4)
            visible: false
            anchors.verticalCenter: parent.verticalCenter
        }

    }

}
