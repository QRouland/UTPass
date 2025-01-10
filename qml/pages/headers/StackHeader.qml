import Lomiri.Components 1.3
import QtQuick 2.4

PageHeader {
    id: stackHeader

    width: parent.width
    height: units.gu(6)
    title: i18n.tr("UTPass")
    leadingActionBar.height: units.gu(4)
    leadingActionBar.actions: [
        Action {
            id: backAction

            iconName: "back"
            text: "Back"
            onTriggered: {
                pageStack.pop();
            }
        }
    ]

    contents: Item {
        height: parent.height
        width: parent.width

        Label {
            id: labelTitle

            text: stackHeader.title
            anchors.verticalCenter: parent.verticalCenter
        }

    }

}
