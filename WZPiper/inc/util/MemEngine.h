// Copyright(C) 2018, Wizard Quant
// Author: huangxiaolin, luoqingming
// Functions: MemEngine create attach dettach destroy a shared memory
// Date: 2018-03-30

#ifndef MEMENGINE_H_
#define MEMENGINE_H_

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "wzadapter/frame.h"
#include "util/MemQueue.hpp"

#define PRT(...) printf(__VA_ARGS__);
#define SHM_FAILED -1 
#define SHM_DATA_SIZE 2048
#define SHM_FLAG IPC_CREAT|0666

typedef struct QueueManager_
{
   MemQueue<Frame,3,2> mem_queue;
   bool init_manager(){
      return mem_queue.init_queue();
   }
   
}QueueManager;

class MemEngine{
 
 public:
   
   // build-in function
   MemEngine(){};
   virtual ~MemEngine(){};

   // create shared memory function
   bool create_memory(const int &m_key, const int &m_size, const int &m_flag, int &m_shmid, char* & m_memory_addr);

   // destroy shared memory function
   bool destroy_memory(int & m_shmid, char* & m_memory_addr);

   // attach shared memory function
   bool attach_memory(const int & m_key, int & m_shmid, const int & m_flag, char*& m_memory_addr);

   // detach shared memory function
   bool detach_memory(const int & m_shmid, char*& m_memory_addr);

   virtual int get_key(){};

   virtual int get_size(){};

   virtual int get_flag(){};

   // return the shmid
   virtual int get_shmid(){};

   // get a char pointer point to the first address of shared memory
   virtual char * get_memory_addr(){};

   // set info accordding to config
   virtual int set_config_info(char file_path[256]){};

   // use as a reader read a data from
   virtual int init_as_reader(){};


   virtual int init_as_writer(){};

   // write a datum to shared memory
   virtual int write_mem(Frame &frame){};

   // read a datum from shared memory
   virtual int read_mem(Frame &frame){};


 private:


};

#endif // MEMENGINE_H_
