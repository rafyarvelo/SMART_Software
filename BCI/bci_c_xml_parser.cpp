#include "bci_c_xml_parser.h"

C_XML_Parser::C_XML_Parser(const QString& filename, ReadOrWrite direction)
    :C_AbstractParser(filename, direction)
{
    if (direction == READ)
    {
        xmlIn.setDevice(this->fp);
    }
    else
    {
        xmlOut.setDevice(this->fp);
    }
}

C_XML_Parser::~C_XML_Parser()
{
}

//Read EEG Data
C_EEG_Data& C_XML_Parser::readEEGData ()
{
    //Clear Existing Data
    eegData.clear();

    //cout << "Reading EEG Data from File: " << filename.toStdString() << endl;

    //Read EEG Data
    while (!xmlIn.atEnd())
    {
        //Beginning of Data
        if (xmlIn.isStartElement())
        {
            if (xmlIn.name() == FILE_FORMAT::EEG_FRAME)
            {
                xmlIn.readNext();
                eegData.AddFrame(readEEGFrame());
                xmlIn.readNext();
            }
            else
            {
                xmlIn.readNext();
            }
        }
        else
        {
            xmlIn.readNext();
        }
    }

    return eegData;
}

//Write EEG Data
void C_XML_Parser::writeEEGData()
{
    //cout << "Writing EEG Data to File: " << filename.toStdString() << endl;
    StartDocument();

    //Write the Beginning Tag
    StartElement(FILE_FORMAT::EEG_DATA);

    //Write Each EEG Frame
    for (int i = 0; i < eegData.size(); i++)
    {
        writeEEGFrame(eegData.GetFramePtr(i));
    }

    //Write the Ending Tag
    EndElement();
    EndDocument();
}

//Read BRS Data
C_TM& C_XML_Parser::readTMData ()
{
    //cout << "Reading BRS Data from File: " << filename.toStdString() << endl;

    //Read BRS Data
    while (!xmlIn.atEnd())
    {
        //Beginning of Data
        if (xmlIn.isStartElement())
        {
            if (xmlIn.name() == FILE_FORMAT::TM_FRAME)
            {
                xmlIn.readNext();
                tmData.addFrame(readTMFrame());
                xmlIn.readNext();
            }
            else
            {
                xmlIn.readNext();
            }
        }
        else
        {
            xmlIn.readNext();
        }
    }

    return tmData;
}

//Write BRS Data
void C_XML_Parser::writeTMData()
{
    //cout << "Writing BRS Data to File: " << filename.toStdString() << endl;
    StartDocument();

    //Write the Beginning Tag
    StartElement(FILE_FORMAT::TM_DATA);

    //Write Each BRS Frame
    for (int i = 0; i < tmData.size(); i++)
    {
        writeTMFrame(tmData.GetFrame(i));
    }

    //Write the Ending Tag
    EndElement();
    EndDocument();
}

EEG_Frame_t* C_XML_Parser::readEEGFrame()
{
    EEG_Frame_t* buffer = EEG_Frame_t::create();
    int i;

    //EEG Type
    buffer->eegType = (eegTypeEnum)(ReadValue().toUInt());

    //EEG Counter
    buffer->counter = ReadValue().toUInt();

    //Electrode Data
    for (i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        buffer->electrodeData[i] = ReadValue().toInt();
    }

    //Contact Quality
    for (i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        buffer->contactQuality[i] = ReadValue().toShort();
    }

    //Gyros
    buffer->gyroX = (qint8) (ReadValue().toInt());
    buffer->gyroY = (qint8) (ReadValue().toInt());

    //Battery Percentage
    buffer->batteryPercentage = (quint8) (ReadValue().toUInt());

    //Pass the Last Element
    xmlIn.readNext();

    return buffer;
}

//Read a Value from the XML File
QString C_XML_Parser::ReadValue()
{
    QString value;

    //Get to the Beginning of the element
    while (!xmlIn.isStartElement())
        xmlIn.readNext();

    //Read Text and move to next
    value = xmlIn.readElementText();
    xmlIn.readNext();

    return value;
}

TM_Frame_t*  C_XML_Parser::readTMFrame()
{
    TM_Frame_t* buffer = TM_Frame_t::createFrame();

    return buffer;
}

void C_XML_Parser::writeEEGFrame(EEG_Frame_t* frame)
{
    int i, tabStart = 1;

    StartElement(FILE_FORMAT::EEG_FRAME, tabStart);

    //EEG Type
    WriteEntireElement(FILE_FORMAT::EEG_TYPE,FILE_FORMAT::EEG_TYPES[frame->eegType],tabStart + 1);

    //EEG Counter
    WriteEntireElement(FILE_FORMAT::EEG_COUNTER,QString::number(frame->counter),tabStart + 1);

    //Electrode Data
    for (i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        WriteEntireElement(FILE_FORMAT::EEG_DATA,QString::number(frame->electrodeData[i]),tabStart + 1);
    }

    //Contact Quality
    for (i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        WriteEntireElement(FILE_FORMAT::EEG_CONTACT,QString::number(frame->contactQuality[i]),tabStart + 1);
    }

    //Gyros
    WriteEntireElement(FILE_FORMAT::EEG_GYROX,QString::number(frame->gyroX),tabStart + 1);
    WriteEntireElement(FILE_FORMAT::EEG_GYROY,QString::number(frame->gyroY),tabStart + 1);

    //Battery Percentage
    WriteEntireElement(FILE_FORMAT::EEG_BATTERY,QString::number(frame->batteryPercentage),tabStart + 1);

    EndElement(tabStart);//EEG Frame
}

