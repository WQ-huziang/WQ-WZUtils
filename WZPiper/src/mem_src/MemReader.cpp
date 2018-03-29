#include "util/MemReader.h"

extern Logger *logger;
char logger_buf[1024];

MemReader::MemReader(){

}

MemReader::~MemReader(){}

// initialize the client shared memory address pointer
int MemReader::init_as_client() {
   if(MemEngine::create_memory(this -> m_key, this -> m_size, this -> m_flag) ) {
      sprintf(logger_buf, "create_memory successfully");
      logger -> Info(logger_buf);

      memcpy(&this->memQueue, m_memory_addr, sizeof(MEMQueue));
      return 0;
   }
   return -1;
}

// read from the shared memory
int MemReader::do_read(Frame &mail) {
   int res = this -> memQueue -> pop(mail,this -> reader_index);
   return res;
}
