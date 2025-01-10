import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import QtQuick 2.4

Dialog {
    id: passphraseProvider

    signal validated(string passphrase)
    signal canceled()

    function activateFocus() {
        passphraseField.forceActiveFocus();
    }

    title: i18n.tr("Authentication required")
    text: i18n.tr("Enter passphrase:")

    TextField {
        id: passphraseField

        placeholderText: i18n.tr("passphrase")
        echoMode: TextInput.Password
        onAccepted: okButton.clicked(text)
    }

    Button {
        id: okButton

        text: i18n.tr("Ok")
        color: LomiriColors.green
        onClicked: {
            validated(passphraseField.text);
            passphraseField.text = "";
            PopupUtils.close(passphraseProvider);
        }
    }

    Button {
        id: cancelButton

        text: i18n.tr("Cancel")
        color: LomiriColors.red
        onClicked: {
            canceled();
            PopupUtils.close(passphraseProvider);
        }
    }

}
