import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3

Dialog {
    id: passphraseProvider
    title: i18n.tr("Authentication required")
    text: i18n.tr("Enter passphrase:")

    signal validated(string passphrase)
    signal canceled

    function activateFocus() {
        passphraseField.forceActiveFocus()
    }

    TextField {
        id: passphraseField

        placeholderText: i18n.tr("passphrase")
        echoMode: TextInput.Password

        onAccepted: okButton.clicked(text)
    }

    Button {
        id: okButton

        text: i18n.tr("Ok")
        color: UbuntuColors.green

        onClicked: {
            validated(passphraseField.text)
            passphraseField.text = ""
            PopupUtils.close(passphraseProvider)
        }
    }

    Button {
        id: cancelButton
        text: i18n.tr("Cancel")

        color: UbuntuColors.red

        onClicked: {
            canceled()
            PopupUtils.close(passphraseProvider)
        }
    }
}
