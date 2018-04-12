//
// Author: luojunbin
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "frame.h"
#include "iniparser.h"
#include "wzpiper.hpp"

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

#define MAXEPOLLSIZE 10
#define MAXLISTENQUEUE 10

class UdpSocket
{
public:
	UdpSocket();
	~UdpSocket();

	int init(char file_path[256], int piperMode, int blockMode);
	int Recv(Frame &md);
	int Send(Frame &md);

private:
	int Bind();
	int Connect();
	int addEvent(int sockfd, int state);

private:
	char ip[256];
	int port;

	int udpfd;
	int epollfd;

	bool isClient;
	bool isSocketBlocking;
	int isEpollBlocking;

	struct sockaddr_in addr;
	struct epoll_event events[MAXEPOLLSIZE];
};

#endif
