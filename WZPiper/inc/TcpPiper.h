#ifndef TCPPIPER_H_
#define TCPPIPER_H_

#include "WZPiper.h"

class TcpPiper : public WZPiper {
 public:
   TcpPiper();
   virtual void init();
   virtual void connect();
   virtual void do_read();
   virtual void do_write();

 private:
   char local_ip[256];
}

#endif