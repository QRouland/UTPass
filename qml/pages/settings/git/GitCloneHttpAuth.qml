import Git 1.0
import Lomiri.Components 1.3
import Pass 1.0
import QtQuick 2.4

Column {
    anchors.top: parent.fill
    spacing: units.gu(1)

    Text {
        id: repoUrlLabel

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        text: i18n.tr('Repo Url')
    }

    TextField {
        id: repoUrlInput

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
    }

    Text {
        id: repoPasswordLabel

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        text: i18n.tr('Password')
    }

    TextField {
        id: repoPasswordInput

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        echoMode: TextInput.Password
    }

    Button {
        id: buttonClone

        width: parent.width
        color: theme.palette.normal.positive
        text: i18n.tr('Clone')
        onClicked: {
            Git.cloneHttpPass(repoUrlInput.text, Pass.password_store, repoPasswordInput.text);
        }
    }

}
