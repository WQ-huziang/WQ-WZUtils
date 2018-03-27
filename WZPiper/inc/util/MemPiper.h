#ifndef MEMPIPER_H_
#define MEMPIPER_H_

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <unistd.h>
#include "util/WZPiper.h"
#include "MemType.h"

#define PRT(...) printf(__VA_ARGS__);

#ifndef SHM_FAILED 
#define SHM_FAILED -1  
#endif // SHM_FAILED

#ifndef SHM_DATA_SIZE 
#define SHM_DATA_SIZE 2048
#endif // SHM_DATA_SIZE

#ifndef SHM_FLAG
#define SHM_FLAG IPC_CREAT|0666
//#define SHM_FLAG IPC_CREAT|IPC_EXCL|SHM_R|SHM_W
#endif // SHM_FLAG

class MemPiper : public WZPiper {

 public:
   
   // from WZPiper
   virtual int init_as_server();
   virtual int set_config_info(char file_path[256]);
   virtual int init_as_client();
   virtual int do_read(Frame &mail);
   virtual int do_write(Frame &mail);

 public:
   
   // build-in function
   MemPiper();
   ~MemPiper();

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

   int m_block_num; // the number of SharedMemBlock
   
};

#endif // MEMPIPER_H_