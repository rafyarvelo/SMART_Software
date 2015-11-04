#ifndef C_PORTNAMES_H
#define C_PORTNAMES_H

#include <QStringList>
#include <QFile>
#include <QTextStream>
#include "bci_c_singleton.h"
#include "qextserialenumerator.h"
#include "../smart_config.h"

class C_PortNames : public C_Singleton<C_PortNames>
{
public:
    C_PortNames();

    inline QString GetPCCPortName() { return pccPortName; }
    inline QString GetBRSPortName() { return brsPortName; }

    void readFromFile(QString filename);
private:
    void findPortNames_UNIX();
    void findPortNames_WIN();

    //Read names from file and enumerate them in case the file is missing
    QList<QextPortInfo> portInfo;
    QString pccPortName;
    QString brsPortName;
};

#endif // C_PORTNAMES_H
