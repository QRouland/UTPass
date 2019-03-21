#ifndef PASSPLUGIN_H
#define PASSPLUGIN_H

#include <QQmlExtensionPlugin>

class PassPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID
                      "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) override;
};

#endif
