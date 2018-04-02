#include "util/MemWriter.h"
#include "util/logger.h"

#define MAX_QUEUE_SIZE 20

extern Logger *logger;
char logger_buf[1024];

MemWriter::MemWriter(){
   this->m_flag = 3 ;
   this->m_key  = 0 ;
   this->m_size = 1024;
   this->m_flag = SHM_FLAG ;
   this->m_shmid   = -1;
}

MemWriter::~MemWriter(){
   if(this->m_memory_addr != NULL) {
      detach_memory(m_memory_addr);
   }
}

// initialize the server shared memory address pointer
int MemWriter::init_as_writer() {
   if (MemEngine::create_memory(this -> m_key, this -> m_size, this -> m_flag) ) {
      sprintf(logger_buf, "create_memory successfully");
      logger -> Info(logger_buf);
      
      // memcpy(&m_memory_addr, this->memQueue, sizeof(MEMQueue));

      this->memQueue = reinterpret_cast<MemQueue<Frame,3,2> * > (this ->m_memory_addr);

      return 0;
   }
   return -1;
}

// write from the shared memory
int MemWriter::do_write(Frame &mail) {
   int res = this -> memQueue -> push(mail);
   return res;
}