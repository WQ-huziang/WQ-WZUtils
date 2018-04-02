// Copyright(C) 2018, Wizard Quant
// Author: luoqingming
// Functions: single writer and multiple reader lockless queue with atomic
// Date: 2018-03-30

#ifndef MEMQUEUE_H_
#define MEMQUEUE_H_

#include <atomic>
#include <sched.h>
#include <assert.h>
#include <string.h>
#include <iostream>

// #ifndef PRT(...)
// #define PRT(...) printf(__VA_ARGS__)
// #define PRT(...)
// #endif

using std::atomic_compare_exchange_weak;

// #define MAX_READER_SIZE 20
// #define MAX_QUEUE_SIZE 4

template <typename ELEM_T, int queue_size, int reader_size>
class MemQueue{
private:
	// each reader's readIndex
    // int m_readIndex_arr[MAX_READER_SIZE];
    unsigned int m_readIndex_arr[reader_size];

    // the total number of reader start from 1
    volatile std::atomic<unsigned int> reader_num;

    // the next index to write(next time right to this index)
    volatile std::atomic<unsigned int> m_write_index;

    //the max can read index
    volatile std::atomic<unsigned int> m_max_read_index;

    // the last waiting to be read slot index
    volatile std::atomic<unsigned int> m_min_read_index;

    // queue to contain the data
    // ELEM_T m_theQueue[MAX_QUEUE_SIZE];
    ELEM_T m_theQueue[queue_size];

    // the every slot read time
    volatile std::atomic<unsigned int> read_time[queue_size];

    // transform the count number to real index of theQueue
    unsigned int count_to_index(unsigned int a_count);

    // max size of the reader list
    int MAX_READER_SIZE;

    // data queue max size
    int Q_SIZE;

    // count of the number of occupied queue slot
    volatile std::atomic<unsigned int> count;

public:
    MemQueue();
    virtual ~MemQueue();

    // initialize the queue
    bool init_queue();

    // add a reader to the queue
    unsigned int add_reader();

    // write a data to the queue
    bool push(const ELEM_T &a_data);

    // read a data from the queue
    bool pop(ELEM_T &a_data,int &read_num);

    unsigned int get_queue_size();

    int get_reader_size();

    unsigned int get_write_index();

    unsigned int get_max_read_index();

    unsigned int get_min_read_index();

    unsigned int get_read_time(int index = 0);

};


// map the current a_count index to the real queue index
template <typename ELEM_T, int queue_size, int reader_size>
unsigned int MemQueue<ELEM_T, queue_size, reader_size>::count_to_index(unsigned int a_count) {
    return (a_count%Q_SIZE);
}

// constructure
template <typename ELEM_T, int queue_size, int reader_size>
MemQueue <ELEM_T, queue_size, reader_size>::MemQueue() {
    this -> init_queue();
}

template <typename ELEM_T, int queue_size, int reader_size>
MemQueue<ELEM_T, queue_size, reader_size>::~MemQueue() {
    // delete[] m_theQueue;
    // delete[] m_readIndex_arr;
    // delete[] read_time;
}

template <typename ELEM_T, int queue_size, int reader_size>
bool MemQueue<ELEM_T, queue_size, reader_size>::init_queue(){
    this->reader_num = 0;
    this->m_write_index = 0;
    this->m_max_read_index = 0;
    this->m_min_read_index = 0;
    this->MAX_READER_SIZE = reader_size;
    this->Q_SIZE = queue_size;
    this->count = 0;

    memset(m_theQueue, 0, sizeof(m_theQueue));

    // set every readIndex as the zero value
    memset(m_readIndex_arr, 0, sizeof(m_readIndex_arr));

    for (int i = 0; i < queue_size; i++) {
        *(read_time + i) = 0;
    }

    return true;
}


template <typename ELEM_T, int queue_size, int reader_size>
bool MemQueue<ELEM_T, queue_size, reader_size>::push(const ELEM_T &a_data) {

    unsigned int cur_writeIndex;

    do{
        cur_writeIndex = m_write_index;

        // if the producer catch the slowest consumer
        if  (count_to_index(cur_writeIndex) == count_to_index(m_min_read_index) && count!= 0){
            // the queue is full
            // the producer catch the slowest consumer
            return false;
        }

    // add, get the right to write
    }while(!atomic_compare_exchange_weak(&m_write_index, &cur_writeIndex, (cur_writeIndex+1)));

    // write a_data to queue
    memcpy(&m_theQueue[count_to_index(cur_writeIndex)], &a_data, sizeof(a_data));

    // cout<<"m_theQueue= "<<m_theQueue[count_to_index(cur_writeIndex)] << "; count_to_index(cur_writeIndex) = " << count_to_index(cur_writeIndex) <<endl;
    // PRT("m_theQueue= %d; count_to_index(cur_writeIndex) = %d\n", m_theQueue[count_to_index(cur_writeIndex)], count_to_index(cur_writeIndex));

    // Consider that there is more than 1 producer thread
    // commit, wait for the producer finish the push operation, reset the upper bound of the to cur_writeIndex + 1
    while(!atomic_compare_exchange_weak(&m_max_read_index, &cur_writeIndex, (cur_writeIndex + 1))){
	// sched_yield();
    // 死等
    }
    ++count;
    return true;
}

