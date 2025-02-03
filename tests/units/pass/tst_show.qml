import Pass 1.0
import QtQuick 2.9
import QtTest 1.2
import TestsUtils 1.0

PassTestCase {
    //TODO some additionanl error test

    function init_data() {
        return [{
            "spy": showFailed,
            "err_msg": "Bad password",
            "add_home_gpg_data": true,
            "file": "../../assets/gpg/clear_text.txt.gpg"
        }, {
            "spy": showFailed,
            "err_msg": "No suitable key",
            "add_home_gpg_data": false,
            "file": "../../assets/gpg/clear_text.txt.gpg"
        }];
    }

    function test_pass_show(data) {
        if (data.add_home_gpg_data === true) {
            TestsUtils.copyFolder(Qt.resolvedUrl("../../assets/gpghome"), Qt.resolvedUrl(gpg_home));
        }

        var fname, ctext;
        Pass.showSucceed.connect(function(file_name, clear_text) {
            fname = file_name;
            ctext = clear_text;
        });
        var err_msg;
        Pass.showFailed.connect(function(err) {
            err_msg = err;
        });

        Pass.show(Qt.resolvedUrl(data.file));
        data.spy.wait();
        if (data.err_msg) {
            verify(err_msg === data.err_msg, "Should return %1 but return %2".arg(data.err_msg).arg(err_msg));
        } else {
            verify(false);
        }
    }

    SignalSpy {
        id: showSucceed

        target: Pass
        signalName: "showSucceed"
    }

    SignalSpy {
        id: showFailed

        target: Pass
        signalName: "showFailed"
    }

}
