#ifndef TESTSUTILSPLUGIN_H
#define TESTSUTILSPLUGIN_H

#include <QQmlExtensionPlugin>

class TestsUtilsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID
                      "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) override;
};

#endif
