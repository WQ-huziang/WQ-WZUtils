#ifndef MEMENGINE_H_
#define MEMENGINE_H_

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <unistd.h>

#define PRT(...) printf(__VA_ARGS__);
#define SHM_FAILED -1 
#define SHM_DATA_SIZE 2048
#define SHM_FLAG IPC_CREAT|0666

class MemEngine{
 
 public:
   
   // build-in function
   MemEngine();
   ~MemEngine();

   // set info accordding to config
   int set_config_info(char file_path[256]);

   // create shared memory function
   bool create_memory(int m_key, int m_size, int m_flag);

   // destroy shared memory function
   bool destroy_memory(int shmid);

   // attach shared memory function
   bool attach_memory(char*& pmemoryAddr, int m_flag);

   // detach shared memory function
   bool detach_memory(char* pmemoryAddr);

   // return the shmid
   int get_shmid();


 private:

   // shared memory information
   int m_key;			// shared memory key
   int m_size;			// shared memory size
   int m_flag;			// shared memory flag
   int m_shmid;			// shared memory descriptor
   char *m_memory_addr;	// shared memory address pointer  

};

#endif // MEMENGINE_H_
