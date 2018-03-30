#include "util/socketserver.h"
#include "util/logger.h"
extern Logger *logger;

SocketServer::SocketServer(bool sockType)
{
	// initialize
	port = 0;
	sockfd = -1;
	epollfd = -1;
	memset(&ip, 0, sizeof(ip));
	memset(&events, 0, sizeof(events));
	memset(&serveraddr, 0, sizeof(serveraddr));
	this->sockType = sockType;
}

SocketServer::~SocketServer()
{
	port = 0;
	sockfd = -1;
	epollfd = -1;
	sockType = 0;
	memset(&ip, 0, sizeof(ip));
	memset(&events, 0, sizeof(events));
	memset(&serveraddr, 0, sizeof(serveraddr));
}

int addEvent(int& sock, int epollfd, int state) 
{
	struct epoll_event ev;
	ev.events = state | EPOLLET;
	ev.data.fd = sock;
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev) == -1)
	{
		string buffer = "add event error";
		logger->Error(buffer);
		return -1;
	}
	return 0;
}

int deleteEvent(int& sock, int epollfd, int state) 
{
	struct epoll_event ev;
	ev.events = state | EPOLLET;
	ev.data.fd = sock;
	if(epoll_ctl(epollfd, EPOLL_CTL_DEL, sock, &ev) == -1)
	{
		string buffer = "delete event error";
		logger->Error(buffer);
		return -1;
	}
	return 0;
}

int setNonblocking(int sockfd)
{
    if(fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1)
    {
        return -1;
    }
    return 0;
}

int SocketServer::connect()
{
	if(sockType == 0)  		
	{
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	    if(sockfd == -1)
	    {
	        string buffer = "Server socket create error";
	        logger->Error(buffer);
	        return -1;
	    } 

	    bzero(&serveraddr,sizeof(serveraddr));
	    serveraddr.sin_family = AF_INET;
	    serveraddr.sin_port = htons(port);
	    serveraddr.sin_addr.s_addr = inet_addr(ip);

	    if(bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
	    {
	        string buffer = "UDP server bind error";
	        logger->Error(buffer);
	        return -1;
	    } 
	}
	else if(sockType == 1)  
	{
		sockfd = socket(AF_INET, SOCK_STREAM, 0);

		bzero(&serveraddr, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		inet_pton(AF_INET, ip, &serveraddr.sin_addr);
		serveraddr.sin_port = htons (port);
  		int opt = 1;
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		if(setNonblocking(sockfd) != 0) return -1;
		if(bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(struct sockaddr)) == -1) 
		{
			string buffer = "bind error";
			logger->Error(buffer);
			return -1;
		}
		if(listen(sockfd, MAXLISTENQUEUR) == -1) 
		{
			string buffer = "listen error";
			logger->Error(buffer);
			return -1;
		}

		epollfd = epoll_create(MAXEPOLLSIZE);
		addEvent(sockfd, epollfd, EPOLLIN);
	}
	return 0;
}

int SocketServer::readBuffer(Frame &f)
{
	if(sockType == 0)
	{
		socklen_t in_socklen;
    	int length = recvfrom(sockfd, &f, sizeof(Frame), 0, (struct sockaddr* ) &serveraddr, &in_socklen);

	    if(length < 0)
	    {
	        string buffer = "UDP read error";
	        logger->Error(buffer);
	        return -1;
	    } 
	    else
	    {
	        if(f.error_id != WZ_ERROR_ID_SUCCESS) 
	        {
	            string buffer = "receive wrong data";
	            logger->Warn(buffer);
	            return -1;
	        }
	    }
	}
	else if(sockType == 1)
	{
		int num_of_events = epoll_wait(epollfd, events, 1, 0);
		for(int i = 0; i < num_of_events; i++) 
		{
			int eventfd = events[i].data.fd;
			if(eventfd == sockfd) 
			{
				struct sockaddr_in cliaddr;
				socklen_t socklen = sizeof(struct sockaddr_in);
				int cli_fd = accept(sockfd, (struct sockaddr *)&cliaddr, &socklen);
				if(setNonblocking(cli_fd) != 0) return -1;
				addEvent(cli_fd, epollfd, EPOLLIN);
			}
			memset(&f, 0, sizeof(Frame));
			int ret = read(eventfd, &f, sizeof(f));
			if(ret < 0)
			{
				deleteEvent(eventfd, epollfd, EPOLLIN);
			}
			else
			{
				return 0;
			}
		}
	}
}

int SocketServer::writeBuffer(Frame &f)
{
	int ret;
	if(sockType == 0)
	{
	    ret = sendto(sockfd, (char* ) &f, sizeof(Frame), 0, (struct sockaddr*) &serveraddr, sizeof(serveraddr) );	
	}
	else if(sockType == 1)
	{
		ret = write(sockfd, (char*)&f, sizeof(Frame));
	}
	if(ret < 0) 
	{
		char *buffer = "write error";
		logger->Error(buffer);
		return -1;
	}
	return 0;
}
