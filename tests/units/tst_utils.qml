import QtQuick 2.9
import QtTest 1.2
import TestsUtils 1.0
import Utils 1.0

TestCase {
    function test_unzip() {
        var tempPath = TestUtils.getTempPath() + "/password-store";
        var zipUrl = Qt.resolvedUrl("../assets/archive.zip");
        var r = Utils.unzip(zipUrl, tempPath);
        verify(r, "Unzip return an error %1".arg(zipUrl));
    }

    name: "utils"
}
