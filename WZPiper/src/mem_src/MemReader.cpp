#include "util/MemReader.h"
#include "util/logger.h"

extern Logger *logger;
char logger_buf[1024];

MemReader::MemReader(){
   this->m_flag = 3 ;
   this->m_key  = 0 ;
   this->m_size = 0 ;
   this->m_flag = SHM_FLAG ;
   this->m_shmid  = -1;
   this->queue_manager = NULL;
}

MemReader::~MemReader(){
   if(this->m_memory_addr != NULL) {
      detach_memory(this -> m_shmid, this -> m_memory_addr);
   }
}

// read key and size from configure file 
int MemReader::set_config_info(char file_path[256]) {
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

// initialize the client shared memory address pointer
int MemReader::init_as_reader() {

   sprintf(logger_buf, "Create memory MemInfo key = %d, MemInfo size = %d\n", this->m_key, this->m_size);
   logger -> Info(logger_buf);
   
   if(MemEngine::create_memory(this -> m_key, this -> m_size, this -> m_flag, this -> m_shmid, this -> m_memory_addr) ) {
      sprintf(logger_buf, "create_memory successfully");
      logger -> Info(logger_buf);

      // memcpy(&this->memQueue, m_memory_addr, sizeof(MEMQueue));

      this -> queue_manager = reinterpret_cast<QueueManager * > (this -> m_memory_addr);

      return 0;
   }
   return -1;
}


// read from the shared memory
int MemReader::do_read(Frame &mail) {
   int res = this -> queue_manager -> mem_queue.pop(mail,this -> reader_index);
   return res;
}
