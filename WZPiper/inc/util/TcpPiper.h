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
    TcpPiper();
 	/* System configuration */
	virtual int parseConfigFile(char file_path[256]);
	/* Session management */
	virtual int socketInit(bool sockType);
	virtual int socketConnect();
	/* Data management */
	virtual char* getInputStream(int len);
	virtual int getOutputStream(void *, int len);
	/* Event management */
	int addEvent(int &sockfd, int state);
	int deleteEvent(int &sockfd, int state);
    int setNonblocking(int sockfd);
    int handleAccept(int);

 private:
	bool is_server;
   /*link information*/
   char ip[256];
   int port;
   /*epoll socket*/
   int sockfd;
   int epollfd;
   struct sockaddr_in servaddr;
   struct epoll_event events[MAXEPOLLSIZE];
};

#endif
