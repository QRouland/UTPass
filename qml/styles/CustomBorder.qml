// Author : Amit Tomar
// @ https://stackoverflow.com/questions/16534489/qml-control-border-width-and-color-on-any-one-side-of-rectangle-element#16562823
import QtQuick 2.4

Rectangle {
    property bool commonBorder: true

    property int lBorderwidth: 1
    property int rBorderwidth: 1
    property int tBorderwidth: 1
    property int bBorderwidth: 1

    property int commonBorderWidth: 1

    z: -1

    property string borderColor: "white"

    color: borderColor

    anchors {
        left: parent.left
        right: parent.right
        top: parent.top
        bottom: parent.bottom

        topMargin: commonBorder ? -commonBorderWidth : -tBorderwidth
        bottomMargin: commonBorder ? -commonBorderWidth : -bBorderwidth
        leftMargin: commonBorder ? -commonBorderWidth : -lBorderwidth
        rightMargin: commonBorder ? -commonBorderWidth : -rBorderwidth
    }
}
