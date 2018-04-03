/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: huangxiaolin, luoqingming
Description: MemReader reads data from shared memory(pop data from the MemQueue)
Date: 2018-03-30
***************************************************************************/

#include "MemReader.h"
#include "iniparser.h"
#include "logger.h"

// logger
extern Logger *logger;

// logger buffer
char logger_buf[1024];

MemReader::MemReader(){
   this->m_flag = 3 ;
   this->m_key  = 0 ;
   this->m_size = 0 ;
   this->m_flag = SHM_FLAG ;
   this->m_shmid  = -1;
   this->reader_index=0;
   this->queue_manager = NULL;
}

MemReader::~MemReader(){
   if(this->m_memory_addr != NULL) {
      detachMemory(this -> m_shmid, this -> m_memory_addr);
   }
}

// read key and size from configure file 
int MemReader::setConfigInfo(char file_path[256]) {
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
int MemReader::initAsReader() {

   sprintf(logger_buf, "Memory MemInfo key = %d, MemInfo size = %d\n", this->m_key, this->m_size);
   logger -> Info(logger_buf);
   
   if(MemEngine::createMemory(this -> m_key, this -> m_size, this -> m_flag, this -> m_shmid, this -> m_memory_addr) ) {

      // assign queue_manager to the first address of shared memory
      this -> queue_manager = reinterpret_cast<QueueManager * > (this -> m_memory_addr);

      // add as a reader and get the reader_id
      this -> reader_index = this -> queue_manager -> frame_mem_queue.addReader();

      sprintf(logger_buf, "reader id is:%d", this -> reader_index);
      logger -> Info(logger_buf);

      sprintf(logger_buf, "attach memory successfully");
      logger -> Info(logger_buf);

      return 0;
   }
   return -1;
}


// read from the shared memory
int MemReader::readMem(Frame &mail) {

   // printf("reader_index =%d\n", this -> reader_index);
   int res ;

   if(this -> queue_manager -> frame_mem_queue.pop(mail,this -> reader_index) == 1) {
      res = 0;
   }
   else {
      res = -1;
   }

   return res;
}
