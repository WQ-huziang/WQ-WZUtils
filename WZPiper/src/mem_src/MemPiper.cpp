#include "util/MemPiper.h"
#include "util/logger.h"

extern Logger *logger;
char logger_buf[1024];

// initialize some private variable
MemPiper::MemPiper() {
   // init private variable
   m_flag = 3 ;
   m_key  = 0 ;
   m_size = 0 ;
   m_flag = SHM_FLAG ;
   m_shmid   = -1;
   m_memory_addr = NULL;
}

// destructor
MemPiper::~MemPiper() {
   // call detach_memory to detach memory
   if(m_memory_addr != NULL) {
      detach_memory(m_memory_addr);
   }

}

// initialize the server shared memory address pointer
int MemPiper::init_as_server() {
   if ( create_memory(this -> m_key, this -> m_size, this -> m_flag) ) {
      sprintf(logger_buf, "create_memory successfully");
      logger -> Info(logger_buf);
      
      return 0;
   }
   return -1;
}

// read key and size from configure file 
int MemPiper::set_config_info(char file_path[256]) {
   CIni ini;
   if (ini.OpenFile(file_path, "r") == INI_OPENFILE_ERROR){
      sprintf(logger_buf, "INI_OPENFILE_ERROR");
      logger -> Info(logger_buf);
      return -1;
   }

   this -> m_key = ini.GetInt("MemInfo","key");
   
   // this -> m_size = ini.GetInt("MemInfo","size");

   this -> m_block_num = ini.GetInt("MemInfo", "blocknum");

   this -> m_size = m_block_num * sizeof(SharedMemBlock);

   sprintf(logger_buf, "MemInfo key = %d, MemInfo size = %d, MemInfo blocknum = %d\n", this->m_key, this->m_size, this->m_block_num);
   logger -> Info(logger_buf);

   return 0;
}
// initialize the client shared memory address pointer
int MemPiper::init_as_client() {
   if( create_memory(this -> m_key, this -> m_size, this -> m_flag) ) {
      sprintf(logger_buf, "create_memory successfully");
      logger -> Info(logger_buf);
      return 0;
   }
   return -1;
}
// read from the shared memory
int MemPiper::do_read(Frame &mail) {
   SharedMemBlock* blk_cur;
   blk_cur = (SharedMemBlock*) this -> m_memory_addr;
   for (int i = 0; i < this -> m_block_num; i++) {
      if( (* (blk_cur + i)).written == 0 ){
         (* (blk_cur + i)).written = 1;
         memcpy((* (blk_cur + i)).data, &mail, sizeof(Frame));
         return 0;
      } 
   }
   // memcpy(&mail, (m_memory_addr), sizeof(mail));
   return -1;
}
// write from the shared memory
int MemPiper::do_write(Frame &mail) {
   SharedMemBlock* blk_cur;
   blk_cur = (SharedMemBlock*) this -> m_memory_addr;
   for (int i = 0; i < this -> m_block_num ; i++) {
      if( (* (blk_cur + i)).written == 1 ){
         (* (blk_cur + i)).written = 0;
         memcpy(&mail, (* (blk_cur + i)).data, sizeof(Frame));
         return 0;
      } 
   }
   // memcpy(m_memory_addr, &mail, sizeof(mail));
   return -1;
}

// create shared memory function
bool MemPiper::create_memory(int m_key, int m_size, int m_flag) {
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
bool MemPiper::destroy_memory(int shmid) {

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
bool MemPiper::attach_memory(char*& pmemoryAddr, int m_flag) {
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
bool MemPiper::detach_memory(char* pmemoryAddr) {
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

// get private m_shmid 
int MemPiper::get_shmid(){
   return m_shmid;
}