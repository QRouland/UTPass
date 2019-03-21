#ifndef PASS_H
#define PASS_H

#include <QObject>

class Pass : public QObject
{
    Q_OBJECT
    QString pass_store;

public:
    Pass();
    ~Pass() override = default;

    Q_INVOKABLE void speak();
};

#endif
