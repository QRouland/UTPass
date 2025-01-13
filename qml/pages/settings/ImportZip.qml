import "../../dialogs"
import "../headers"
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Lomiri.Content 1.3
import Pass 1.0
import QtQuick 2.4
import Utils 1.0

Page {
    id: importZipPage

    property var activeTransfer

    Component.onCompleted: {
        PopupUtils.open(importZipPageImportValidation, importZipPage);
    }

    ContentPeerPicker {
        anchors.top: importZipHeader.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: importZipPage.header.height
        width: parent.width
        visible: parent.visible
        showTitle: false
        contentType: ContentType.Text
        handler: ContentHandler.Source
        onPeerSelected: {
            peer.selectionType = ContentTransfer.Single;
            importZipPage.activeTransfer = peer.request();
            importZipPage.activeTransfer.stateChanged.connect(function() {
                if (importZipPage.activeTransfer.state === ContentTransfer.Charged) {
                    console.log("Charged");
                    console.log(importZipPage.activeTransfer.items[0].url);
                    var status = Utils.unzip(importZipPage.activeTransfer.items[0].url, Pass.getPasswordStore());
                    Utils.rmFile(importZipPage.activeTransfer.items[0].url);
                    if (status)
                        PopupUtils.open(dialogImportZipPageSuccess);
                    else
                        PopupUtils.open(dialogImportZipPageError);
                    importZipPage.activeTransfer = null;
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
        activeTransfer: importZipPage.activeTransfer
    }

    Component {
        id: importZipPageImportValidation

        SimpleValidationDialog {
            text: i18n.tr("Importing a new zip will delete<br>any existing password store!<br>Continue ?")
            continueText: i18n.tr("Yes")
            continueColor: theme.palette.normal.negative
            onCanceled: {
                pageStack.pop();
            }
        }

    }

    Component {
        id: dialogImportZipPageError

        ErrorDialog {
            textError: i18n.tr("Password store import failed !")
        }

    }

    Component {
        id: dialogImportZipPageSuccess

        SuccessDialog {
            textSuccess: i18n.tr("Password store sucessfully imported !")
            onDialogClosed: {
                pageStack.pop();
                pageStack.pop();
            }
        }

    }

    header: StackHeader {
        id: importZipHeader

        title: i18n.tr("Zip Password Store Import")
    }

}
