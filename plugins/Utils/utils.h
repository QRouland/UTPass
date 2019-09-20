#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QUrl>
#include <QQuickWindow>

class Utils : public QObject
{
    Q_OBJECT

public:
    Utils();
    ~Utils() override = default;

    Q_INVOKABLE bool unzip(QUrl zip_url, QString dir_out);
    Q_INVOKABLE bool rmFile(QUrl file_url);
    Q_INVOKABLE bool rmDir(QUrl dir_url);
};

#endif
