#ifndef PASS_H
#define PASS_H

#include <QObject>

class Pass: public QObject {
    Q_OBJECT

public:
    Pass();
    ~Pass() override = default;

    Q_INVOKABLE void cmd_show(QString path) ;
    Q_INVOKABLE void cmd_version() ;
};

#endif
