#include "bci_c_abstract_parser.h"

C_AbstractParser::C_AbstractParser(const QString& filename, QIODevice::OpenModeFlag openMode)
    : fp(0)
{
    fp = new QFile(filename);
    fp->open(openMode);

    m_filename = filename;
}

C_AbstractParser::~C_AbstractParser()
{
    if (fp)
    {
        delete fp;
    }
}

void C_AbstractParser::writeEEGData(C_EEG_Data& data)
{
    this->eegData = data;
    writeEEGData();
}


void C_AbstractParser::writeTMData(C_TM& data)
{
    this->tmData = data;
    writeTMData();
}
