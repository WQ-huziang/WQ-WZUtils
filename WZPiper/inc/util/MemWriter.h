// Copyright(C) 2018, Wizard Quant
// Author: huangxiaolin, luoqingming
// Functions: MemWriter writes data to shared memory(push data to the MemQueue)
// Date: 2018-03-30

#ifndef MEMSERVER_H_
#define MEMSERVER_H_

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "util/MemQueue.hpp"
#include "util/MemEngine.h"
#include "wzadapter/frame.h"

class MemWriter : public MemEngine{
public:
	MemWriter();
	~MemWriter();

	int init_as_writer();
	int set_config_info(char file_path[256]);
	int do_write(Frame &mail);

	// int get_key();

	// int get_size();

	// int get_flag();

	// // return the shmid
	// int get_shmid();
	
private:
	QueueManager *queue_manager;
	int m_key;			// shared memory key
	int m_size;			// shared memory size
	int m_flag;			// shared memory flag
	int m_shmid;			// shared memory descriptor
	char *m_memory_addr;	// shared memory address pointer 
};

#endif // MEMSERVER_H_
