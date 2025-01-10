#include <QtQml>

#include "plugin.h"
#include "utils.h"

void TestsUtilsPlugin::registerTypes(const char *uri)
{
    //@uri TestUtils
    qmlRegisterSingletonType<TestsUtilsPlugin>(uri, 1, 0, "TestUtils", [](QQmlEngine *, QJSEngine *) -> QObject * { return new TestsUtils; });
}
