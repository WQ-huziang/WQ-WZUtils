/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: huangxiaolin, luoqingming
Description:struct template QueueManager:manage the MemQueue used in shared memory
            class template MemEngine:reads data from shared memory(pop data from the MemQueue)
                        and writes data into shared memory(push data to the MemQueue)
Date: 2018-04-09
***************************************************************************/

#ifndef MEMENGINE_H_
#define MEMENGINE_H_

#include "wzpiper.hpp"
#include "memqueue.hpp"
#include "frame.h"
#include "logger.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#ifndef PRT
#define PRT(...) printf(__VA_ARGS__);
//#define PRT(...)
#endif

#ifndef SHM_FAILED
#define SHM_FAILED -1 
#endif

#ifndef SHM_FLAG
#define SHM_FLAG IPC_CREAT|0666
#endif

// #ifndef QueueDataType
// #define QueueDataType Frame
// #endif

// #ifndef DataQueueSize
// #define DataQueueSize 2048
// #endif

// #ifndef MaxReaderSize
// #define MaxReaderSize 4
// #endif

// logger
extern Logger *logger;

// logger buffer
char logger_buf[1024];

/***************************************************************************
Description: QueueManager manage the MemQueue used in shared memory
****************************************************************************/
template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
struct QueueManager
{

   // the MemQueue<DataType, DataQueueSize(must be 2^n), MaxReaderSize>
   MemQueue<QueueDataType, DataQueueSize, MaxReaderSize > frame_rec_queue;
   MemQueue<QueueDataType, DataQueueSize, MaxReaderSize > frame_req_queue;

   // Return: 1 if initManager succeed, 0 if failed
   bool initManager(){

      // call different queue's init
      if (!frame_req_queue.initQueue()) {
         return false;
      }

      if (!frame_rec_queue.initQueue()) {
         return false;
      }

      return true;
   }
};

/***************************************************************************
Description: MemEngine reads data from shared memory(pop data from the MemQueue)
             MemEngine writes data into shared memory(push data to the MemQueue)
****************************************************************************/
template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
class MemEngine{
public:

    /************************************************* 
    Function: MemEngine
    Description: Constructor, read key and size from configure file 
       create or attach the shared memory
       and init the queue_manager according to piperMode
    InputParameter: 
       piperMode:  the flag to mark server or client, 
         0 or WZ_PIPER_SERVER as server,
         1 or WZ_PIPER_CLIENT as client
    Return: none
    *************************************************/
    MemEngine(int piperMode);

    /************************************************* 
    Function: ~MemEngine
    Description: Destructor, detach the shared memory
    InputParameter: none
    Return: none
    *************************************************/
    ~MemEngine();

    /************************************************* 
    Function: init
    Description: read configure file and init as server or client,
      server will create the shared memory and init the QueueManager
    InputParameter: none
    Return: true if create succeed, false if failed
    *************************************************/
    bool init(char file_path[256]);

    /************************************************* 
    Function: wzRecv
    Description: read a frame from shared memory queue
    InputParameter: 
    	frame: pop(memcpy) a datum in queue to mail
    Return: 1 if receive succeed, 0 if failed
    *************************************************/
    int wzRecv(QueueDataType &data);

    /************************************************* 
    Function: wzSend
    Description: write a frame to shared memory queue
    InputParameter: 
    	frame: the datum to push(memcpy) into queue
    Return: 1 if send succeed, 0 if failed
    *************************************************/
    int wzSend(QueueDataType &data);
	  
    /************************************************* 
    Function: createMemory
    Description: create shared memory function
    InputParameter: 
       m_key: the key of to-create shared memory
       m_size: the size of to-create shared memory
       m_flag: the shm flag of to-create shared memory
       m_shmid: a initial int id, will be set to
          the shared memory id after calling the function.
       m_memory_addr: a initial address, will be set point to
          the shared memory first address after calling the function.
    Return: 1 if create succeed, 0 if failed
    *************************************************/
    bool createMemory(const int &m_key, const int &m_size, const int &m_flag, int &m_shmid, char* & m_memory_addr);


