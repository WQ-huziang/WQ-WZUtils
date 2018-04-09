/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: huangxiaolin, luoqingming
Description: MemEngine reads data from shared memory(pop data from the MemQueue)
             MemEngine writes data into shared memory(push data to the MemQueue)
Date: 2018-04-09
***************************************************************************/

// #include "memengine.h"
#include "iniparser.h"
#include "logger.h"

// logger
extern Logger *logger;

// logger buffer
char logger_buf[1024];

// server:piperMode = 0  client:piperMode = 1; 
MemEngine::MemEngine(int piperMode){
   this->m_flag = 3 ;
   this->m_key  = 0 ;
   this->m_size = 0 ;
   this->m_flag = SHM_FLAG ;
   this->m_shmid  = -1;
   this->reader_index=0;
   this->queue_manager = NULL;
   this->piperMode = piperMode;
}

MemEngine::~MemEngine(){
   if(this->m_memory_addr != NULL) {
      detachMemory(this -> m_shmid, this -> m_memory_addr);
   }
}

// create shared memory function
bool MemEngine::createMemory(const int &m_key, const int &m_size, const int &m_flag, int &m_shmid, char* & m_memory_addr) {
   // call shmget and use return value to initialize shared memory address pointer
   m_shmid = shmget(m_key, m_size, m_flag);

   if ( m_shmid == -1 ) {  
      sprintf(logger_buf, "shared memory create failed");
      logger -> Error(logger_buf);
      return false;
   }

   // call shmat to attach shared memory
   m_memory_addr = reinterpret_cast<char*>(shmat(m_shmid, NULL, 0));

   if ( m_memory_addr == (char*)SHM_FAILED ) {
      sprintf(logger_buf, "shared memory m_memory_addr attach failed");
      logger -> Error(logger_buf);
      return false;
   }

   sprintf(logger_buf, "shared memory create succeed, key = %d, size = %d, shmid = %d", m_key, m_size, m_shmid);
   logger -> Info(logger_buf);

   return true;
}

// destroy shared memory function
bool MemEngine::destroyMemory(int & shmid, char* & m_memory_addr) {

   if (shmid == -1) {
      sprintf(logger_buf, "destroy memory failed, shmid = -1");
      logger -> Error(logger_buf);
      return false;
   }
   
   if (m_memory_addr != NULL) {
      if (shmdt(m_memory_addr) != 0) {
         sprintf(logger_buf, "call shmdt failed");
         logger -> Error(logger_buf);
         return false;
      }
      m_memory_addr = NULL;
   }

   // call shmctl destroy the memory
   if (shmctl(shmid, IPC_RMID, NULL) != 0) {
      sprintf(logger_buf, "call shmctl destroy memory failed");
      logger -> Error(logger_buf);
      return false;
   }
   
   shmid = -1;
   m_memory_addr = NULL;
   sprintf(logger_buf, "destroy memory succeed");
   logger -> Info(logger_buf);

   return true;
}

// attach shared memory function
bool MemEngine::attachMemory(const int & m_key, int & m_shmid, const int & m_flag, char*& m_memory_addr) {

   // is the id exist
   if (m_shmid == SHM_FAILED ) {
      // is not exist the recreate it according to m_key
      m_shmid = shmget(m_key, 0, m_flag);
      sprintf(logger_buf, "recreate shared memory");
      logger -> Info(logger_buf);
   }

   // call shmat to attach and return address
   m_memory_addr = reinterpret_cast<char*> (shmat(m_shmid, NULL, 0));

   // is null
   if (m_memory_addr == (char*) SHM_FAILED ) {
      sprintf(logger_buf, "the memory address is null");
      logger -> Error(logger_buf);
      return false;
   }

   return true;
}

// detach shared memory function
bool MemEngine::detachMemory(const int & m_shmid, char*& m_memory_addr) {
   // is private variable address valid
   if (m_shmid == -1 || m_memory_addr == NULL) {
      // not valid
      return true;
   }

   // call detach and return to nCode
   int nCode = shmdt(m_memory_addr);

   if (nCode != 0) {
     if(errno == EINVAL) {
         m_memory_addr = NULL;
         sprintf(logger_buf, "*Already shmdt*");
         logger -> Info(logger_buf);
         return true;  
     }
     return false;
   }

   m_memory_addr = NULL;
   return true;
}


// initialize the client shared memory address pointer
bool MemEngine::init(char file_path[256]){
    // read key and size from configure file 
    CIni ini;
    if (ini.OpenFile(file_path, "r") == INI_OPENFILE_ERROR){
       sprintf(logger_buf, "INI_OPENFILE_ERROR");
       logger -> Info(logger_buf);
       return false;
    }
   this -> m_key = ini.GetInt("MemInfo","key");
   this -> m_size = ini.GetInt("MemInfo", "memorysize");

   sprintf(logger_buf, "Config read MemInfo key = %d, MemInfo size = %d\n", this->m_key, this->m_size);
   logger -> Info(logger_buf);
   
   // init the queue_manager
   // if(createMemory(this -> m_key, this -> m_size, this -> m_flag, this -> m_shmid, this -> m_memory_addr) ) {
   if(createMemory(this -> m_key, sizeof(QueueManager<QueueDataType, DataQueueSize, MaxReaderSize>), this -> m_flag, this -> m_shmid, this -> m_memory_addr) ) {
      // assign queue_manager to the first address of shared memory
      this -> queue_manager = reinterpret_cast<QueueManager<QueueDataType, DataQueueSize, MaxReaderSize> * > (this -> m_memory_addr);

      if(this->piperMode == 0){
         // initialize the queues before using
         this -> queue_manager -> initManager();
         // add as a reader of the frame_req_queue and get the reader_id
         this -> reader_index = this -> queue_manager -> frame_req_queue.addReader();
      }
      else if(this->piperMode == 1){
         // add as a reader of the frame_rec_queue and get the reader_id
         this -> reader_index = this -> queue_manager -> frame_rec_queue.addReader();
      }

      //printf("sizeof queue_manager:%ld\n", sizeof(*queue_manager) );

      sprintf(logger_buf, "Reader id is:%d", this -> reader_index);
      logger -> Info(logger_buf);

      sprintf(logger_buf, "attach memory successfully");
      logger -> Info(logger_buf);

      return true;
   }
   return false;
}


// read from the shared memory
int MemEngine::wzRecv(QueueDataType &data) {

   if(this->piperMode == 1 && this -> queue_manager -> frame_rec_queue.pop(data,this -> reader_index) == 1) {
      return 0;
   } // client
   else if(this->piperMode == 0 && this -> queue_manager -> frame_req_queue.pop(data,this -> reader_index) == 1){
      return 0;
   } // server
   else {
      return -1;
   }
}

// write from the shared memory
int MemEngine::wzSend(QueueDataType &data) {

   if(this->piperMode == 1 && this -> queue_manager -> frame_req_queue.push(data) == 1) {
      return 0;
   }// client
   else if(this->piperMode == 0 && this -> queue_manager -> frame_rec_queue.push(data) == 1){
      return 0;
   }// server
   else {
      return -1;
   }
}
