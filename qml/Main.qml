import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Pass 1.0
import QtQuick 2.4
import QtQuick.Layouts 1.1
import "dialogs"

MainView {
    //theme.name: "Lomiri.Components.Themes.SuruDark"

    id: root

    function initPass(rootView) {
        Pass.initialize(rootView);
        pageStack.push(Qt.resolvedUrl("pages/PasswordList.qml"));
    }

    function callPassphraseDialog(useridHint, description, previousWasBad) {
        //TODO use parameters to impove passphrase dialog
        var pop = PopupUtils.open(passphraseDialog);
        pop.activateFocus();
    }

    objectName: "mainView"
    applicationName: "utpass.qrouland"
    automaticOrientation: true

    width: units.gu(45)
    height: units.gu(75)

    PageStack {
        id: pageStack

        anchors.fill: parent
    }

    Component {
        id: passphraseDialog

        PassphraseDialog {
            onValidated: {
                console.info("valided");
                Pass.responsePassphraseDialog(false, passphrase);
            }
            onCanceled: {
                console.info("canceled");
                Pass.responsePassphraseDialog(true, "");
            }
        }

    }
}
