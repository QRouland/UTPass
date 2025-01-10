#ifndef GITPLUGIN_H
#define GITPLUGIN_H

#include <QQmlExtensionPlugin>

class GitPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID
                      "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) override;
};

#endif
