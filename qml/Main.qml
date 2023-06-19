import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Pass 1.0
import "dialogs"

MainView {
    id: root
    objectName: "mainView"
    applicationName: "utpass.qrouland"

    automaticOrientation: false

    width: units.gu(48)
    height: units.gu(80)

    signal responsePassphraseDialog(bool canceled, string passphrase)

    function initPass(rootView) {
        Pass.init(rootView)
        pageStack.push(Qt.resolvedUrl("pages/PasswordList.qml"));
    }

    function callPassphraseDialog(useridHint, description, previousWasBad) {
        //TODO use parameters to impove passphrase dialog
        var passphraseDialog = PopupUtils.open(
                    Qt.resolvedUrl("dialogs/PassphraseDialog.qml"))
        passphraseDialog.activateFocus()

        var validated = function (passphrase) {
            responsePassphraseDialog(false, passphrase)
        }

        var canceled = function () {
            responsePassphraseDialog(true, "")
        }

        passphraseDialog.validated.connect(validated)
        passphraseDialog.canceled.connect(canceled)
    }

    PageStack {
        id: pageStack
        anchors.fill: parent

        Component.onCompleted: {}
    }
}
