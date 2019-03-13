#ifndef PASS_H
#define PASS_H

#include <QObject>

class FileSystem: public QObject {
    Q_OBJECT

public:
    FileSystem();
    ~FileSystem() = default;

    Q_INVOKABLE void list_dir();
};

#endif
