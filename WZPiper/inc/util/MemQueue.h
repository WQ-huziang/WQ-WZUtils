#ifndef MEMQUEUE_H_
#define MEMQUEUE_H_

#include <atomic>
#include <sched.h>
#include <assert.h>
using std::atomic_compare_exchange_weak;


// #define MAX_READER_SIZE 20

template <typename ELEM_T, typename MAX_READER_SIZE>
class MemQueue{
private:

    int m_readIndex_arr[MAX_READER_SIZE];
    int reader_num=-1;
    volatile atomic<int> m_writeIndex;
    volatile atomic<int> m_maxReadIndex;
    volatile atomic<int> m_minReadIndex;
    ELEM_T *m_theQueue;
    inline int countToIndex(unsigned int a_count);
    int Q_SIZE;
    int count;

public:
    MemQueue(unsigned int size = 1024);
    virtual ~MemQueue();

    // add a reader to the queue
    int add_reader();

    bool push(const ELEM_T &a_data);
    bool pop(ELEM_T &a_data,read_num);
    int size();
};


// map the current a_count index to the real queue index
template <typename ELEM_T, typename MAX_READER_SIZE>
inline unsigned int MemQueue::countToIndex(unsigned int &a_count) {
    return (a_count%Q_SIZE);
}

// constructure
template <typename ELEM_T, typename MAX_READER_SIZE>
MemQueue::MemQueue(int &size) {
    m_theQueue = new ELEM_T[size];
    memset(m_theQueue, 0, size*sizeof(ELEM_T));
    // set every readIndex as the max value
    memset(m_readIndex_arr, -1, sizeof(MAX_READER_SIZE));
    this->m_writeIndex = 0;
    this->m_maxReadIndex = 0;
    this->m_minReadIndex = 0;
    this->Q_SIZE = size;
    this->count = 0;
}

template <typename ELEM_T>
MemQueue::~MemQueue() {
    delete[] m_theQueue;
}

template <typename ELEM_T, typename MAX_READER_SIZE>
bool MemQueue::push(const ELEM_T &a_data) {

    int currentWriteIndex;

    do{
        currentWriteIndex = m_writeIndex;

        // if the producer catch the slowest consumer
        if  (countToIndex(currentWriteIndex+1) == countToIndex(m_minReadIndex)){
            // the queue is full
            // the producer catch the consumer
            return false;
        }
    // add, get the right to write
    }while(!atomic_compare_exchange_weak(&m_writeIndex, &currentWriteIndex, (currentWriteIndex+1)));

    // write a_data to queue
    memcpy(&m_theQueue[countToIndex(currentWriteIndex)], &a_data, sizeof(a_data));

    // Consider that there is more than 1 producer thread
    // commit, wait for the producer finish the push operation, reset the upper bound of the to currentWriteIndex + 1
    while(!atomic_compare_exchange_weak(&m_maxReadIndex, &currentWriteIndex, (currentWriteIndex + 1))){
	// sched_yield();
    // 死等
    }
    ++count;
    return true;
}

template <typename ELEM_T, typename MAX_READER_SIZE>
bool MemQueue::pop(ELEM_T &a_data, int reader_num) {
    int currentReadIndex;
    do{
        currentReadIndex = m_readIndex_arr[reader_num];
        currentMaxReadIndex = m_maxReadIndex;

        // if the consumer catch the producer
        if (countToIndex(currentReadIndex) == countToIndex(m_maxReadIndex)){
            // the queue is empty
            // the consumer catch the producer
            return false;
        }

        // pop a data from queue 
        memcpy(&a_data, &m_theQueue[countToIndex(currentReadIndex)], sizeof(a_data));

        // have atomic problem, because it's possible many reader is the lowest one
        while(!atomic_compare_exchange_weak(&m_minReadIndex, &currentReadIndex, (currentReadIndex + 1))){
        }

        // no atomic problem(every reader has a distinct reader number), asign it directly
        m_readIndex_arr[reader_num] = currentReadIndex + 1;

        // pop decrease the count
        --count;

		return true;

    }while(true);

    // Something went wrong. it shouldn't be possible to reach here
    assert(0);
    return false;
}

template <typename ELEM_T, typename MAX_READER_SIZE>
int MemQueue::add_reader(){

	// add reader, reader num
	reader_num++;
	m_readIndex_arr[reader_num] = m_minReadIndex;
	return reader_num;

}

template <typename ELEM_T>
int MemQueue::size() {
    return count;
}

#endif// MEMQUEUE_H_