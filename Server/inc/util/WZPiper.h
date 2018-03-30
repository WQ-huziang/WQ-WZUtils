//
// Author: Alva
// Functions: Interprocess communication:Pipe signal message shared_memory socket
#ifndef WZPIPER_H_
#define WZPIPER_H_

#include "wzadapter/frame.h"
#include "util/iniparser.h"

class WZPiper {

 public:

   // function: create server or client 
   // return: if create successfully return sockid, or return -1
   WZPiper(){};
   virtual int parseConfigFile(char file_path[256]){};
   /* Session management */
   virtual int socketInit(bool sockType){};
   virtual int socketConnect(){};
   /* Data management */
   virtual char* getInputStream(int len){};
   virtual int getOutputStream(void *, int len){};
};


#endif
