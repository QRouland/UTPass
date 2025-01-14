import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Pass 1.0
import QtQuick 2.4
import QtQuick.Layouts 1.1
import "dialogs"

MainView {
    //theme.name: "Lomiri.Components.Themes.SuruDark"

    id: root

    signal responsePassphraseDialog(bool canceled, string passphrase)

    function initPass(rootView) {
        Pass.initialize(rootView);
        pageStack.push(Qt.resolvedUrl("pages/PasswordList.qml"));
    }

    function callPassphraseDialog(useridHint, description, previousWasBad) {
        //TODO use parameters to impove passphrase dialog
        var passphraseDialog = PopupUtils.open(Qt.resolvedUrl("dialogs/PassphraseDialog.qml"));
        passphraseDialog.activateFocus();
        var validated = function validated(passphrase) {
            responsePassphraseDialog(false, passphrase);
        };
        var canceled = function canceled() {
            responsePassphraseDialog(true, "");
        };
        passphraseDialog.validated.connect(validated);
        passphraseDialog.canceled.connect(canceled);
    }

    objectName: "mainView"
    applicationName: "utpass.qrouland"
    automaticOrientation: false
    width: units.gu(48)
    height: units.gu(80)

    PageStack {
        id: pageStack

        anchors.fill: parent
        Component.onCompleted: {
        }
    }

}
