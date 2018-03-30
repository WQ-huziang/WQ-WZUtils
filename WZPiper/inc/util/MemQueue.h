#ifndef MEMQUEUE_H_
#define MEMQUEUE_H_

#include <atomic>
#include <sched.h>
#include <assert.h>
#include <string.h>
#include <iostream>

// using std::cout;
// using std::endl;
using std::atomic_compare_exchange_weak;


// #define MAX_READER_SIZE 20
// #define MAX_QUEUE_SIZE 4

template <typename ELEM_T>
class MemQueue{
private:
	// each reader's readIndex
    // int m_readIndex_arr[MAX_READER_SIZE];
    int *m_readIndex_arr;

    // the total number of reader start from 1
    std::atomic<int> reader_num;

    // the write index
    std::atomic<int> m_write_index;

    //the max can read index
    std::atomic<int> m_max_read_index;

    // the last waiting to read slot index
    std::atomic<int> m_min_read_index;

    // queue to contain the data
    // ELEM_T m_theQueue[MAX_QUEUE_SIZE];
    ELEM_T *m_theQueue;

    // the slowest slot read time
    std::atomic<int> slowest_read_time;

    // transform the count number to real index of theQueue
    int count_to_index(int a_count);

    int MAX_READER_SIZE;
    int Q_SIZE;
    int count;

public:
    MemQueue(int queue_size = 1024, int reader_size = 2);
    virtual ~MemQueue();

    // add a reader to the queue
    int add_reader();

    // write a data to the queue
    bool push(const ELEM_T &a_data);

    // read a data from the queue
    bool pop(ELEM_T &a_data,int read_num);

    int get_queue_size();

    int get_reader_size();

    int get_write_index();

    int get_max_read_index();

    int get_min_read_index();

    int get_slowest_read_time();

};


// map the current a_count index to the real queue index
template <typename ELEM_T>
int MemQueue<ELEM_T>::count_to_index(int a_count) {
    return (a_count%Q_SIZE);
}

// constructure
template <typename ELEM_T>
MemQueue <ELEM_T> ::MemQueue(int queue_size, int reader_size) {

	m_readIndex_arr = new int[reader_size];
	m_theQueue = new ELEM_T[queue_size];

    memset(m_theQueue, 0, sizeof(m_theQueue));
    // set every readIndex as the max value
    memset(m_readIndex_arr, -1, sizeof(m_readIndex_arr));

    this->slowest_read_time = 0;
    this->reader_num = 0;
    this->m_write_index = 0;
    this->m_max_read_index = 0;
    this->m_min_read_index = 0;
    this->MAX_READER_SIZE = reader_size;
    this->Q_SIZE = queue_size;
    this->count = 0;

}

template <typename ELEM_T>
MemQueue<ELEM_T>::~MemQueue() {
    delete[] m_theQueue;
}

template <typename ELEM_T>
bool MemQueue<ELEM_T>::push(const ELEM_T &a_data) {

    int cur_writeIndex;

    do{
        cur_writeIndex = m_write_index;

        // if the producer catch the slowest consumer
        if  (count_to_index(cur_writeIndex) == count_to_index(m_min_read_index) && count!= 0){
            // the queue is full
            // the producer catch the consumer
            return false;
        }

    // add, get the right to write
    }while(!atomic_compare_exchange_weak(&m_write_index, &cur_writeIndex, (cur_writeIndex+1)));

    // write a_data to queue
    memcpy(&m_theQueue[count_to_index(cur_writeIndex)], &a_data, sizeof(a_data));

    // cout<<"m_theQueue= "<<m_theQueue[count_to_index(cur_writeIndex)] << "; count_to_index(cur_writeIndex) = " << count_to_index(cur_writeIndex) <<endl;

    // Consider that there is more than 1 producer thread
    // commit, wait for the producer finish the push operation, reset the upper bound of the to cur_writeIndex + 1
    while(!atomic_compare_exchange_weak(&m_max_read_index, &cur_writeIndex, (cur_writeIndex + 1))){
	// sched_yield();
    // 死等
    }
    ++count;
    return true;
}

template <typename ELEM_T>
bool MemQueue<ELEM_T>::pop(ELEM_T &a_data, int reader_id) {
    int cur_readIndex, cur_max_readIndex, cur_read_time, nxt_read_time;

    // the reader is not exist
    if ( reader_id == -1 || reader_id >= reader_num) {
    	return false;
    }

    do{
        cur_readIndex = m_readIndex_arr[reader_id];
        cur_max_readIndex = m_max_read_index;

        // if the consumer catch the producer
        if (count_to_index(cur_readIndex) == count_to_index(cur_max_readIndex)){
            // the queue is empty
            // the consumer catch the producer
            return false;
        }

        // pop a data from queue 
        memcpy(&a_data, &m_theQueue[count_to_index(cur_readIndex)], sizeof(a_data));
        // cout << "reader: " << reader_id << "pop: " << a_data << "from: " << cur_readIndex <<endl;

	    if(m_min_read_index == cur_readIndex) {
	    	// cout<<"m_min_read_index == cur_readIndex = " << cur_readIndex << endl;
	    	do{
	    		// add, get the right to add read time
		        cur_read_time = slowest_read_time;

		    }while(!atomic_compare_exchange_weak(&slowest_read_time, &cur_read_time, (cur_read_time+1)));
		    // if every reader reader the slot
		    if(slowest_read_time % reader_num == 0 && slowest_read_time != 0) {
		        // have atomic problem?
		        m_min_read_index++;
		        // pop decrease the count
		        // have atomic problem?
        		--count;
		    }
	    }

        // no atomic problem(every reader has a distinct reader number), asign it directly
        m_readIndex_arr[reader_id] = cur_readIndex + 1;

        

		return true;

    }while(true);

    // Something went wrong. it shouldn't be possible to reach here
    assert(0);
    return false;
}


// get a reader number when a reader add to read
// return the reader number, or return -1 if reader is full
template <typename ELEM_T>
int MemQueue<ELEM_T>::add_reader(){

	// add reader, reader num + 1
	reader_num++;
	if(reader_num > MAX_READER_SIZE) {
		return -1;
	}
	m_readIndex_arr[reader_num - 1] = m_min_read_index;
	return reader_num-1;

}

template <typename ELEM_T>
int MemQueue<ELEM_T>::get_queue_size() {
    return count;
}

template <typename ELEM_T>
int MemQueue<ELEM_T>::get_reader_size() {
    return reader_num;
}

template <typename ELEM_T>
int MemQueue<ELEM_T>::get_write_index() {
	return m_write_index;
}

template <typename ELEM_T>
int MemQueue<ELEM_T>::get_max_read_index() {
    return m_max_read_index;
}

template <typename ELEM_T>
int MemQueue<ELEM_T>::get_min_read_index() {
    return m_min_read_index;
}

template <typename ELEM_T>
int MemQueue<ELEM_T>::get_slowest_read_time() {
    return slowest_read_time;
}


#endif// MEMQUEUE_H_