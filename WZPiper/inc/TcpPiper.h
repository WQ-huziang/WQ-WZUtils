#ifndef TCPPIPER_H_
#define TCPPIPER_H_

#include "WZPiper.h"
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h> /* epoll function */
#include <fcntl.h>     /* nonblocking */
#include <unistd.h>

#define MAXEPOLLSIZE 10000
#define MAXLISTENQUEUR 1024

#define PRINTF(msg){printf("%s\n",msg);}
#define PRINTINT(msg){printf("%d\n", msg);}

class TcpPiper : public WZPiper {
 public:

   TcpPiper();

   virtual void init_as_server();
   virtual void set_config_info(char file_path[256]);
   virtual void init_as_client();
   
   virtual Frame do_read();
   virtual void do_write(Frame mail);

   virtual int wait_event();
   virtual bool handle_accept();

   virtual void close_link();

   virtual void set_event_fd(int sockfd);
   virtual int get_event_fd(int pos);
   virtual int get_listen_fd();

 public:
   int set_nonblocking(int sockfd);

   void add_event(int epollfd, int sockfd, int state);

   void delete_event(int epollfd, int sockfd, int state);

   void modify_event(int epollfd, int sockfd, int state);

 private:

   char ip[256];
   int port;

   int listen_fd;
   int epoll_fd;
   int event_fd;

   struct sockaddr_in servaddr;

   struct epoll_event events[MAXEPOLLSIZE];

   int cur_fd;
};

#endif