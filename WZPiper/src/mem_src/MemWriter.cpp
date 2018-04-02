#include "util/MemWriter.h"
#include "util/logger.h"

#define MAX_QUEUE_SIZE 20

extern Logger *logger;
char logger_buf[1024];

MemWriter::MemWriter(){
   this->m_flag = 3 ;
   this->m_key  = 1 ;
   this->m_size = 1024;
   this->m_flag = SHM_FLAG ;
   this->m_shmid = -1;
   this->queue_manager = NULL;
}

MemWriter::~MemWriter(){
   if(this->m_memory_addr != NULL) {
      detach_memory(this -> m_shmid, this -> m_memory_addr);
   }
   if(this->m_memory_addr != NULL) {
      destroy_memory(this -> m_shmid, this -> m_memory_addr);
   }
}

// read key and size from configure file 
int MemWriter::set_config_info(char file_path[256]) {
   CIni ini;
   if (ini.OpenFile(file_path, "r") == INI_OPENFILE_ERROR){
      sprintf(logger_buf, "INI_OPENFILE_ERROR");
      logger -> Info(logger_buf);
      return -1;
   }

   this -> m_key = ini.GetInt("MemInfo","key");

   this -> m_size = ini.GetInt("MemInfo", "memorysize");

   sprintf(logger_buf, "Config read MemInfo key = %d, MemInfo size = %d\n", this->m_key, this->m_size);
   logger -> Info(logger_buf);

   return 0;
}

// initialize the server shared memory address pointer
int MemWriter::init_as_writer() {

   sprintf(logger_buf, "Create memory MemInfo key = %d, MemInfo size = %d\n", this->m_key, this->m_size);
   logger -> Info(logger_buf);

   if (MemEngine::create_memory(this -> m_key, this -> m_size, this -> m_flag, this -> m_shmid, this -> m_memory_addr) ) {
      
      printf("Before asigin to queue_manager\n" );

      // copy the queue to shared memory
      // memcpy(&m_memory_addr, this->memQueue, sizeof(this->memQueue));

      this -> queue_manager = (QueueManager *) (this -> m_memory_addr) ;

      printf("After asigin to queue_manager\n");

      printf("queue_manager size is:%ld\n",sizeof(*queue_manager) );

      this -> queue_manager -> init_manager();

      printf("After init to queue_manager\n");

      printf("%d\n", this -> queue_manager -> mem_queue.get_reader_size());

      printf("create_memory successfully\n");

      return 0;
   }
   return -1;
}

// write from the shared memory
int MemWriter::do_write(Frame &mail) {

   int res = this -> queue_manager -> mem_queue.push(mail);

   return res;
}
