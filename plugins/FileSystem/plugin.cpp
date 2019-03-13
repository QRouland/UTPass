#include <QtQml>
#include <QtQml/QQmlContext>

#include "plugin.h"
#include "filesystem.h"

void FileSystemPlugin::registerTypes(const char *uri) {
    //@uri Pass
    qmlRegisterSingletonType<FileSystem>(uri, 1, 0, "FileSystem", [](QQmlEngine*, QJSEngine*) -> QObject* { return new FileSystem; });
}
