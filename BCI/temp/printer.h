#ifndef PRINTER_H
#define PRINTER_H

#include <QObject>

class Printer : public QObject
{
    Q_OBJECT
public:
    explicit Printer(QObject *parent = 0);

signals:

public slots:

};

#endif // PRINTER_H
