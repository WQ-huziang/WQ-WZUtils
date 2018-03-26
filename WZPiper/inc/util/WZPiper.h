#ifndef WZPIPER_H_
#define WZPIPER_H_

#include "wzadapter/frame.h"
#include "util/iniparser.h"

// #define PRINTSTR(str) printf("%s\n", str);
// #define PRINTINT(num) printf("%d\n", num);

#define PRINTSTR(str)
#define PRINTINT(num)

class WZPiper {

 public:

   /*init as server or client to build connect*/
   virtual int init_as_server() {}
   virtual int init_as_client() {}

   /*set ip and port*/
   /* read a config file include ip and port*/
   virtual int set_config_info(char file_path[256]) {}

   /*recieve and send request*/
   /*recieve the data to Frame mail*/
   virtual int do_read(Frame &mail) {}
   /*send Frame mail to remote*/
   virtual int do_write(Frame &mail) {}

 private:
   
};


#endif
