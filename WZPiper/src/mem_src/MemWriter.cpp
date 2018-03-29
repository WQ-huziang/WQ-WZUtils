#include "util/MemWriter.h"

#define MAX_QUEUE_SIZE 20

extern Logger *logger;
char logger_buf[1024];

MemWriter::MemWriter(){
	this->memQueue = new MEMQueue(MAX_QUEUE_SIZE);
}

MemWriter::~MemWriter(){}

// initialize the server shared memory address pointer
int MemWriter::init_as_server() {
   if (MemEngine::create_memory(this -> m_key, this -> m_size, this -> m_flag) ) {
      sprintf(logger_buf, "create_memory successfully");
      logger -> Info(logger_buf);
      
      memcpy(&m_memory_addr, this->memQueue, sizeof(MEMQueue));

      return 0;
   }
   return -1;
}

// write from the shared memory
int MemWriter::do_write(Frame &mail) {
   int res = this -> memQueue -> push(mail);
   return res;
}