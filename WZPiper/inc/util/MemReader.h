#ifndef MEMCLIENT_H_
#define MEMCLIENT_H_

#include "util/MemEngine.h"
#include "util/MemQueue.h"

class MemReader : public MemEngine{
public:
	MemReader();
	~MemReader();

	int init_as_client();
	int do_read(Frame &mail);
private:
	MEMQueue *memQueue;
	int reader_index;
};

#endif MEMCLIENT_H_