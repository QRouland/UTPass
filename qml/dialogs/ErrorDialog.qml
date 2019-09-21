import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3

Dialog {
    id: dialogSuccess

    property string textError

    signal dialogClosed

    title: i18n.tr("Error !")
    text: textError
    Button {
        text: i18n.tr("OK")
        color: UbuntuColors.red
        onClicked: function () {
            dialogClosed()
            PopupUtils.close(dialogSuccess)
        }
    }
}
