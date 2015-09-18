#include "bci_c_abstract_parser.h"

C_AbstractParser::C_AbstractParser()
    :
      tmDataIn(0),
      eegDataIn(0),
      tmDataOut(0),
      eegDataOut(0)
{
}

C_AbstractParser::~C_AbstractParser()
{
    if (tmDataIn)
    {
        delete tmDataIn;
    }
    if (tmDataOut)
    {
        delete tmDataOut;
    }
    if (eegDataIn)
    {
        delete eegDataIn;
    }
    if (eegDataOut)
    {
        delete eegDataOut;
    }
}

//Update Input Filename for EEG Data
void C_AbstractParser::SetEEGInputFilename(const QString& filename)
{
    SetFileName(eegDataIn, filename, READ_ONLY);
}

//Update Input Filename for BRS Data
void C_AbstractParser::SetTMInputFilename(const QString& filename)
{
    SetFileName(tmDataIn, filename, READ_ONLY);
}

//Update Output Filename for EEG Data
void C_AbstractParser::SetEEGOutputFilename(const QString &filename)
{
    SetFileName(eegDataOut, filename, WRITE_ONLY);
}

//Update Output Filename for BRS Data
void C_AbstractParser::SetTMOutputFilename(const QString& filename)
{
    SetFileName(tmDataOut, filename, WRITE_ONLY);
}

void C_AbstractParser::SetFileName(QFile*& device, const QString& filename, bool ReadOnly)
{
    if (device)
    {
        if (device->isOpen())
        {
            device->close();
        }
        delete device;
        device = 0;
    }

    //Create New QFile Object
    device = new QFile(filename);

    //Open File
    if (ReadOnly)
    {
        device->open(QIODevice::ReadOnly);
    }
    else
    {
        device->open(QIODevice::WriteOnly);
    }
}

void C_AbstractParser::writeEEGData(C_EEG_Data& data, const QString& filename)
{
    this->eegData = data;
    writeEEGData(filename);
}


void C_AbstractParser::writeTMData(C_TM& data, const QString& filename)
{
    this->tmData = data;
    writeTMData(filename);
}
