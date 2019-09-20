import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3

Dialog {
    id: doubleValidationDialog

    property int nb_validation: 0
    property string text1
    property string text2

    signal doubleValidated
    signal canceled

    Text {
        visible: nb_validation == 0
        horizontalAlignment: Text.AlignHCenter
        text: text1
    }

    Text {
        visible: nb_validation == 1
        horizontalAlignment: Text.AlignHCenter
        text: text2
    }

    Button {
        text: i18n.tr("Ok")
        color: UbuntuColors.green

        onClicked: {
            if (nb_validation == 1) {
                nb_validation = 0
                doubleValidated()
                PopupUtils.close(doubleValidationDialog)
            } else {
                nb_validation += 1
            }
        }
    }

    Button {
        id: cancelButton
        text: i18n.tr("Cancel")
        color: UbuntuColors.red
        onClicked: {
            nb_validation = 0
            canceled()
            PopupUtils.close(doubleValidationDialog)
        }
    }
}
