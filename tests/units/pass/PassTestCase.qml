import QtQuick 2.9
import QtTest 1.2
import TestsUtils 1.0
import Pass 1.0

TestCase {
    function init() {
        Pass.initialize(null);
        Pass.gpg_home = TestsUtils.getTempPath();
        Pass.password_store = TestsUtils.getTempPath();
        Pass.passphrase_provider = TestsUtils.getTestPassphraseProvider();
    }
}
