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
#include <map>

#include "frame.h"
#include "iniparser.h"
#include "wzpiper.hpp"

using namespace std;
using std::map;

#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

#define MAXEPOLLSIZE 10
#define MAXLISTENQUEUE 10

class TcpSocket
{
public:
	TcpSocket();
	~TcpSocket();

	int init(char file_path[256], int piperMode, int blockMode);
	int Recv(Frame &md);
	int Send(Frame &md);

private:
	int Bind();
	int Connect();
	int addEvent(int sockfd, int state);
	int sockAccept(int eventfd);

private:
	char ip[256];
	int port;

	int tcpfd;
	int epollfd;

	struct sockaddr_in addr;
	struct epoll_event events[MAXEPOLLSIZE];

	map<int, int> sock_map;

	bool isClient;
	bool isSocketBlocking;
	int isEpollBlocking;
};

#endif
