#ifndef WZPIPER_H_
#define WZPIPER_H_

#include "frame.h"
#include "Ini.h"

class WZPiper {
 public:
   virtual void init() {}
   virtual void set_config_info(char file_path[256]) {}
   virtual void link_to_server() {}
   virtual Frame do_read(int socket) {}
   virtual void do_write(int socket) {}
   virtual void handle_event() {}
   virtual void handle_accept() {}
 private:
   
};


#endif