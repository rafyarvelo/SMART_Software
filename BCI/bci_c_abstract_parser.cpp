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

QString FILE_FORMAT::Int2String(const QString* array, unsigned size, unsigned value)
{
    QString convertedString;

    if (value > size)
    {
        convertedString = QString("Invalid Value::") + QString::number(value) ;
    }
    else
    {
        convertedString = array[value];
    }

    return convertedString;
}
