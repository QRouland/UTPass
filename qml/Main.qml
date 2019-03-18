import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Gpg 1.0

import "components"

MainView {
    id: root
    objectName: "mainView"
    applicationName: "utpass.qrouland"

    automaticOrientation: true

    width: units.gu(45)
    height: units.gu(75)
    PageStack {
        id: pageStack
        anchors.fill: parent

        Component.onCompleted: push(pageStack.push(Qt.resolvedUrl("pages/PasswordList.qml")))
    }
    Component.onCompleted: { Gpg.import_key("password-store/public.key"); Gpg.get_all_keys_id();}
}
