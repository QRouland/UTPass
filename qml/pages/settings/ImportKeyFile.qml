import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Content 1.3
import Ubuntu.Components.Popups 1.3
import Pass 1.0
import Utils 1.0
import "../headers"
import "../../dialogs"

Page {
    id: importKeyFilePage

    property var activeTransfer

    header: StackHeader {
        id: importKeyHeader
        title: i18n.tr("GPG Key Import")
    }

    ContentPeerPicker {
        anchors.top: importKeyHeader.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: importKeyFilePage.header.height

        width: parent.width

        visible: parent.visible
        showTitle: false
        contentType: ContentType.Text
        handler: ContentHandler.Source

        onPeerSelected: {
            peer.selectionType = ContentTransfer.Single
            importKeyFilePage.activeTransfer = peer.request()

            importKeyFilePage.activeTransfer.stateChanged.connect(function () {
                if (importKeyFilePage.activeTransfer.state === ContentTransfer.Charged) {
                    console.log("Charged")
                    console.log(importKeyFilePage.activeTransfer.items[0].url)

                    var status = Pass.gpgImportKeyFromFile(
                                importKeyFilePage.activeTransfer.items[0].url)

                    Utils.rmFile(importKeyFilePage.activeTransfer.items[0].url)
                    if (status) {
                        PopupUtils.open(dialogImportKeyPageSucess)
                    } else {
                        PopupUtils.open(dialogImportKeyPageError)
                    }
                    importKeyFilePage.activeTransfer = null
                }
            })
        }

        onCancelPressed: {
            pageStack.pop()
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
            textError: i18n.tr("Key import failed !")
        }
    }

    Component {
        id: dialogImportKeyPageSucess
        SuccessDialog {
            textSuccess: i18n.tr("Key successfully imported !")
            onDialogClosed: {
                pageStack.pop()
            }
        }
    }
}
