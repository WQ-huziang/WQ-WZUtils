#ifndef MEMSERVER_H_
#define MEMSERVER_H_

#include "util/MemQueue.h"
#include "util/MemEngine.h"

class MemWriter : public MemEngine{
public:
	MemWriter();
	~MemWriter();

	int init_as_server();
	int do_write(Frame &mail);
private:
	MEMQueue *memQueue;
}

#endif MEMSERVER_H_
