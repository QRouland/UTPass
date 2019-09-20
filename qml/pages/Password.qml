import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import "headers"

Page {
    id: passwordPage

    property string title
    property string plainText
    property var objects

    header: PageHeader {
        id: passwordPageHeader
        width: parent.width
        height: units.gu(6)
        title: passwordPage.title

        contents: Item {
            height: parent.height
            width: parent.width
            Label {
                id: labelTitle
                text: passwordPage.title
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
                    passwordPage.plainText = ""
                    for (var object in objects) {
                        object.text = ""
                        object.destroy()
                    }
                    pageStack.pop()
                }
            }
        ]
    }

    Rectangle {
        anchors.top: passwordPageHeader.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left

        Flow {
            id: container
            anchors.fill: parent
        }
    }

    Component.onCompleted: {
        var text_split = passwordPage.plainText.split('\n')
        var component = Qt.createComponent("../components/CopyText.qml")
        for (var i = 0; i < text_split.length; i++) {
            if (text_split[i]) {
                var object = component.createObject(container)
                object.text = text_split[i]
            }
        }
    }
}
