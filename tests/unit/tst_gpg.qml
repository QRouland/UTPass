import QtTest 1.0
import Ubuntu.Test 1.0
import Gpg 1.0

UbuntuTestCase {
   name: "GpgTests"
   function test_empty_gnuhome() {
       Gpg::getListIds().empty()
   }
}
