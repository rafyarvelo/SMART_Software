#ifndef C_SAFEQUEUE_H
#define C_SAFEQUEUE_H

#include <QObject>
#include <QSemaphore>
#include <QVector>

#define DEFAULT_BUFFER_SIZE 50
#define SEMAPHORE_TIMEOUT   100 //ms

//Implement our Queue using a Simple Circular Buffer
template <typename T>
class C_CircularBuffer : public QVector<T>
{
private:
    //Where to insert and Retrieve from the buffer
    unsigned int insertionIndex;
    unsigned int retrievalIndex;

public:
     C_CircularBuffer(int bufferSize = DEFAULT_BUFFER_SIZE)
        : QVector<T>(bufferSize)
     {
         insertionIndex = 0;
         retrievalIndex = 0;
     }
    ~C_CircularBuffer(){}

    //FIFO Retrieval
    inline const T& Get()
    {
        //Implement Circular Retrieval
        if (retrievalIndex >= this->QVector<T>::size())
        {
            retrievalIndex = 0;
        }

        //Return Desired item and increment index
        return this->QVector<T>::at(retrievalIndex++);
    }

    //Circular insertion
    inline void Put(const T& item)
    {
        if (insertionIndex >= this->QVector<T>::size())
        {
            insertionIndex = 0;
        }

        this->QVector<T>::replace(insertionIndex++, item);
    }

    //Easy printing
    void print(std::ostream& stream)
    {
        stream << "Array Contents: ";
        for (int i = 0; i < this->QVector<T>::size(); i++)
        {
            stream << this->QVector<T>::at(i) << " ";
        }
        stream << std::endl;
    }
};


//This class provides a thread safe implementation of a
//circular buffer using QSemaphores
template <typename T>
class C_SafeQueue
{
private:
    //Initially Buffer Size, this holds how many items we can insert to the buffer
    QSemaphore* pItemsToProduce;

    //Initially 0, this hold how many items we have already produced
    QSemaphore* pItemsToConsume;

    //Optional Timeouts
    unsigned int mTimeout;

    //How we'll Contain our Items
    C_CircularBuffer<T>* buffer;

public:
    C_SafeQueue(unsigned int bufferSize    = DEFAULT_BUFFER_SIZE,
                bool         enableTimeout = true,
                unsigned int timeout       = SEMAPHORE_TIMEOUT)
    {
        //Initialize Members
        pItemsToProduce = new QSemaphore(bufferSize);
        pItemsToConsume = new QSemaphore(0);
        buffer          = new C_CircularBuffer<T>(bufferSize);
        mTimeout        = (enableTimeout) ? timeout : -1;
    }
    ~C_SafeQueue(){}

    //Item Retrieval, We don't need to know nor care about the size here,
    //if we try to retrieve before there's anything available, we will
    //be forced to wait on the ItemsToConsume Semaphore
    const T& Get(bool* status = 0)
    {
        bool ok = false;

        //Check on our Consumption Semaphore to see if we have items available
        ok = pItemsToConsume->tryAcquire(1, mTimeout);
        if (ok)
        {
            //Notify that we can now produce one more item
            pItemsToProduce->release();

            //Update Status
            if (status) *status = ok;

            //Return a reference to the requested item
            return buffer->Get();
        }

        else //No items available, return first item by default
        {
            if (status) *status = false;
            return buffer->at(0);
        }
    }

    //Item Insertion, We can insert freely until the ItemsToProduce Semaphore says
    //that we need to slow down and wait
    void Put(T& item, bool* status = 0)
    {
        bool ok = false;

        //Check if we have space to produce more items
        ok = pItemsToProduce->tryAcquire(1, mTimeout);
        if (ok)
        {
            //Put the Item in our Buffer
            buffer->Put(item);

            //Notify that we have produced an item
            pItemsToConsume->release();
        }

        //Update Status
        if (status) *status = ok;
    }

    //Access the size of our Circular buffer
    unsigned int itemsAvailable () { return pItemsToConsume->available();}
    unsigned int spacesAvailable() { return pItemsToProduce->available();}

    //Easy Printing
    void print(std::ostream stream) { buffer->print(stream); }
};

#endif // C_SAFEQUEUE_H
