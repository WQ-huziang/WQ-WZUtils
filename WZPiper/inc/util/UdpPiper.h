#ifndef UDPPIPER_H_
#define UDPPIPER_H_

#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "wzadapter/frame.h"
#include "util/WZPiper.h"



class UdpPiper : public WZPiper {
 public:
   UdpPiper();
   ~UdpPiper();
   // initialize the server
   virtual int init_as_server();
   // set the ip and port according to the config file
   virtual int set_config_info(char file_path[256]);
   // link to the 
   virtual int init_as_client();
   // read data from the pipe
   virtual int do_read(Frame& mail);
   // write data to the pipe
   virtual int do_write(Frame& mail);


 private:
   // sokect ip address
   char UDP_ip [128];
   // sokect conect port 
   int UDP_port;
   // sokect id
   int UDP_sockfd;
   // buffer size
   char buffer[256];

   struct sockaddr_in addr;
};

#endif //UdpPiper
