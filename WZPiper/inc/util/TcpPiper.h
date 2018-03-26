#ifndef TCPPIPER_H_
#define TCPPIPER_H_

#include "util/WZPiper.h"
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h> 
#include <fcntl.h>     
#include <unistd.h>

#define MAXEPOLLSIZE 10000
#define MAXLISTENQUEUR 1024

class TcpPiper : public WZPiper {
 public:

   /*
   from WZPiper
   */
   virtual int init_as_server();
   virtual int set_config_info(char file_path[256]);
   virtual int init_as_client();
   virtual int do_read(Frame &mail);
   virtual int do_write(Frame &mail);

 public:

   /*
   build-in function
   */
   TcpPiper();
   ~TcpPiper();

   /*set socket nonblocking*/
   int set_nonblocking(int sockfd);

   /*action in epoll*/
   bool handle_accept();
   int add_event(int &sockfd, int state);
   int delete_event(int &sockfd, int state);

 private:

   /*link information*/
   char ip[256];
   int port;

   /*epoll socket*/
   int listen_fd;
   int server_fd;
   int epoll_fd;
   int event_fd;

   struct sockaddr_in servaddr;
   struct epoll_event events[MAXEPOLLSIZE];

   bool is_server;

   char buffer[256];
};

#endif