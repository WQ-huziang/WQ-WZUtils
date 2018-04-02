// Copyright(C) 2018, Wizard Quant
// Author: huangxiaolin, luoqingming
// Functions: MemReader reads data from shared memory(pop data from the MemQueue)
// Date: 2018-03-30

#ifndef MEMCLIENT_H_
#define MEMCLIENT_H_

#include "util/MemEngine.h"
#include "util/MemQueue.hpp"
#include "wzadapter/frame.h"

class MemReader : public MemEngine{
public:
	MemReader();
	~MemReader();

	int init_as_reader();
	int do_read(Frame &mail);
private:
	MemQueue<Frame,3,2> *memQueue;
	int reader_index;
	int m_key;			// shared memory key
	int m_size;			// shared memory size
	int m_flag;			// shared memory flag
	int m_shmid;			// shared memory descriptor
	char *m_memory_addr;	// shared memory address pointer 
};

#endif // MEMCLIENT_H_
