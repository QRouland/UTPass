#include <QtQml>

#include "plugin.h"
#include "utils.h"

void UtilsPlugin::registerTypes(const char *uri)
{
    //@uri Utils
    qmlRegisterSingletonType<Utils>(uri, 1, 0, "Utils", [](QQmlEngine *, QJSEngine *) -> QObject * { return new Utils; });
}
