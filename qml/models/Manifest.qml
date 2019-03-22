pragma Singleton
import QtQuick 2.4

Item {
	id: manifest
	property string name : ""
	property string description : ""
	property string architecture : ""
	property string title : ""
	property string version : ""
	property string maintainer : ""
	property string framework : ""
	
    Component.onCompleted: {
		var xhr = new XMLHttpRequest;
		xhr.open("GET", "../../manifest.json");
		xhr.onreadystatechange = function() {
			if (xhr.readyState == XMLHttpRequest.DONE) {
				var mJson = JSON.parse(xhr.responseText);
				manifest.name = mJson.name
				manifest.description = mJson.description
				manifest.architecture = mJson.architecture
				manifest.title = mJson.title
				manifest.version = mJson.version
				manifest.maintainer = mJson.maintainer
				manifest.framework = mJson.framework
			}
		};
		xhr.send();
	}	
}
