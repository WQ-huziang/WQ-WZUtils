/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: huangxiaolin, luoqingming
Description: MemWriter writes data to shared memory(push data to the MemQueue)
Date: 2018-03-30
****************************************************************************/

#ifndef MEMSERVER_H_
#define MEMSERVER_H_

#include "MemQueue.hpp"
#include "MemEngine.h"
#include "frame.h"

class MemWriter : public MemEngine{
public:
	// init
	MemWriter();
	~MemWriter();

    /************************************************* 
    Function: initAsWriter
    Description: init as writer to read
    InputParameter: none
    Return: 0 if create succeed, -1 if failed
    *************************************************/
	int initAsWriter();

    /************************************************* 
    Function: setConfigInfo
    Description: read configure file and set key and size
    InputParameter: none
    Return: 0 if create succeed, -1 if failed
    *************************************************/
	int setConfigInfo(char file_path[256]);

    /************************************************* 
    Function: writeMem
    Description: write a frame to shared memory queue
    InputParameter: 
    	mail: push (memcpy) a datum to push into queue
    Return: 0 if create succeed, -1 if failed
    *************************************************/
	int writeMem(Frame &mail);
	
private:
	QueueManager *queue_manager;// pointer point to the queue manager in shared memory address
	int m_key;					// shared memory key
	int m_size;					// shared memory size
	int m_flag;					// shared memory flag
	int m_shmid;				// shared memory descriptor
	char *m_memory_addr;		// shared memory address pointer 
};

#endif // MEMSERVER_H_
