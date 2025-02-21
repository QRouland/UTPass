import "../dialogs"
import "../pages/headers"
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Lomiri.Content 1.3
import Pass 1.0
import QtQuick 2.4
import Utils 1.0

Page {
    id: importKeyFilePage

    property var activeTransfer
    property alias contentPicker : contentPicker
    property alias dialogImportKeyPageError : dialogImportKeyPageError
    property alias dialogImportKeyPageSucess : dialogImportKeyPageSucess

    property string headerTitle : i18n.tr("Import succeeded !")
    property string dialogErrorTxt : i18n.tr("Import failed !")
    property string dialogSuccessTxt : i18n.tr("File Import")

    ContentPeerPicker {
        id: contentPicker
        anchors.top: importKeyHeader.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: importKeyFilePage.header.height
        width: parent.width
        visible: parent.visible
        showTitle: false
        contentType: ContentType.Text
        handler: ContentHandler.Source
        onCancelPressed: {
            pageStack.pop();
        }
    }

    ContentTransferHint {
        id: transferHint

        anchors.fill: parent
        activeTransfer: importKeyFilePage.activeTransfer
    }

    Component {
        id: dialogImportKeyPageError

        ErrorDialog {
            textError: importKeyFilePage.dialogErrorTxt
        }

    }

    Component {
        id: dialogImportKeyPageSucess

        SuccessDialog {
            textSuccess: importKeyFilePage.dialogSuccessTxt
            onDialogClosed: {
                pageStack.pop();
            }
        }

    }

    header: StackHeader {
        id: importKeyHeader

        title: importKeyFilePage.headerTitle
    }

}
