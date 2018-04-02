#include "util/MemEngine.h"
#include "util/logger.h"
#include "util/iniparser.h"

#define MAX_QUEUE_SIZE 20

extern Logger *logger;
char logger_buf[1024];

// initialize some private variable
// MemEngine::MemEngine() {
//    // init private variable
//    m_flag = 3 ;
//    m_key  = 0 ;
//    m_size = 0 ;
//    m_flag = SHM_FLAG ;
//    m_shmid   = -1;
//    m_memory_addr = NULL;
// }

// // destructor
// MemEngine::~MemEngine() {
//    // call detach_memory to detach memory
//    if(m_memory_addr != NULL) {
//       detach_memory(m_memory_addr);
//    }
// }

// read key and size from configure file 
int MemEngine::set_config_info(char file_path[256]) {
   CIni ini;
   if (ini.OpenFile(file_path, "r") == INI_OPENFILE_ERROR){
      sprintf(logger_buf, "INI_OPENFILE_ERROR");
      logger -> Info(logger_buf);
      return -1;
   }

   this -> m_key = ini.GetInt("MemInfo","key");

   this -> m_size = ini.GetInt("MemInfo", "memorysize");

   sprintf(logger_buf, "MemInfo key = %d, MemInfo size = %d\n", this->m_key, this->m_size);
   logger -> Info(logger_buf);

   return 0;
}

// create shared memory function
bool MemEngine::create_memory(int m_key, int m_size, int m_flag) {
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

   return true;
}

// destroy shared memory function
bool MemEngine::destroy_memory(int shmid) {

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
   return true;
}

// attach shared memory function
bool MemEngine::attach_memory(char*& pmemoryAddr, int m_flag) {
   // init as null;
   pmemoryAddr = NULL;

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

   pmemoryAddr = reinterpret_cast<char*> (m_memory_addr);
   return true;
}

// detach shared memory function
bool MemEngine::detach_memory(char* pmemoryAddr) {
   // is private variable address valid
   if (m_shmid == -1 || m_memory_addr == NULL) {
      // not valid
      return true;
   }

   // is the same address as private variable address
   if (pmemoryAddr != (m_memory_addr)) {
      sprintf(logger_buf, "is not the same address as private variable address");
      logger -> Error(logger_buf);
      return false;
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

int MemEngine::get_key(){
   return m_key;
}

int MemEngine::get_size(){
   return m_size;
}

int MemEngine::get_flag(){
   return m_flag;
}

// get private m_shmid 
int MemEngine::get_shmid(){
   return m_shmid;
}

char * MemEngine::get_memory_addr(){
   return m_memory_addr;
}