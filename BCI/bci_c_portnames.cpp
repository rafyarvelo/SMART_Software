#include "bci_c_portnames.h"

//Find the PCC and BRS Port Names
C_PortNames::C_PortNames()
{
    //Get Port Info
    portInfo = QextSerialEnumerator::getPorts();
    pccPortName = "NOT_FOUND";
    brsPortName = "NOT_FOUND";

    #ifdef WIN32
    findPortNames_WIN();
    #else
    findPortNames_UNIX();
    #endif

    //Try to Read from the Default file
    readFromFile(PORTNAMES_FILE);
}

void C_PortNames::findPortNames_WIN()
{
    QextPortInfo info;

    //Guess the port names
    for (int i =0; i < portInfo.size(); i++)
    {
        info = portInfo.at(i);
        if (info.friendName.contains("Stellaris"))
        {
            brsPortName = info.portName;
        }

        if (info.friendName.contains("MSP430"))
        {
            pccPortName = info.portName;
        }

        if (info.friendName.contains("USB"))
        {
            if (brsPortName == "NOT_FOUND" && pccPortName == "NOT_FOUND")
            {
                brsPortName = info.portName;
            }
            else
            {
                pccPortName = info.portName;
            }
        }
    }
}

void C_PortNames::findPortNames_UNIX()
{
    QStringList ttyACMNames;
    QStringList ttyUSBNames;
    QString portName;

    for (int i = 0; i < portInfo.size(); i++)
    {
        portName = portInfo.at(i).portName;
        if (portName.contains("ACM"))
        {
            ttyACMNames.append(portName);
        }
        else if (portName.contains("USB"))
        {
            ttyACMNames.append(portName);
        }
    }

    //Guess the Port Name
    if (ttyACMNames.size() >= 2)
    {
        brsPortName = ttyACMNames[0];
        pccPortName = ttyACMNames[1];
    }
    else if (ttyACMNames.size() == 1)
    {
        brsPortName = ttyACMNames[0];
        pccPortName = ttyUSBNames.first();
    }
    else
    {
        brsPortName = ttyUSBNames.first();
        pccPortName = ttyUSBNames.last();
    }
}

void C_PortNames::readFromFile(QString filename)
{
    QFile fp(filename);
    QString tmp;

    if (fp.exists())
    {
        QTextStream stream(&fp);

        fp.open(QIODevice::ReadOnly);
        stream.readLine(); //Read First Line

         //Read Second Line on Windows
        #ifdef WIN32
        stream.readLine();
        #endif

        //FORMAT = BRS NAME PCC_NAME
        tmp = stream.readLine();
        brsPortName = tmp.split(" ").first();
        pccPortName = tmp.split(" ").last();
    }
}
