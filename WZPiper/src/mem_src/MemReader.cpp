#include "util/MemReader.h"
#include "util/logger.h"

extern Logger *logger;
char logger_buf[1024];

MemReader::MemReader(){
   this->m_flag = 3 ;
   this->m_key  = 0 ;
   this->m_size = 0 ;
   this->m_flag = SHM_FLAG ;
   this->m_shmid   = -1;
}

MemReader::~MemReader(){
   if(this->m_memory_addr != NULL) {
      detach_memory(m_memory_addr);
   }
}

// initialize the client shared memory address pointer
int MemReader::init_as_reader() {
   if(MemEngine::create_memory(this -> m_key, this -> m_size, this -> m_flag) ) {
      sprintf(logger_buf, "create_memory successfully");
      logger -> Info(logger_buf);

      // memcpy(&this->memQueue, m_memory_addr, sizeof(MEMQueue));

      this->memQueue = reinterpret_cast<MemQueue<Frame,3,2> * > (this->m_memory_addr);

      return 0;
   }
   return -1;
}


// read from the shared memory
int MemReader::do_read(Frame &mail) {
   int res = this -> memQueue -> pop(mail,this -> reader_index);
   return res;
}
