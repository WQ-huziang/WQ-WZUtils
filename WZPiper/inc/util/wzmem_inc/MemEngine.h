/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: huangxiaolin, luoqingming
Description: MemEngine create attach dettach destroy a shared memory
Date: 2018-03-30
***************************************************************************/

#ifndef MEMENGINE_H_
#define MEMENGINE_H_

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "frame.h"
#include "MemQueue.hpp"
#include "logger.h"

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

#ifndef QueueDataType
#define QueueDataType Frame
#endif

#ifndef DataQueueSize
#define DataQueueSize 1024
#endif

#ifndef MaxReaderSize
#define MaxReaderSize 2
#endif

/***************************************************************************
Description: QueueManager manage the MemQueue used in shared memory
****************************************************************************/

struct QueueManager
{

   // the MemQueue<DataType, DataQueueSize(must be 2^n), MaxReaderSize>
   // MemQueue<QueueDataType, DataQueueSize, MaxReaderSize> frame_mem_queue;
   MemQueue<QueueDataType, > frame_rec_queue;
   MemQueue<QueueDataType, > frame_req_queue;

   // Return: 1 if initManager succeed, 0 if failed
   bool initManager(int size, int reader_size){
      // init MemQueue
      int rtn = true;

      // call different queue's init
      // if (!frame_mem_queue.initQueue()) {
      //    rtn = false;
      // }

      if (!frame_rec_queue.initQueue(int size, int reader_size)) {
         rtn = false;
      }

      if (!frame_req_queue.initQueue()) {
         rtn = false;
      }
      
      return rtn;
   }
   
};

/***************************************************************************
Description: MemEngine create attach dettach destroy a shared memory
****************************************************************************/
class MemBuilder{
 
 public:
   
   // build-in function
   MemBuilder(){};
   virtual ~MemBuilder(){};

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
   Return: 1 if create succeed, 0 if failed
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
   Return: 1 if create succeed, 0 if failed
   *************************************************/
   bool attachMemory(const int & m_key, int & m_shmid, const int & m_flag, char*& m_memory_addr);

   /************************************************* 
   Function: detachMemory
   Description: detach shared memory function
   InputParameter:
      m_shmid: the to-detach shared memory id.
      m_memory_addr: the pointer point to to-detach shared memory address.
   Return: 1 if create succeed, 0 if failed
   *************************************************/
   bool detachMemory(const int & m_shmid, char*& m_memory_addr);

   // return the shmkey
   virtual int getKey(){};

   // return the shmsize
   virtual int getSize(){};

   // return the shmflag
   virtual int getFlag(){};

   // return the shmid
   virtual int getShmid(){};

   // get a char pointer point to the first address of shared memory
   virtual char * getMemoryAddr(){};

   // set info accordding to config
   virtual int setConfigInfo(char file_path[256]){};

   // use as a reader read a data from queue
   virtual int initAsReader();

   // use as a writer write a data to queue
   virtual int initAsWriter();

   // write a frame to shared memory
   virtual int writeMem(Frame &frame);

   // read a frame from shared memory
   virtual int readMem(Frame &frame);


 private:
   QueueManager *queue_manager;// pointer point to the queue manager in shared memory address

   (Type, queuesize, reader_index)

   template <typename T>
   QueueManager* malloc_queuemanager(int queuesize, int reader_index){
      void *head = m_memory_addr;
      queue_manager = (Queue_manager*) head;
      queue_manager->init(queuesize, reader_index);      
      // queue_manager->frame_rec_queue = (QueueManager*) head;
      // queue_manager->frame_rec_queue->init
      // int size = sizeof( QueueManager<T>(queuesize, reader_index));
      return queue_manager;
   }

   int reader_index;          // reader index
   int m_key;              // shared memory key
   int m_size;             // shared memory size
   int m_flag;             // shared memory flag
   int m_shmid;            // shared memory descriptor
   char *m_memory_addr;    // shared memory address pointer
 public:

};

#endif // MEMENGINE_H_
