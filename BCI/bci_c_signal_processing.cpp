#include <zmq.hpp>

// TODO: Figure out what headers I need here...

#include "bci_c_signal_processing.h"
#include "seniordesign.pb.h"

void doRun(C_SignalProcessing *sp);

void doRun(C_SignalProcessing *sp) {
    sp->recvLoop();
}

C_SignalProcessing::C_SignalProcessing()
{
    debugLog = SMART_DEBUG_LOG::Instance();

    //Execute Processing Tasks in a Seperate Thread
    moveToThread(&mThread);
    mThread.start();

    mIsAlive.store(true);

    mRecvThread = std::thread(doRun, this);

    //Initialize Members
    resetData();
}

C_SignalProcessing::~C_SignalProcessing()
{
    mIsAlive.store(false);
    mRecvThread.join();
}

void C_SignalProcessing::recvLoop()
{
    auto c = zmq::context_t(1);
    auto s = zmq::socket_t(c, ZMQ_SUB);
    zmq::pollitem_t polls[] = {
        { s, 0, ZMQ_POLLIN, 0 }
    };
    s.connect("tcp://localhost:9000");
    while (mIsAlive.load())
    {
        auto nMsgs = zmq::poll(polls, 1, 1000);
        if (nMsgs > 0)
        {
            for (size_t i = 0; i < nMsgs; ++i)
            {
                zmg::message_t r;
                s.recv(&msg);
                seniordesign::ProcessingResults r;
                r.ParseFromString(std::string(
                            static_cast<char*>(msg.data(), msg.size())));
                if (mResultsBuf.spacesAvailable())
                {
                    ProcessingResult_t pr;
                    switch (r.direction())
                    {
                        case seniordesign::NEUTRAL:
                            pr.command = PCC_STOP;
                            break;
                        case seniordesign::FORWARD:
                            pr.command = PCC_FORWARD;
                            break;
                        case seniordesign::BACKWARD:
                            pr.command = PCC_BACKWARD;
                            break;
                        case seniordesign::LEFT:
                            pr.command = PCC_LEFT;
                            break;
                        case seniordesign::RIGHT:
                            pr.command = PCC_RIGHT;
                            break;
                        default:
                            pr.command = PCC_STOP;
                    }
                    double confidence = pr.confidence();
                    if (confidence > 0.5)
                    {
                        pr.confidence = LIKELY;
                    }
                    else
                    {
                        pr.confidence = UNSURE;
                    }
                    mResultsBuf.Put(pr);
                    emit eggDataProcessed(mResultsBuf);
                }
            }
        }
        else
        {
            // TODO: log timeout
        }
    }
}
