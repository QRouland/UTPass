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
    property alias contentPicker : contentPicker

    property string __text_error_description :  null


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

        onPeerSelected: {
           {
               importKeyFilePage.contentPicker.peer.selectionType = ContentTransfer.Single;
               importKeyFilePage.activeTransfer = importKeyFilePage.contentPicker.peer.request();
               importKeyFilePage.activeTransfer.stateChanged.connect(function() {
                   if (importKeyFilePage.activeTransfer.state === ContentTransfer.Charged) {
                       console.log("Charged");
                       console.log(importKeyFilePage.activeTransfer.items[0].url);
                       Pass.importGPGKey(importKeyFilePage.activeTransfer.items[0].url);
                       Pass.importGPGKeySucceed.connect(function() {
                           Utils.rmFile(importKeyFilePage.activeTransfer.items[0].url);
                           importKeyFilePage.activeTransfer = null;
                           PopupUtils.open(dialogImportKeyPageSucess);
                       });
                       Pass.importGPGKeyFailed.connect(function(err, message) {
                           Utils.rmFile(importKeyFilePage.activeTransfer.items[0].url);
                           importKeyFilePage.activeTransfer = null;
                           switch (code) {
                               case 1: // UnexceptedError -> use the default (not translate) rnp error
                                   __text_error_description = message;
                                   break;
                               case 2: // BadFormat
                                    __text_error_description = i18n.tr("The file is not in a valid key format");
                                   break;
                               default:
                                   console.warn("Unhandled error code");
                                   __text_error_description = message;
                                   break;
                           }
                           PopupUtils.open(dialogImportKeyPageError);
                       });
                   }
               });
           }
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


    header: StackHeader {
        id: importKeyHeader

        title: i18n.tr("GPG Key Import")
    }

    Component {
        id: dialogImportKeyPageError

        ErrorDialog {
            textError: i18n.tr("Key import failed !")
            textErrorDescription: __text_error_description
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
}
