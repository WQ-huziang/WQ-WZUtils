/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: huangxiaolin, luoqingming
Description: MemReader reads data from shared memory(pop data from the MemQueue)
Date: 2018-03-30
***************************************************************************/

#ifndef MEMCLIENT_H_
#define MEMCLIENT_H_

#include "MemEngine.h"
#include "MemQueue.hpp"
#include "frame.h"

class MemReader : public MemEngine{
public:
	MemReader();
	~MemReader();

    /************************************************* 
    Function: initAsReader
    Description: init as reader to read
    InputParameter: none
    Return: 0 if create succeed, -1 if failed
    *************************************************/
	int initAsReader();

    /************************************************* 
    Function: setConfigInfo
    Description: read configure file and set key and size
    InputParameter: none
    Return: 0 if create succeed, -1 if failed
    *************************************************/
	int setConfigInfo(char file_path[256]);

    /************************************************* 
    Function: readMem
    Description: read a frame from shared memory queue
    InputParameter: 
    	mail: pop(memcpy) a datum in queue to mail
    Return: 0 if create succeed, -1 if failed
    *************************************************/
	int readMem(Frame &mail);
	
private:

};

#endif // MEMCLIENT_H_
