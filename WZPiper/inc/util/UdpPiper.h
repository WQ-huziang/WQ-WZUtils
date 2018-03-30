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
   virtual ~UdpPiper();
   // initialize the server
   virtual int parseConfigFile(char file_path[256]);
   /* Session management */
   virtual int socketInit(bool sockType);
   virtual int socketConnect();
   /* Data management */
   virtual char* getInputStream(int len);
   virtual int getOutputStream(void *, int len);

 private:
   // sokect ip address
   char ip[256];
   // sokect conect port 
   int port;
   // sokect id
   int sockfd;
   // buffer size

   struct sockaddr_in addr;

   bool is_server;
};

#endif //UdpPiper
