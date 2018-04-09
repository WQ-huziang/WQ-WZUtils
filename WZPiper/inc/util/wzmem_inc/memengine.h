/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: huangxiaolin, luoqingming
Description: MemEngine reads data from shared memory(pop data from the MemQueue)
			 MemEngine writes data into shared memory(push data to the MemQueue)
Date: 2018-04-09
***************************************************************************/

#ifndef MEMCLIENT_H_
#define MEMCLIENT_H_

#include "wzpiper.h"
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
   //MemQueue<QueueDataType, DataQueueSize, MaxReaderSize> frame_mem_queue;
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

class MemEngine{
public:
	   MemEngine(int server_client_flag);
	   ~MemEngine();

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




    /************************************************* 
    Function: init
    Description: read configure file and init as reader and writer
    InputParameter: none
    Return: true if create succeed, false if failed
    *************************************************/
	  bool init(char file_path[256]);

    /************************************************* 
    Function: readMem
    Description: read a frame from shared memory queue
    InputParameter: 
    	mail: pop(memcpy) a datum in queue to mail
    Return: true if create succeed, false if failed
    *************************************************/
	  bool wzRecv(Frame &mail);

	  /************************************************* 
    Function: writeMem
    Description: write a frame to shared memory queue
    InputParameter: 
    	mail: push (memcpy) a datum to push into queue
    Return: true if create succeed, false if failed
    *************************************************/
	  bool wzSend(Frame &mail);
	
private:
  	QueueManager *queue_manager;// pointer point to the queue manager in shared memory address
  	int reader_index; 			// reader index
  	int m_key;					// shared memory key
  	int m_size;					// shared memory size
  	int m_flag;					// shared memory flag
  	int m_shmid;				// shared memory descriptor
  	char *m_memory_addr;		// shared memory address pointer
  	int server_client_flag;     // the flag to mark server or client
};

#endif // MEMCLIENT_H_
