import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import QtQuick 2.4

Dialog {
    id: dialog

    property string textError

    signal dialogClosed()

    title: i18n.tr("Error !")
    text: textError

    Button {
        text: i18n.tr("Close")
        onClicked: function() {
            dialogClosed();
            PopupUtils.close(dialog);
        }
    }

}
