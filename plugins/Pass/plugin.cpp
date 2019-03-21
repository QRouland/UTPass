#include <QtQml>

#include "plugin.h"
#include "pass.h"

void PassPlugin::registerTypes(const char *uri)
{
    //@uri Pass
    qmlRegisterSingletonType<Pass>(uri, 1, 0, "Pass", [](QQmlEngine *, QJSEngine *) -> QObject * { return new Pass; });
}
