#include "util/MemReader.h"
#include "util/logger.h"

extern Logger *logger;
char logger_buf[1024];

MemReader::MemReader(){

}

MemReader::~MemReader(){}

// initialize the client shared memory address pointer
int MemReader::init_as_client() {
   if(MemEngine::create_memory(this -> get_key(), this -> get_size(), this -> get_flag()) ) {
      sprintf(logger_buf, "create_memory successfully");
      logger -> Info(logger_buf);

      // memcpy(&this->memQueue, m_memory_addr, sizeof(MEMQueue));

      this->memQueue = reinterpret_cast<MemQueue<Frame,3,2> * > (this->get_memory_addr());

      return 0;
   }
   return -1;
}


// read from the shared memory
int MemReader::do_read(Frame &mail) {
   int res = this -> memQueue -> pop(mail,this -> reader_index);
   return res;
}
