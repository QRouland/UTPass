import Git 1.0
import Lomiri.Components 1.3
import Lomiri.Components.Pickers 1.3
import QtQuick 2.4

OptionSelector {
    id: combo

    width: parent.width
    model: ["HTTP", "HTTP AUTH"]
    onDelegateClicked: function(i) {
        if (i === 0)
            timer.setTimeout(function() {
            importGitCloneForm.source = Qt.resolvedUrl("GitCloneHttp.qml");
        }, 500);
        else if (i === 1)
            timer.setTimeout(function() {
            importGitCloneForm.source = Qt.resolvedUrl("GitCloneHttpAuth.qml");
        }, 500);
    }

    Timer {
        id: timer

        function setTimeout(cb, delayTime) {
            timer.interval = delayTime;
            timer.repeat = false;
            timer.triggered.connect(cb);
            timer.triggered.connect(function release() {
                timer.triggered.disconnect(cb); // This is important
                timer.triggered.disconnect(release); // This is important as well
            });
            timer.start();
        }

    }

}
