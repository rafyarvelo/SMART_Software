#ifndef BCI_C_SIGNAL_PROCESSING_H
#define BCI_C_SIGNAL_PROCESSING_H

#include <QThread>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_eeg_io.h"

#define MIN_CONTACT_QUALITY 2000
#define MAX_CONTACT_QUALITY 6000

#define MIN_FRAMES_NEEDED 10

//Final Score as an Unsigned integer
typedef unsigned short ProcessingScore_t;

//Since the Signal Processing will likely occur at a faster rate then we can
//send commands, we'll just stick the Processing Results in a Circular Buffer
//and then Send out a Signal with Reference to the Buffer whenever we add a result.
//Since we will used a Qt::QueuedConnection for our Signal along with FIFO access to
//our buffer, we will be all good to go here
typedef C_SafeQueue<ProcessingResult_t> resultsBufferType;
#define RESULTS_BUFFER_SIZE 30

class C_SignalProcessing : public QObject , public C_Singleton<C_SignalProcessing>
{
    Q_OBJECT
public:

    C_SignalProcessing();
   ~C_SignalProcessing();	

signals:
    void eegDataProcessed(resultsBufferType* pProcessingResults);

public slots:
    void processFrame(EEG_Frame_t* frame);
	
private:
    void resetData();

    //Top Level Calculation Call, called when MIN_FRAMES_NEEDED is Reached
    void performCalculations();

    //Average out the Summed Data
    void calculateAverages();

    //Calculate the Final Scores after Processing is complete
    void calculateFinalScores();

    //Calculate the Result after all the data has been processed
    void calculateResult();

private:
    SMART_DEBUG_LOG*   debugLog;
    QThread            mThread;
    C_EEG_Data         eegData; //Buffer Frames here

    //Store all Processing Results in a Circular Buffer FIFO
    resultsBufferType* pProcessingResults;

    //Only the latest Result
    ProcessingResult_t mCurrentProcessingResult;

    //Electrode Contact Values for Processing
    uint32_t normalizedData[MAX_EEG_ELECTRODES];

    //Average Contact Quality
    uint16_t averageCQ[MAX_EEG_ELECTRODES];

    //Keep Track of Sums to Calculate the Averages
    uint64_t voltageSums[MAX_EEG_ELECTRODES];
    uint64_t qualitySums[MAX_EEG_ELECTRODES];

    //Final Scores from Data Processing
    ProcessingScore_t finalScores[NUM_PCC_DIRECTIONS];
    Confidence_Type   finalConfidences[NUM_PCC_DIRECTIONS];

    //Count of how many frames we've processed
    uint64_t framesProcessed;

    bool averagesCalculated;
    bool scoresCalculated;
    bool resultReady;
};

#endif // BCI_C_SIGNAL_PROCESSING_H

