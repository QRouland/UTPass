import QtQuick 2.4

Item {
    TextField {
        id: searchBar
        placeholderText: i18n.tr("Search")
        height: units.gu(4)
        visible: false
        anchors.verticalCenter: parent.verticalCenter
    }
    Component.onCompleted: ;
}
