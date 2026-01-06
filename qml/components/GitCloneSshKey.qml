import Git 1.0
import Lomiri.Components 1.3
import Pass 1.0
import QtQuick 2.4
import Utils 1.0

Column {
    property alias importSshPrivKeyButton: repoImportPrivKeyButton
    property alias importSshPubKeyButton: repoImportPubKeyButton
    property alias deleteSshPrivKeyButton: repoDeletePrivKeyButton
    property alias deleteSshPubKeyButton: repoDeletePubKeyButton
    property bool __sshPrivKeyAvailable: false
    property bool __sshPubKeyAvailable: false

    signal repoUrlChanged(string url)

    function setRepoUrl(url) {
        repoUrlInput.text = url;
    }

    function update() {
        __sshPrivKeyAvailable = Utils.fileExists(Git.privKey);
        __sshPubKeyAvailable = Utils.fileExists(Git.pubKey);
    }

    Component.onCompleted: {
        update();
    }
    anchors.top: parent.fill
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
        placeholderText: "<username>@<hostname>"
        onTextChanged: repoUrlChanged(repoUrlInput.text)
    }

    Text {
        id: repoPrivKeyLabel

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        text: i18n.tr('SSH private key')
        color: theme.palette.normal.backgroundText
    }

    Button {
        id: repoImportPrivKeyButton

        width: parent.width
        color: theme.palette.normal.positive
        text: i18n.tr('Import SSH private key')
        visible: !__sshPrivKeyAvailable
    }

    Button {
        id: repoDeletePrivKeyButton

        width: parent.width
        color: theme.palette.normal.negative
        text: i18n.tr('Delete SSH private key')
        visible: __sshPrivKeyAvailable
    }

    Text {
        id: repoPubKeyLabel

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        text: i18n.tr('SSH public key')
        color: theme.palette.normal.backgroundText
    }

    Button {
        id: repoDeletePubKeyButton

        width: parent.width
        color: theme.palette.normal.negative
        text: i18n.tr('Delete SSH public key')
        visible: __sshPrivKeyAvailable
    }

    Button {
        id: repoImportPubKeyButton

        width: parent.width
        color: theme.palette.normal.positive
        text: i18n.tr('Import SSH public key')
        visible: !__sshPrivKeyAvailable
    }

    Text {
        id: repoPassphraseLabel

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        text: i18n.tr('Passphrase')
        color: theme.palette.normal.backgroundText
    }

    TextField {
        id: repoPassphraseInput

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        echoMode: TextInput.Password
        placeholderText: i18n.tr('Passphrase')
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
            Git.cloneSshKey(repoUrlInput.text, Pass.password_store, repoPassphraseInput.text);
        }
    }

}
