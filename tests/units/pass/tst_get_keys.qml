import Pass 1.0
import QtQuick 2.9
import QtTest 1.2
import TestsUtils 1.0

PassTestCase {
    //TODO some additionanl error test

    function init_data() {
        return [{
            "spy": getAllGPGKeysSucceed,
            "signal": Pass.getAllGPGKeysSucceed,
            "err_msg": null,
            "add_home_gpg_data": false,
            "keys": []
        }, {
            "spy": getAllGPGKeysSucceed,
            "signal": Pass.getAllGPGKeysSucceed,
            "err_msg": null,
            "add_home_gpg_data": true,
            "keys": [{
                "fingerprint": "F97476B6FA58A84B004E4616D4BAF1FDB7BA9ECC",
                "keyid": "D4BAF1FDB7BA9ECC",
                "userids": "UTPass Test <utpass@test.org>",
                "hasSecret": true
            }]
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
        verify(keys.length === data.keys.length, "Nb keys %1 but was excepted %2".arg(keys.length).arg(data.nb_keys));
        for (var i = 0; i < keys.length; i++) {
            console.info(keys.keys[i]);
            console.info(keys.keys[i].keyid);
            verify(keys.keys[i].fingerprint === data.keys[i].fingerprint, "fingerprint is %1 but was excepted %2".arg(keys.keys[i].fingerprint).arg(data.keys[i].fingerprint));
            verify(keys.keys[i].keyid === data.keys[i].keyid, "keyid is %1 but was excepted %2".arg(keys.keys[i].keyid).arg(data.keys[i].keyid));
            verify(keys.keys[i].userids[0] === data.keys[i].userids, "userids is %1 but was excepted %2".arg(keys.keys[i].userids[0]).arg(data.keys[i].userids));
            verify(keys.keys[i].hasSecret === data.keys[i].hasSecret, "hasSecret is %1 but was excepted %2".arg(keys.keys[i].hasSecret).arg(data.keys[i].hasSecret));
        }
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
