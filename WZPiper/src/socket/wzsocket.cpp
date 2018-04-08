//
// Author: luojunbin

#include "util/wzsocket.h"

int WZSocket::epollInit()
{
	epollfd = epoll_create(MAXEPOLLSIZE);
	if (epollfd < 0)
		return -1;
	addEvent(tcpfd, EPOLLIN);
	addEvent(udpfd, EPOLLIN);
	cout << udpfd << " " << epollfd << endl;
	return 0;
}

int WZSocket::wzRecv(Frame &md)
{
	int num_of_events = epoll_wait(epollfd, events, 1, -1);
	for (int i=0; i<num_of_events; i++) 
	{
		int eventfd = events[i].data.fd;
		cout << eventfd << endl;
		if (sockAccept(eventfd) == 0)
			continue;
		else if (eventfd == udpfd)
		{
			struct sockaddr_in cliaddr;
			socklen_t socklen = sizeof(struct sockaddr_in);
			int ret = recvfrom(udpfd, &md, sizeof(Frame), 0, (struct sockaddr* )&cliaddr, &socklen);
			return ret;
		}
		else
		{
			int ret = read(eventfd, &md, sizeof(Frame));
			return ret;
		}
	}
	return -1;
}

int WZSocket::wzSend(Frame &md)
{
	if (write(md.dest, (char*)&md, sizeof(Frame)) == -1)
		return -1;
	return 0;
}

int WZSocket::addEvent(int sockfd, int state)
{
	struct epoll_event ev;
	ev.events = state | EPOLLET;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1){
    	return -1;
    }
    return 0;
}

int WZSocket::sockAccept(int eventfd)
{
	if (eventfd != tcpfd)
		return -1;
	else
	{
		struct sockaddr_in cliaddr;
		socklen_t socklen = sizeof(struct sockaddr_in);
		
		// accept new link
		int clifd = accept(tcpfd, (struct sockaddr*)&cliaddr, &socklen);

		// set non_blocking
		if (fcntl(clifd, F_SETFL, fcntl(clifd, F_GETFD, 0)|O_NONBLOCK) == -1)
			return -1;

		// add event
		if (addEvent(clifd, EPOLLIN) == -1)
			return -1;

		printf("build a new link form: %s: %d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
	}
	return 0;	
}