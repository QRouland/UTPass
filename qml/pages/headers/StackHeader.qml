import QtQuick 2.4
import Ubuntu.Components 1.3

PageHeader {
    id: stackHeader
    width: parent.width
    height: units.gu(6)
    title: i18n.tr("UTPass")

    contents: Item {
        height: parent.height
        width: parent.width
        Label {
            id: labelTitle
            text: stackHeader.title
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    leadingActionBar.height: units.gu(4)
    leadingActionBar.actions: [
        Action {
            id: backAction
            iconName: "back"
            text: "Back"
            onTriggered: {
                pageStack.pop()
            }
        }
    ]
}
