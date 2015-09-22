#ifndef BCI_C_SIGNAL_PROCESSING_H
#define BCI_C_SIGNAL_PROCESSING_H


#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_eeg_io.h"

class C_SignalProcessing : public QObject
{
    Q_OBJECT
public:
	 C_SignalProcessing(C_EEG_IO* eegIO);
	~C_SignalProcessing();
	
	static  C_SignalProcessing* Instance(C_EEG_IO* eegIO)
	{
		return new C_SignalProcessing(eegIO);
	}

    C_EEG_Data& GetProcessedData() { return processedData; }

signals:
    void eegDataProcessed(C_EEG_Data& processedData);

public slots:
    void processEEGData(C_EEG_Data& rawEEGData);
	
private:
    C_EEG_IO*  mEEG_IO_Ptr;
    C_EEG_Data processedData;
};

#endif // BCI_C_SIGNAL_PROCESSING_H

