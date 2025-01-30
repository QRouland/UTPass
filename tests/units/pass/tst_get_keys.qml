import Pass 1.0
import QtQuick 2.9
import QtTest 1.2
import TestsUtils 1.0

PassTestCase {
    function init_data() {
        //TODO some additionanl error test

        return [{
            "spy": getAllGPGKeysSucceed,
            "signal": Pass.getAllGPGKeysSucceed,
            "err_msg": null,
            "add_home_gpg_data": false,
            "nb_keys": 0
        }, {
            "spy": getAllGPGKeysSucceed,
            "signal": Pass.getAllGPGKeysSucceed,
            "err_msg": null,
            "add_home_gpg_data": true,
            "nb_keys": 2
        }];
    }

    function test_get_keys(data) {
        if (data.add_home_gpg_data === true)
            TestsUtils.copyFolder(Qt.resolvedUrl("../../assets/gpghome"), Qt.resolvedUrl(gpg_home));

        var keys;
        data.signal.connect(function(keys_info) {
            keys = keys_info;
        });
        Pass.getAllGPGKeys();
        data.spy.wait();
        verify(keys.length === data.nb_keys, "Nb keys %1 but was excepted %2".arg(keys.length).arg(data.nb_keys));
    }

    SignalSpy {
        id: getAllGPGKeysSucceed

        target: Pass
        signalName: "getAllGPGKeysSucceed"
    }

    SignalSpy {
        id: getAllGPGKeysFailed

        target: Pass
        signalName: "getAllGPGKeysFailed"
    }

}
