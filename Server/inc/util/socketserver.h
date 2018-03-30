#ifndef SOCKETSERVER_H_
#define SOCKETSERVER_H_

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h> 
#include <fcntl.h>     
#include <unistd.h>
#include "server.h"
#include "socket.h"

#define MAXEPOLLSIZE 10000
#define MAXLISTENQUEUR 1024


class SocketServer : public Socket, public Server
{
public:
	SocketServer(bool sockType);
	~SocketServer();
	int connect();
	int readBuffer(Frame &);
	int writeBuffer(Frame &);
	void parseConfigFile(char configFilePath[256]);

private:
	bool sockType;
	char ip[256];
	int port;
	int sockfd, epollfd;
	struct sockaddr_in serveraddr;
	struct epoll_event events[MAXEPOLLSIZE];
};
#endif
