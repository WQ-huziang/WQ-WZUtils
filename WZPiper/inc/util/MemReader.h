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

	// init as reader to read
	int init_as_reader();

	int set_config_info(char file_path[256]);
	int read_mem(Frame &mail);

	// int get_key();

	// int get_size();

	// int get_flag();

	// // return the shmid
	// int get_shmid();
	
private:
	QueueManager *queue_manager;
	int reader_index;
	int m_key;			// shared memory key
	int m_size;			// shared memory size
	int m_flag;			// shared memory flag
	int m_shmid;			// shared memory descriptor
	char *m_memory_addr;	// shared memory address pointer 
};

#endif // MEMCLIENT_H_
