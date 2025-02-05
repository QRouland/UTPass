import Pass 1.0
import QtQuick 2.9
import QtTest 1.2
import TestsUtils 1.0

TestCase {
    //Pass.passphrase_provider = TestsUtils.getTestPassphraseProvider();

    property string password_store
    property string gpg_home

    function init() {
        Pass.initialize(null);
        gpg_home = TestsUtils.getTempPath();
        Pass.gpg_home = gpg_home;
        password_store = TestsUtils.getTempPath();
        Pass.password_store = password_store;
    }

}
