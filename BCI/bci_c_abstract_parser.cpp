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
