import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import QtQuick 2.4

Dialog {
    id: doubleValidationDialog

    property string text

    signal validated()
    signal canceled()

    Text {
        horizontalAlignment: Text.AlignHCenter
        text: doubleValidationDialog.text
    }

    Button {
        text: i18n.tr("Ok")
        color: LomiriColors.green
        onClicked: {
            validated();
            PopupUtils.close(doubleValidationDialog);
        }
    }

    Button {
        id: cancelButton

        text: i18n.tr("Cancel")
        color: LomiriColors.red
        onClicked: {
            canceled();
            PopupUtils.close(doubleValidationDialog);
        }
    }

}
