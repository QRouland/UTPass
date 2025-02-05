import Pass 1.0
import QtQuick 2.9
import QtTest 1.2
import TestsUtils 1.0

PassTestCase {
    //TODO some additionanl error test

    function init_data() {
        return [{
            "spy": lsSucceed,
            "add_home_gpg_data": true,
            "passwords": ["test.gpg"]
        }, {
            "spy": lsSucceed,
            "add_home_gpg_data": false,
            "passwords": []
        }];
    }

    function test_ls(data) {
        if (data.add_home_gpg_data === true)
            TestsUtils.copyFolder(Qt.resolvedUrl("../../assets/password-store"), Qt.resolvedUrl(password_store));

        var passwords;
        Pass.lsSucceed.connect(function(ret) {
            passwords = ret;
        });
        Pass.ls();
        data.spy.wait();
        verify(passwords.length === data.passwords.length, "Should return %1 password(s) but return %2 password(s)".arg(data.nb_password).arg(passwords.length));
        for (var i = 0; data.passwords.length; i++) {
            verify(passwords[i] === data.passwords[i], "%1 name should be %2 but is %3".arg(i).arg(data.passwords[i]).arg(passwords[i]));
        }
    }

    SignalSpy {
        id: lsSucceed

        target: Pass
        signalName: "lsSucceed"
    }

}
