import "../../components"
import Git 1.0
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Lomiri.Content 1.3
import Utils 1.0

ImportFile {
    id: importSSHKeyPage

    property bool isPrivateKey

    headerTitle: i18n.tr("SSH Key Import")
    dialogSuccessTxt: i18n.tr("SSH Key successfully imported !")
    dialogErrorTxt: i18n.tr("SSH Key import failed !")
    contentPicker.onPeerSelected: {
        {
            importSSHKeyPage.contentPicker.peer.selectionType = ContentTransfer.Single;
            importSSHKeyPage.activeTransfer = importSSHKeyPage.contentPicker.peer.request();
            importSSHKeyPage.activeTransfer.stateChanged.connect(function() {
                if (importSSHKeyPage.activeTransfer.state === ContentTransfer.Charged) {
                    console.log("Charged");
                    console.log(importSSHKeyPage.activeTransfer.items[0].url);
                    var ret = Git.importSshKey(importSSHKeyPage.activeTransfer.items[0].url, isPrivateKey);
                    Utils.rmFile(importSSHKeyPage.activeTransfer.items[0].url);
                    importSSHKeyPage.activeTransfer = null;
                    if (ret)
                        PopupUtils.open(importSSHKeyPage.dialogImportKeyPageSucess);
                    else
                        PopupUtils.open(importSSHKeyPage.dialogImportKeyPageError);
                }
            });
        };
    }
}
