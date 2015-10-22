#include "bci_c_signal_processing.h"

C_SignalProcessing::C_SignalProcessing()
{
    debugLog = SMART_DEBUG_LOG::Instance();

    //Store the Processing Results in a Thread Safe Circular Buffer FIFO with a 5ms timeout
    pProcessingResults = new resultsBufferType(RESULTS_BUFFER_SIZE,true, 5);

    //Execute Processing Tasks in a Seperate Thread
    moveToThread(&mThread);
    mThread.start();

    //Initialize Members
    resetData();
}

C_SignalProcessing::~C_SignalProcessing()
{
}

void C_SignalProcessing::resetData()
{
    //Reset Member to their Default Values
    mCurrentProcessingResult.command    = PCC_CMD_NONE;
    mCurrentProcessingResult.confidence = UNSURE;
    memset(&voltageSums     [0], 0, sizeof(uint64_t) * MAX_EEG_ELECTRODES);
    memset(&qualitySums     [0], 0, sizeof(uint64_t) * MAX_EEG_ELECTRODES);
    memset(&averageCQ       [0], 0, sizeof(uint16_t) * MAX_EEG_ELECTRODES);
    memset(&normalizedData  [0], 0, sizeof(uint32_t) * MAX_EEG_ELECTRODES);

    memset(&finalScores     [0], 0, sizeof(ProcessingScore_t) * NUM_PCC_DIRECTIONS);
    memset(&finalConfidences[0], 0, sizeof(Confidence_Type)   * NUM_PCC_DIRECTIONS);

    resultReady        = false;
    scoresCalculated   = false;
    averagesCalculated = false;
    framesProcessed    = 0;
}

void C_SignalProcessing::calculateAverages()
{
    //Divide By the EEG Data Size to Get the Average
    for (int k = 0; k < MAX_EEG_ELECTRODES; k++)
    {
        if (framesProcessed > 0)
        {
            normalizedData[k] =  voltageSums[k] / framesProcessed;
            averageCQ[k]      =  qualitySums[k] / framesProcessed;
        }
    }

    averagesCalculated = true;
}

void C_SignalProcessing::calculateFinalScores()
{
    finalScores[FORWARD]  = (normalizedData[P7]  + normalizedData[P8] ) / 2;
    finalScores[BACKWARD] = (normalizedData[F7]  + normalizedData[F8] ) / 2;
    finalScores[RIGHT]    = (normalizedData[FC5] + normalizedData[FC6]) / 2;
    finalScores[LEFT]     = (normalizedData[F3]  + normalizedData[F4] ) / 2;

    finalConfidences[FORWARD]  = static_cast<Confidence_Type>(finalScores[FORWARD]  % NUM_CONFIDENCE_TYPES);
    finalConfidences[BACKWARD] = static_cast<Confidence_Type>(finalScores[BACKWARD] % NUM_CONFIDENCE_TYPES);
    finalConfidences[RIGHT]    = static_cast<Confidence_Type>(finalScores[RIGHT]    % NUM_CONFIDENCE_TYPES);
    finalConfidences[LEFT]     = static_cast<Confidence_Type>(finalScores[LEFT]     % NUM_CONFIDENCE_TYPES);

    scoresCalculated = true;
}

//Calculate the Final Processing result
void C_SignalProcessing::calculateResult()
{
    int max = -1;
    PCC_Command_Type PCC_Commands[NUM_PCC_DIRECTIONS] =
    {
        PCC_FORWARD,
        PCC_BACKWARD,
        PCC_RIGHT,
        PCC_LEFT
    };


    if (!averagesCalculated)
    {
        calculateAverages();
    }

    if (!scoresCalculated)
    {
        calculateFinalScores();
    }

    //Find out which command scored the highest
    for (int i = 0; i < NUM_PCC_DIRECTIONS; i++)
    {
        if (finalScores[i] > max)
        {
            max = finalScores[i];
            mCurrentProcessingResult.command    = PCC_Commands[i];
            mCurrentProcessingResult.confidence = finalConfidences[i];
        }
    }

    //TODO: Actually Do something here
    mCurrentProcessingResult.confidence = static_cast<Confidence_Type>(max % NUM_CONFIDENCE_TYPES);

    resultReady = true;
}

void C_SignalProcessing::processFrame(EEG_Frame_t* pFrame)
{
    //TODO: PROCESS FRAME HERE
    if (pFrame)
    {
        for (int j = 0; j < MAX_EEG_ELECTRODES; j++)
        {
            voltageSums[j] += pFrame->electrodeData[j];
            qualitySums[j] += pFrame->contactQuality[j];
        }

        //Save Memory here by clearing the frames
        delete pFrame;
        framesProcessed++;
    }

    if (framesProcessed >= MIN_FRAMES_NEEDED)
    {
        performCalculations();
        framesProcessed = 0;
    }
}

void C_SignalProcessing::performCalculations()
{ 
    //Calculate Average Contact Quality and Normalize Voltages
    calculateAverages();

    //Calculate the Final Scores for each Potential Command
    calculateFinalScores();\

    //Calculate the Final Result
    calculateResult();

    //If the Data was good, we would have set the "resultReady" Flag
    if (resultReady && pProcessingResults->spacesAvailable() > 0)
    {
        //Add the Result to the Buffer and Signal that it can now be retrieved
        pProcessingResults->Put(mCurrentProcessingResult);
        emit eegDataProcessed(pProcessingResults);
    }

    //Reset Processing Data
    resetData();
}
