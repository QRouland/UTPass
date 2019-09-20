#ifndef UTILSPLUGIN_H
#define UTILSPLUGIN_H

#include <QQmlExtensionPlugin>

class UtilsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID
                      "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) override;
};

#endif
