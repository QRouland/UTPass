import Git 1.0
import QtQuick 2.9
import QtTest 1.2

TestCase {
    function test_git_clone() {
        verify(Git.clone("", ""));
    }

    name: "git"
}
