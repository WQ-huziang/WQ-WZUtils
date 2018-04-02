// Copyright(C) 2018, Wizard Quant
// Author: huangxiaolin, luoqingming
// Functions: MemWriter writes data to shared memory(push data to the MemQueue)
// Date: 2018-03-30

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
   printf("call ~MemWriter\n");
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

   if (MemEngine::create_memory(this -> m_key, this -> m_size, this -> m_flag, this -> m_shmid, this -> m_memory_addr) ) {

      sprintf(logger_buf, "Create memory MemInfo key = %d, MemInfo size = %d\n", this->m_key, this->m_size);
      logger -> Info(logger_buf);
      
      // assign queue_manager to the first address of shared memory
      this -> queue_manager = reinterpret_cast<QueueManager * > (this -> m_memory_addr) ;

      // initialize the queues before using
      this -> queue_manager -> init_manager();

      sprintf(logger_buf, "check reader size:%d\n", this -> queue_manager -> mem_queue.get_reader_size());
      logger -> Info(logger_buf);

      sprintf(logger_buf, "init as writer successfully");
      logger -> Info(logger_buf);

      return 0;
   }

   sprintf(logger_buf, "Failed to init as writer, create memory failed.");
   logger -> Error(logger_buf);

   return -1;
}

// write from the shared memory
int MemWriter::write_mem(Frame &mail) {

   int res = this -> queue_manager -> mem_queue.push(mail);

   return res;
}
