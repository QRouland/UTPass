import Pass 1.0
import QtQuick 2.9
import QtTest 1.2
import TestsUtils 1.0

PassTestCase {
    function init_data() {
        return [{
            "file": Qt.resolvedUrl("../../assets/gpg/test_key.gpg"),
            "spy": importGPGKeySucceed,
            "err_msg": null
        }, {
            "file": Qt.resolvedUrl("../../assets/gpg/test_key_do_not_exist.gpg"),
            "spy": importGPGKeyFailed,
            "err_msg": "Error reading file"
        }, {
            "file": Qt.resolvedUrl("../../assets/gpg/test_key_invalid.gpg"),
            "spy": importGPGKeyFailed,
            "err_msg": "Bad state"
        }];
    }

    function test_import_key(data) {
        var err_msg;
        Pass.importGPGKeyFailed.connect(function(message) {
            err_msg = message;
        });
        Pass.importGPGKey(data.file);
        data.spy.wait();
        if (data.err_msg) {
            verify(err_msg === data.err_msg, "Should return %1 but return %2".arg(data.err_msg).arg(err_msg));
        } else {
            console.info(Qt.resolvedUrl("%1/pubkeyring.pgp".arg(gpg_home)));
            verify(TestsUtils.fileExists(Qt.resolvedUrl("%1/pubring.pgp".arg(gpg_home))), "%1/pubring.pgp should be created".arg(gpg_home));
            verify(TestsUtils.fileExists(Qt.resolvedUrl("%1/secring.pgp".arg(gpg_home))), "%1/secring.pgp should be created".arg(gpg_home));
        }
    }

    SignalSpy {
        id: importGPGKeySucceed

        target: Pass
        signalName: "importGPGKeySucceed"
    }

    SignalSpy {
        id: importGPGKeyFailed

        target: Pass
        signalName: "importGPGKeyFailed"
    }

}
