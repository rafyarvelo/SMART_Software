#include "bci_c_textparser.h"

C_TextParser::C_TextParser()
{

    //Initialize Default Input/Output Files
    SetTMInputFilename(TM_DATA_INPUTFILE_TXT);
    SetTMOutputFilename(TM_DATA_OUTPUTFILE_TXT);
    SetEEGInputFilename(EEG_DATA_INPUTFILE_TXT);
    SetEEGOutputFilename(EEG_DATA_OUTPUTFILE_TXT);
}

C_TextParser::~C_TextParser()
{
}

//Read EEG Data
C_EEG_Data& C_TextParser::readEEGData (const QString& filename)
{
    //Set EEG Filename
    SetEEGInputFilename(filename);

}

//Write EEG Data
void C_TextParser::writeEEGData(const QString& filename)
{
    //Set EEG Filename
    SetEEGOutputFilename(filename);

}

//Read BRS Data
C_TM& C_TextParser::readTMData (const QString& filename)
{
    //Set TM Filename
    SetTMInputFilename(filename);

    return tmData;
}

//Write BRS Data
void C_TextParser::writeTMData(const QString& filename)
{
    //Set TM Filename
    SetTMOutputFilename(filename);

}

void C_TextParser::writeEEGData(C_EEG_Data& data, const QString& filename)
{
    C_AbstractParser::writeEEGData(data, filename);
}

void C_TextParser::writeTMData(C_TM& data, const QString& filename)
{
    C_AbstractParser::writeTMData(data, filename);
}

EEG_Frame_t* C_TextParser::readEEGFrame()
{
    EEG_Frame_t* buffer = EEG_Frame_t::create();

    return buffer;
}

TM_Frame_t*  C_TextParser::readTMFrame()
{
    TM_Frame_t* buffer = TM_Frame_t::createFrame();

    return buffer;
}

void C_TextParser::writeEEGFrame(EEG_Frame_t* frame)
{

}

void C_TextParser::writeTMFrame(TM_Frame_t* frame)
{

}
