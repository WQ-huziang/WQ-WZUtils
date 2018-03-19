#ifndef WZPIPER_H_
#define WZPIPER_H_

#include "frame.h"
#include "Ini.h"

class WZPiper {
 public:
   virtual void init_as_server() {}
   virtual void set_config_info(char file_path[256]) {}
   virtual void init_as_client() {}
   virtual Frame do_read() {}
   virtual void do_write(Frame mail) {}
   virtual void handle_event() {}
   virtual void handle_accept() {}
 private:
   
};


#endif
