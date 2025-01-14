import "../../dialogs"
import "../headers"
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Lomiri.Content 1.3
import Pass 1.0
import QtQuick 2.4
import Utils 1.0

Page {
    id: importKeyFilePage

    property var activeTransfer

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
            peer.selectionType = ContentTransfer.Single;
            importKeyFilePage.activeTransfer = peer.request();
            importKeyFilePage.activeTransfer.stateChanged.connect(function() {
                if (importKeyFilePage.activeTransfer.state === ContentTransfer.Charged) {
                    console.log("Charged");
                    console.log(importKeyFilePage.activeTransfer.items[0].url);
                    var status = Pass.importGPGKey(importKeyFilePage.activeTransfer.items[0].url);
                    Utils.rmFile(importKeyFilePage.activeTransfer.items[0].url);
                    if (status)
                        PopupUtils.open(dialogImportKeyPageSucess);
                    else
                        PopupUtils.open(dialogImportKeyPageError);
                    importKeyFilePage.activeTransfer = null;
                }
            });
        }
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
            textError: i18n.tr("Key import failed !")
        }

    }

    Component {
        id: dialogImportKeyPageSucess

        SuccessDialog {
            textSuccess: i18n.tr("Key successfully imported !")
            onDialogClosed: {
                pageStack.pop();
            }
        }

    }

    header: StackHeader {
        id: importKeyHeader

        title: i18n.tr("GPG Key Import")
    }

}
