import QtQuick 2.9
import QtTest 1.2
import Git 1.0

TestCase {
    name: "git"

    function test_git_clone() {

        verify(Git.clone("",""));
    }
}