template <typename ELEM_T, int queue_size, int reader_size>
bool MemQueue<ELEM_T, queue_size, reader_size>::pop(ELEM_T &a_data, int &reader_id) {
    unsigned int cur_readIndex, cur_max_readIndex, cur_read_time, nxt_read_time;

    // the reader is not exist
    if ( reader_id == -1 || reader_id >= reader_num) {
    	return false;
    }

    do{
        cur_readIndex = m_readIndex_arr[reader_id];
        cur_max_readIndex = m_max_read_index;

        // if the consumer catch the producer
        // if (count_to_index(cur_readIndex) == count_to_index(cur_max_readIndex)){
        // cur_readIndex always slower than cur_max_readIndex, no need to use count_to_index
        if (cur_readIndex == cur_max_readIndex){
            // the queue is empty
            // the consumer catch the producer
            return false;
        }

        // pop a data from queue 
        memcpy(&a_data, &m_theQueue[count_to_index(cur_readIndex)], sizeof(a_data));
        // cout << "reader: " << reader_id << "pop: " << a_data << "from: " << cur_readIndex <<endl;
        // PRT("reader: %d; pop: %d; from: %d\n", reader_id, a_data, cur_readIndex);

        do{
            // add, get the right to add read time
            cur_read_time = read_time[count_to_index(cur_readIndex)];

        }while(!atomic_compare_exchange_weak((read_time+count_to_index(cur_readIndex)), &cur_read_time, (cur_read_time + 1)));

	    if(m_min_read_index == cur_readIndex) {
	    	// cout<<"m_min_read_index == cur_readIndex = " << cur_readIndex << endl;
            // PRT("m_min_read_index == cur_readIndex =  %d\n", cur_readIndex);
		    
            // PRT("read_time[%d] =  %d\n", count_to_index(cur_readIndex), read_time[count_to_index(cur_readIndex)]);
            // if every reader have read the slot
		    if(read_time[count_to_index(cur_readIndex)] % reader_num == 0 && read_time[count_to_index(cur_readIndex)] != 0) {
		        // min_read_index++ has atomic problem?
		        // m_min_read_index++;
                unsigned int tmp_int;
                do{
                // add, get the right to add min_read_index
                    tmp_int = m_min_read_index;

                }while(!atomic_compare_exchange_weak(&m_min_read_index, &tmp_int, (tmp_int + 1)));

		        // 
		        // pop decrease the count has atomic problem?
        		// --count;
                do{
                // add, get the right to add min_read_index
                    tmp_int = count;

                }while(!atomic_compare_exchange_weak(&count, &tmp_int, (tmp_int - 1)));
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
template <typename ELEM_T, int queue_size, int reader_size>
unsigned int MemQueue<ELEM_T, queue_size, reader_size>::add_reader(){

	// add reader, reader num + 1
	reader_num++;
	if(reader_num > MAX_READER_SIZE) {
		return -1;
	}

    // reader add start reading from current min_read_index
    // reader_num start from 0, use reader_num - 1 as id
	m_readIndex_arr[reader_num - 1] = m_min_read_index;
	return reader_num-1;
}

template <typename ELEM_T, int queue_size, int reader_size>
unsigned int MemQueue<ELEM_T, queue_size, reader_size>::get_queue_size() {
    return count;
}

template <typename ELEM_T, int queue_size, int reader_size>
int MemQueue<ELEM_T, queue_size, reader_size>::get_reader_size() {
    return reader_num;
}

template <typename ELEM_T, int queue_size, int reader_size>
unsigned int MemQueue<ELEM_T, queue_size, reader_size>::get_write_index() {
	return m_write_index;
}

template <typename ELEM_T, int queue_size, int reader_size>
unsigned int MemQueue<ELEM_T, queue_size, reader_size>::get_max_read_index() {
    return m_max_read_index;
}

template <typename ELEM_T, int queue_size, int reader_size>
unsigned int MemQueue<ELEM_T, queue_size, reader_size>::get_min_read_index() {
    return m_min_read_index;
}

template <typename ELEM_T, int queue_size, int reader_size>
unsigned int MemQueue<ELEM_T, queue_size, reader_size>::get_read_time(int index) {
    if(index >= 0 && index < Q_SIZE)
        return read_time[index];
    return -1;
}


#endif// MEMQUEUE_H_