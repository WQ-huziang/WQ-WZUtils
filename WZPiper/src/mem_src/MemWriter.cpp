#include "util/MemWriter.h"
#include "util/logger.h"

#define MAX_QUEUE_SIZE 20

extern Logger *logger;
char logger_buf[1024];

MemWriter::MemWriter(){
	this->memQueue = new MemQueue<Frame, 3, 2>;
}

MemWriter::~MemWriter(){}

// initialize the server shared memory address pointer
int MemWriter::init_as_server() {
   if (MemEngine::create_memory(this -> get_key(), this -> get_size(), this -> get_flag()) ) {
      sprintf(logger_buf, "create_memory successfully");
      logger -> Info(logger_buf);
      
      // memcpy(&m_memory_addr, this->memQueue, sizeof(MEMQueue));

      this->memQueue = reinterpret_cast<MemQueue<Frame,3,2> * > (this ->get_memory_addr());

      return 0;
   }
   return -1;
}

// write from the shared memory
int MemWriter::do_write(Frame &mail) {
   int res = this -> memQueue -> push(mail);
   return res;
}