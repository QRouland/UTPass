import QtTest 1.0
import Ubuntu.Test 1.0
import Pass 1.0

UbuntuTestCase {
    name: "GpgTests"
    function initTestCase() {
        Pass.gpgSetGpghome("tests/tmp/gnuhome")
    }

    function test_empty_gnuhome() {
        var listKeys = Pass.gpgListAllKeys()
        verify(listKeys.length == 0)
    }

    function test_import_key_form_file_data() {
        return [{
                    "tag": "public1.key",
                    "path": "tests/unit/assets/public1.key",
                    "answer": true
                }, {
                    "tag": "private1.key",
                    "path": "tests/unit/assets/private1.key",
                    "answer": true
                }, {
                    "tag": "bad path",
                    "path": "this/is/a/bad/path",
                    "answer": false
                }, {
                    "tag": "bad key",
                    "path": "tests/unit/assets/not.key",
                    "answer": false
                }]
    }

    function test_import_key_form_file(data) {
        compare(Pass.gpgImportKeyFromFile(data.path), data.answer)
    }
}
