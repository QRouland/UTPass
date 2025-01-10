import QtQuick 2.4
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3

Dialog {
    id: dialogError

    property string textError

    signal dialogClosed

    title: i18n.tr("Error !")
    text: textError
    Button {
        text: i18n.tr("Close")
        color: LomiriColors.red
        onClicked: function () {
            dialogClosed()
            PopupUtils.close(dialogError)
        }
    }
}
