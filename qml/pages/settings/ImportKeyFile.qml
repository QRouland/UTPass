import "../../components"
import Pass 1.0

import Lomiri.Content 1.3

ImportFile {
    id: importKeyFilePage

    headerTitle: i18n.tr("GPG Key Import")
    dialogSuccessTxt : i18n.tr("Key successfully imported !")
    dialogErrorTxt : i18n.tr("Key import failed !")

    contentPicker.onPeerSelected: {
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
                       PopupUtils.open(importKeyFilePage.dialogImportKeyPageSucess);
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
                       PopupUtils.open(importKeyFilePage.dialogImportKeyPageError);
                   });
               }
           });
       }
    }
}
