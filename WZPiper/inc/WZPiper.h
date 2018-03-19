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

   virtual int wait_event() {}
   virtual bool handle_accept() {}

   virtual void close_link() {}

   virtual void set_event_fd(int sockfd) {} 
   virtual int get_event_fd(int pos) {}
   virtual int get_listen_fd() {}

 private:
   
};


#endif
