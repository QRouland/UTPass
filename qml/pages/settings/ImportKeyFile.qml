import "../../components"
import Pass 1.0

ImportFile {

    id: importKeyFilePage

    headerTitle: i18n.tr("GPG Key Import")
    dialogSuccessTxt : i18n.tr("Key successfully imported !")
    dialogErrorTxt : i18n.tr("Key import failed !")

    contentPicker.onPeerSelected: {
       {
           peer.selectionType = ContentTransfer.Single;
           importKeyFilePage.activeTransfer = peer.request();
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
                   Pass.importGPGKeyFailed.connect(function(message) {
                       Utils.rmFile(importKeyFilePage.activeTransfer.items[0].url);
                       importKeyFilePage.activeTransfer = null;
                       PopupUtils.open(importKeyFilePage.dialogImportKeyPageError);
                   });
               }
           });
       }
    }
}
