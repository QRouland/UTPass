import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import QtQuick 2.4

Dialog {
    id: dialog

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
        color: theme.palette.normal.positive
        onClicked: {
            validated(passphraseField.text);
            passphraseField.text = "";
            PopupUtils.close(dialog);
        }
    }

    Button {
        id: cancelButton

        text: i18n.tr("Cancel")
        onClicked: {
            canceled();
            PopupUtils.close(dialog);
        }
    }

}
