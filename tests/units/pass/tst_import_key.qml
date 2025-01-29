import QtQuick 2.9
import QtTest 1.2
import TestsUtils 1.0
import Pass 1.0

PassTestCase {
    function init_data() {
          return [
                { file: Qt.resolvedUrl("../../assets/gpg/test_key.gpg"), spy: importGPGKeySucceed, signal: Pass.importGPGKeySucceed, err_msg : null  }
              , { file: Qt.resolvedUrl("../../assets/gpg/test_key_do_not_exist.gpg"), spy: importGPGKeyFailed, signal: Pass.importGPGKeyFailed, err_msg : "Error reading file" }
              , { file: Qt.resolvedUrl("../../assets/gpg/test_key_invalid.gpg"), spy: importGPGKeyFailed, signal: Pass.importGPGKeyFailed, err_msg : "Bad format" }
          ];
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

    SignalSpy {
        id: importGPGKeyCancelled
        target: Pass
        signalName: "importGPGKeyCancelled"
    }

    function test_import_key(data) {
        var err_msg;

        data.signal.connect(function(message) {
            err_msg = message;
        });

        Pass.importGPGKey(data.file);

        data.spy.wait();
        if(data.err_msg) {
            verify(err_msg === data.err_msg, "Should return arg msg %1 but return %2".arg(data.err_msg).arg(err_msg));
        }
    }
}
