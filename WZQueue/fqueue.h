//
// Created by w2w on 18-3-29.
//

#ifndef STRATEGY_FQUEUE_H
#define STRATEGY_FQUEUE_H

#include <atomic>
#include <sched.h>
#include <assert.h>
using std::atomic_compare_exchange_weak;

template <typename ELEM_T>
class FQueue{
private:
    unsigned int m_readIndex;
    unsigned int m_writeIndex;
    unsigned int m_maxReadIndex;
    ELEM_T *m_theQueue;
    inline unsigned int countToIndex(unsigned int a_count);
    unsigned int Q_SIZE;
    unsigned int count;

public:
    FQueue(unsigned int size = 1024);
    virtual ~FQueue();

    bool push(const ELEM_T &a_data);
    bool pop(ELEM_T &a_data);
    unsigned int size();
};

template <typename ELEM_T>
inline unsigned int FQueue::countToIndex(unsigned int a_count) {
    return (a_count%Q_SIZE);
}

template <typename ELEM_T>
FQueue::FQueue(unsigned int size) {
    m_theQueue = new ELEM_T[size];
    memset(m_theQueue, 0, size*sizeof(ELEM_T));
    this->m_readIndex = 0;
    this->m_writeIndex = 0;
    this->m_maxReadIndex = 0;
    this->Q_SIZE = size;
    this->count = 0;
}

template <typename ELEM_T>
FQueue::~FQueue() {
    delete[] m_theQueue;
}

template <typename ELEM_T>
bool FQueue::push(const ELEM_T &a_data) {
    unsigned int currentReadIndex, currentWriteIndex;
    do{
        currentReadIndex = m_readIndex;
        currentWriteIndex = m_writeIndex;
        if  (countToIndex(currentWriteIndex+1) == countToIndex(currentReadIndex)){
            // the queue is full
            return false;
        }
    }while(!atomic_compare_exchange_weak(&m_writeIndex, &currentWriteIndex, (currentWriteIndex+1)));

    //write
    m_theQueue[countToIndex(currentWriteIndex)] = a_data;

    // Consider that there is more than 1 producer thread
    // commit
    while(!atomic_compare_exchange_weak(&m_maxReadIndex, currentWriteIndex, (currentWriteIndex + 1))){
//        sched_yield();
        // 死等
    }
    ++count;
    return true;
}

template <typename ELEM_T>
bool FQueue::pop(ELEM_T &a_data) {
    unsigned int currentReadIndex, currentWriteIndex;
    do{
        currentReadIndex = m_readIndex;
        currentMaxReadIndex = m_maxReadIndex;
        if (countToIndex(currentReadIndex) == countToIndex(currentMaxReadIndex)){
            // the queue is empty
            return false;
        }
        a_data = m_theQueue[countToIndex(currentReadIndex)];
        if  (atomic_compare_exchange_weak(&m_readIndex, &currentReadIndex, (currentReadIndex + 1))){
            --count;
            return true;
        }
    }while(true);
    // Something went wrong. it shouldn't be possible to reach here
    assert(0);
    return false;
}

template <typename ELEM_T>
unsigned int FQueue::size() {
    return count;
}


#endif //STRATEGY_FQUEUE_H
