#include <QtQml>

#include "plugin.h"
#include "git.h"

void GitPlugin::registerTypes(const char *uri)
{
    //@uri Utils
    qmlRegisterSingletonType<Git>(uri, 1, 0, "Git", [](QQmlEngine *, QJSEngine *) -> QObject * { return new Git; });
}
