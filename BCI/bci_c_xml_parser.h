#ifndef BCI_C_XML_PARSER_H
#define BCI_C_XML_PARSER_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "bci_c_abstract_parser.h"

//Default Input/Output Filenames
const QString TM_DATA_INPUTFILE_XML  = "../debug_files/brs_input_data.xml";
const QString TM_DATA_OUTPUTFILE_XML = "../debug_files/brs_output_data.xml";

const QString EEG_DATA_INPUTFILE_XML  = "../debug_files/eeg_input_data.xml";
const QString EEG_DATA_OUTPUTFILE_XML = "../debug_files/eeg_output_data.xml";

//Using this class to parse XML Files for BRS/EEG Debugging
class C_XML_Parser : public C_AbstractParser
{
public:
     C_XML_Parser();
    ~C_XML_Parser();

    //Read/Write EEG Data
    virtual C_EEG_Data& readEEGData (const QString& filename = EEG_DATA_INPUTFILE_XML);
    virtual void writeEEGData(const QString& filename = EEG_DATA_OUTPUTFILE_XML);
    void writeEEGData(C_EEG_Data& data, const QString& filename = EEG_DATA_OUTPUTFILE_XML);

    //Read/Write BRS Data
    virtual C_TM& readTMData (const QString& filename = TM_DATA_INPUTFILE_XML);
    virtual void writeTMData(const QString& filename = TM_DATA_OUTPUTFILE_XML);
    void writeTMData(C_TM& data, const QString& filename = TM_DATA_OUTPUTFILE_XML);


    //Read Individual Frames
    virtual EEG_Frame_t* readEEGFrame();
    virtual TM_Frame_t*  readTMFrame();
    virtual void writeEEGFrame(EEG_Frame_t* frame);
    virtual void writeTMFrame(TM_Frame_t* frame);
    void writeGPSData(TM_Frame_t* frame, int tabs);

private:
    void StartElement(const QString& element, int tabAmount = 0);
    void EndElement(int tabAmount = 0);
    void StartDocument();
    void EndDocument();
    void WriteEntireElement(const QString& name, const QString& data, int tabs);

    //Read in an XML Value
    QString ReadValue();

    //XML Api
    QXmlStreamReader xmlIn;
    QXmlStreamWriter xmlOut;
};

#endif // BCI_C_XML_PARSER_H
