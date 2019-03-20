#include <QtQml>

#include "plugin.h"
#include "gpg.h"

void FileSystemPlugin::registerTypes(const char *uri) {
  //@uri Pass
  qmlRegisterSingletonType<Gpg>(uri, 1, 0, "Gpg", [](QQmlEngine *, QJSEngine *) -> QObject * { return new Gpg; });
}
