import Git 1.0
import Lomiri.Components 1.3
import Pass 1.0
import QtQuick 2.4

Column {
    width: parent.width
    spacing: units.gu(1)

    Text {
        id: repoUrlLabel

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        text: i18n.tr('Repo Url')
        color: theme.palette.normal.backgroundText
    }

    TextField {
        id: repoUrlInput

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        placeholderText: "http(s)://<hostname>"
    }

    Rectangle {
        width: parent.width
        height: units.gu(1)
        color: theme.palette.normal.background
    }

    Button {
        id: buttonClone

        width: parent.width
        color: theme.palette.normal.positive
        text: i18n.tr('Clone')
        onClicked: {
            Git.cloneHttp(repoUrlInput.text, Pass.password_store);
        }
    }

}
