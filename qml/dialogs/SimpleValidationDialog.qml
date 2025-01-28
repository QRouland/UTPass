import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import QtQuick 2.4

Dialog {
    id: dialog

    property string continueText: i18n.tr("Ok")
    property color continueColor: theme.palette.normal.positive

    signal validated()
    signal canceled()


    Button {
        id: continueButton

        text: dialog.continueText
        color: dialog.continueColor
        onClicked: {
            validated();
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
