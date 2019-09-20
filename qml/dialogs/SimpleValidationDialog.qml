import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3

Dialog {
    id: doubleValidationDialog

    property string text

    signal validated
    signal canceled

    Text {
        horizontalAlignment: Text.AlignHCenter
        text: doubleValidationDialog.text
    }

    Button {
        text: i18n.tr("Ok")
        color: UbuntuColors.green

        onClicked: {
            validated()
            PopupUtils.close(doubleValidationDialog)
        }
    }

    Button {
        id: cancelButton
        text: i18n.tr("Cancel")
        color: UbuntuColors.red
        onClicked: {
            canceled()
            PopupUtils.close(doubleValidationDialog)
        }
    }
}
