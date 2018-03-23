#include "MemPiper.h"

extern Logger *logger;

// initialize some private variable
MemPiper::MemPiper() {
   // init private variable
   m_flag = 3 ;
   m_key  = 0 ;
   m_size = 0 ;
   m_flag = FLAG ;
   m_shmid   = -1;
   m_memoryAddr = NULL;
}

// destructor
MemPiper::~MemPiper() {
   // call detachMemory to detach memory
   if(m_memoryAddr != NULL) {
      detachMemory(m_memoryAddr);
   }
}

// initialize the server shared memory address pointer
void MemPiper::init_as_server() {
   if ( createMemory() ) {
      PRT("createMemory successfully")
   }
}

// read key and size from configure file 
void MemPiper::set_config_info(char file_path[256]) {
   CIni ini;
   ini.OpenFile(file_path,"r");

   this->m_key = ini.GetInt("MemInfo","key");
   PRT("MemInfo key = %d",this->m_key);

   this->m_size = ini.GetInt("MemInfo","size");
   PRT("MemInfo size = %d",this->m_size);

}

void MemPiper::init_as_client() {

}

int MemPiper::do_read(Frame &mail) {

}

void MemPiper::do_write(Frame mail) {

}

// create shared memory function
bool MemPiper::createMemory() {
   // call shmget and use return value to initialize shared memory address pointer
   m_shmid = shmget(m_key, m_size, m_flag);

   if (m_shmid == -1) {  
      return false;
   }

   m_memoryAddr = reinterpret_cast<char*>(shmat(m_shmid, NULL, 0));

   if (m_memoryAddr == (char*)SHM_FAILED) {
      return false;
   }

   return true;
}

// destroy shared memory function
bool MemPiper::destroyMemory() {

   if (m_shmid == -1) {
      return false;
   }
   
   if (m_memoryAddr != NULL) {
      if (shmdt(m_memoryAddr) != 0) {

         return false;
      }
      m_memoryAddr = NULL;
   }

   if (shmctl(m_shmid, IPC_RMID, NULL) != 0) {
      return false;
   }
   
   m_shmid = -1;
   m_memoryAddr = NULL;
   return true;
}

// attach shared memory function
bool MemPiper::attachMemory(char*& pmemoryAddr) {
   // init as null;
   pmemoryAddr = NULL;

   // is the id exist
   if (m_shmid == -1) {
      // is not exist the recreate it according to m_key
      m_shmid = shmget(m_key, 0, SHM_R|SHM_W);
   }

   // call shmat to attach and return address
   m_memoryAddr = reinterpret_cast<char*> (shmat(m_shmid, NULL, 0));

   // is null
   if (m_memoryAddr == (char*) SHM_FAILED ) {
      return false;
   }

   pmemoryAddr = reinterpret_cast<char*> (m_memoryAddr);
   return true;
}
// detach shared memory function
bool MemPiper::detachMemory(char* pmemoryAddr) {
   // is private variable address valid
   if (m_shmid == -1 || m_memoryAddr == NULL) {
      // not valid
      return true;
   }

   // is the same address as private variable address
   if (pmemoryAddr != (m_memoryAddr)) {
      return false;
   }

   // call detach and return to nCode
   int nCode = shmdt(m_memoryAddr);

   if (nCode != 0) {
     if(errno == EINVAL) {
         m_memoryAddr = NULL;  
         PRT("*Already shmdt*");
         return true;  
     }
     return false;
   }

   m_memoryAddr = NULL;
   return true;
}