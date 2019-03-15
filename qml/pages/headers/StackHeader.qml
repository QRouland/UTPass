import QtQuick 2.4
import Ubuntu.Components 1.3

PageHeader {
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
                pageStack.pop()
            }
        }
    ]
}