    /************************************************* 
    Function: destroyMemory
    Description: destroy shared memory function
    InputParameter: 
       m_shmid: the to-destroy shared memory id, will be set to -1 if succeed.
       m_memory_addr: the pointer point to to-destroy shared memory address, will be set to NULL if succeed.
    Return: 1 if destroy succeed, 0 if failed
    *************************************************/
    bool destroyMemory(int & m_shmid, char* & m_memory_addr);


    /************************************************* 
    Function: attachMemory
    Description: attach shared memory function
    InputParameter:
       m_key: the to-attach shared memory key.
       m_shmid: the to-attach shared memory id.
       m_flag: the to-attach shared memory flag.
       m_memory_addr: the pointer will be point to to-attach shared memory address.
    Return: 1 if attach succeed, 0 if failed
    *************************************************/
    bool attachMemory(const int & m_key, int & m_shmid, const int & m_flag, char*& m_memory_addr);


    /************************************************* 
    Function: detachMemory
    Description: detach shared memory function
    InputParameter:
       m_shmid: the to-detach shared memory id.
       m_memory_addr: the pointer point to to-detach shared memory address.
    Return: 1 if detach succeed, 0 if failed
    *************************************************/
    bool detachMemory(const int & m_shmid, char*& m_memory_addr);

private:
    QueueManager<QueueDataType, DataQueueSize, MaxReaderSize> *queue_manager;// pointer point to the queue manager in shared memory address
    int reader_index; 			    // reader index
    int m_key;					        // shared memory key
    int m_size;					        // shared memory size
    int m_flag;					        // shared memory flag
    int m_shmid;				        // shared memory descriptor
    char *m_memory_addr;		    // shared memory address pointer
    int piperMode;              // the flag to mark server or client, 0 as server, 1 as client
};

// server:piperMode = 0  client:piperMode = 1; 
template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
MemEngine<QueueDataType, DataQueueSize, MaxReaderSize>::MemEngine(int piperMode){
   this->m_flag = 3 ;
   this->m_key  = 0 ;
   this->m_size = 0 ;
   this->m_flag = SHM_FLAG ;
   this->m_shmid  = -1;
   this->reader_index=0;
   this->queue_manager = NULL;
   this->piperMode = piperMode;
}

template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
MemEngine<QueueDataType, DataQueueSize, MaxReaderSize>::~MemEngine(){
   if(this->m_memory_addr != NULL) {
      detachMemory(this -> m_shmid, this -> m_memory_addr);
   }
}

// create shared memory function
template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
bool MemEngine<QueueDataType, DataQueueSize, MaxReaderSize>::createMemory(const int &m_key, const int &m_size, const int &m_flag, int &m_shmid, char* & m_memory_addr) {
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
template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
bool MemEngine<QueueDataType, DataQueueSize, MaxReaderSize>::destroyMemory(int & shmid, char* & m_memory_addr) {

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
template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
bool MemEngine<QueueDataType, DataQueueSize, MaxReaderSize>::attachMemory(const int & m_key, int & m_shmid, const int & m_flag, char*& m_memory_addr) {

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
template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
bool MemEngine<QueueDataType, DataQueueSize, MaxReaderSize>::detachMemory(const int & m_shmid, char*& m_memory_addr) {
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
template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
bool MemEngine<QueueDataType, DataQueueSize, MaxReaderSize>::init(char file_path[256]){
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
template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
int MemEngine<QueueDataType, DataQueueSize, MaxReaderSize>::wzRecv(QueueDataType &data) {

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
template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
int MemEngine<QueueDataType, DataQueueSize, MaxReaderSize>::wzSend(QueueDataType &data) {

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

#endif // MEMENGINE_H_
