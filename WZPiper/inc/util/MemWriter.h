// Copyright(C) 2018, Wizard Quant
// Author: huangxiaolin, luoqingming
// Functions: MemWriter writes data to shared memory(push data to the MemQueue)
// Date: 2018-03-30

#ifndef MEMSERVER_H_
#define MEMSERVER_H_

#include "util/MemQueue.hpp"
#include "util/MemEngine.h"
#include "wzadapter/frame.h"

class MemWriter : public MemEngine{
public:
	MemWriter();
	~MemWriter();

	int init_as_writer();
	int do_write(Frame &mail);
private:
	MemQueue<Frame,3,2> *memQueue;
	int m_key;			// shared memory key
	int m_size;			// shared memory size
	int m_flag;			// shared memory flag
	int m_shmid;			// shared memory descriptor
	char *m_memory_addr;	// shared memory address pointer 
};

#endif // MEMSERVER_H_
