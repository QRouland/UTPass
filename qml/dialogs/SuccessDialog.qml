import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import QtQuick 2.4

Dialog {
    id: dialogSuccess

    property string textSuccess

    signal dialogClosed()

    title: i18n.tr("Success !")
    text: textSuccess

    Button {
        text: i18n.tr("OK")
        color: LomiriColors.green
        onClicked: function() {
            dialogClosed();
            PopupUtils.close(dialogSuccess);
        }
    }

}