void C_XML_Parser::writeTMFrame(TM_Frame_t* frame)
{
    int i, tabStart = 1;

    StartElement(FILE_FORMAT::TM_FRAME, tabStart);

    //Time Stamp
    WriteEntireElement(FILE_FORMAT::TIME, QString::number(frame->timeStamp), tabStart + 1);

    //GPS Sensor Data
    writeGPSData(frame, tabStart + 1);

    //US Sensor Data
    StartElement(FILE_FORMAT::SENSOR_DATA, tabStart + 1);
    WriteEntireElement(FILE_FORMAT::NAME, QString("UltraSonic"), tabStart + 2);
    WriteEntireElement(QString("RangeToObject"), QString::number(frame->brsFrame.usData.rangeToObject), tabStart + 2);
    EndElement(tabStart + 1);//US Data

    //PCC Command
    WriteEntireElement(FILE_FORMAT::PCC_COMMAND,
                       QString(frame->brsFrame.remoteCommand), tabStart + 2);

    //LED Groups
    for (i = 0; i < NUM_LED_GROUPS; i++)
    {
        StartElement(FILE_FORMAT::LED_GROUD, tabStart + 1);
        WriteEntireElement(FILE_FORMAT::NAME, FILE_FORMAT::LED_GROUPS[frame->ledGroups[i]->id], tabStart + 2);
        WriteEntireElement(FILE_FORMAT::VALUE, QString::number(frame->ledGroups[i]->frequency), tabStart + 2);
        EndElement(tabStart + 1);
    }

    //EEG Connection Status
    StartElement(FILE_FORMAT::CONN_STATUS, tabStart + 1);
    WriteEntireElement(FILE_FORMAT::NAME, QString("EEG"), tabStart + 2);
    WriteEntireElement(FILE_FORMAT::VALUE, FILE_FORMAT::CONN_STATUSES[frame->eegConnectionStatus], tabStart + 2);
    EndElement(tabStart + 1);//EEG Connection Status

    //BRS Connection Status
    StartElement(FILE_FORMAT::CONN_STATUS, tabStart + 1);
    WriteEntireElement(FILE_FORMAT::NAME, QString("BRS"), tabStart + 2);
    WriteEntireElement(FILE_FORMAT::VALUE, FILE_FORMAT::CONN_STATUSES[frame->brsConnectionStatus], tabStart + 2);
    EndElement(tabStart + 1);//BRS Connection Status

    //PCC Connection Status
    StartElement(FILE_FORMAT::CONN_STATUS, tabStart + 1);
    WriteEntireElement(FILE_FORMAT::NAME, QString("PCC"), tabStart + 2);
    WriteEntireElement(FILE_FORMAT::VALUE, FILE_FORMAT::CONN_STATUSES[frame->pccConnectionStatus], tabStart + 2);
    EndElement(tabStart + 1);//PCC Connection Status

    //Flasher Connection Status
    StartElement(FILE_FORMAT::CONN_STATUS, tabStart + 1);
    WriteEntireElement(FILE_FORMAT::NAME, QString("Flasher"), tabStart + 2);
    WriteEntireElement(FILE_FORMAT::VALUE, FILE_FORMAT::CONN_STATUSES[frame->flasherConnectionStatus], tabStart + 2);
    EndElement(tabStart + 1);//Flasher Connection Status

    EndElement(tabStart);//BRS Frame
}

void C_XML_Parser::writeGPSData(TM_Frame_t* frame, int tabs)
{
    StartElement(FILE_FORMAT::SENSOR_DATA, tabs);
    WriteEntireElement(FILE_FORMAT::NAME, QString("GPS"), tabs + 1);
    WriteEntireElement(QString("Altitude"), QString::number(frame->brsFrame.gpsData.altitude), tabs + 1);
    WriteEntireElement(QString("Latitude"), QString::number(frame->brsFrame.gpsData.latitude), tabs + 1);
    WriteEntireElement(QString("Longitude"), QString::number(frame->brsFrame.gpsData.longitude), tabs + 1);
    WriteEntireElement(QString("GroundSpeed"), QString::number(frame->brsFrame.gpsData.groundSpeed), tabs + 1);
    EndElement(tabs);//GPS Data
}

//Output Carriage Return and Line Feed after element
void C_XML_Parser::StartElement(const QString& element, int TabAmount)
{
    //Insert Desired Tab Amount
    for (int i = 0; i < TabAmount; i++)
    {
        xmlOut.writeCharacters("\t");
    }

    //Write Start of Element
    xmlOut.writeStartElement(element);
    xmlOut.writeCharacters("\r\n");
}

void C_XML_Parser::WriteEntireElement(const QString& name, const QString& data, int tabs)
{
    //Insert Desired Tab Amount
    for (int i = 0; i < tabs; i++)
    {
        xmlOut.writeCharacters("\t");
    }

    //Write Entire Element
    xmlOut.writeStartElement(name);
    xmlOut.writeCharacters(data);
    EndElement();
}

//Output Carriage Return and Line Feed to Current File
void C_XML_Parser::EndElement(int tabAmount)
{
    //Insert Desired Tab Amount
    for (int i = 0; i < tabAmount; i++)
    {
        xmlOut.writeCharacters("\t");
    }

    //Write End Element
    xmlOut.writeEndElement();
    xmlOut.writeCharacters("\r\n");
}

void C_XML_Parser::StartDocument()
{
    xmlOut.writeStartDocument();
    xmlOut.writeCharacters("\r\n");
}

void C_XML_Parser::EndDocument()
{
    xmlOut.writeEndDocument();
    xmlOut.writeCharacters("\r\n");
}
