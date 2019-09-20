import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3

Dialog {
    id: dialogSuccess

    property string textSuccess

    signal dialogClosed

    title: i18n.tr("Success !")
    text: textSuccess
    Button {
        text: i18n.tr("OK")
        color: UbuntuColors.green
        onClicked: function () {
            dialogClosed()
            PopupUtils.close(dialogSuccess)
        }
    }
}
