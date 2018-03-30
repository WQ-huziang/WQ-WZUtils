#include "util/TcpPiper.h"
#include "util/logger.h"
extern Logger *logger;


TcpPiper::TcpPiper() 
{
    memset(&ip, 0, sizeof(ip));
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&events, 0, sizeof(events));
    port = 0;
    epollfd = -1;
    sockfd = -1;
    is_server = 0;
}

int TcpPiper::socketInit(bool sockType)
{
    is_server = sockType;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    epollfd = epoll_create(MAXEPOLLSIZE);
    return sockfd;
}

int TcpPiper::socketConnect()
{
  if(is_server == 1)
  {
      int opt = 1;
      /* port reuse */ 
      setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
      /* non blocking */
      if(setNonblocking(sockfd) != 0) 
          return -1;

      memset(&servaddr, 0, sizeof(servaddr));
      servaddr.sin_family = AF_INET;
      inet_pton(AF_INET, ip, &servaddr.sin_addr);
      servaddr.sin_port = htons (port);

      addEvent(sockfd, EPOLLIN);

      if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(struct sockaddr)) == -1) 
      {
          char buffer[256];
          sprintf(buffer, "bind error");
          logger->Error(buffer);
          return -1;
      }
      if(listen(sockfd, MAXLISTENQUEUR) == -1) 
      {
          char buffer[256];
          sprintf(buffer, "listen error");
          logger -> Error(buffer);
          return -1;
      }
  }
  else 
  {
      if(setNonblocking(sockfd) != 0) 
        return -1;

      memset(&servaddr, 0, sizeof(servaddr));
      servaddr.sin_family = AF_INET; 
      inet_pton(AF_INET, ip, &servaddr.sin_addr);
      servaddr.sin_port = htons (port);

      connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
      addEvent(sockfd, EPOLLIN);
  }
  return 0;
}

int TcpPiper::parseConfigFile(char file_path[256]) 
{
    CIni ini;
    if (ini.OpenFile(file_path, "r") == INI_OPENFILE_ERROR)
      return -1;
    char *temp = ini.GetStr("TCPNetInfo", "ip");
    strcpy(ip, temp);
    port = ini.GetInt("TCPNetInfo", "port");
    char buffer[256];
    sprintf(buffer, "config port = %d, config ip = %s", this->port, this->ip);
    logger -> Info(buffer);
    return 0;
}

char* TcpPiper::getInputStream(int len) 
{
	char* data;
	int num_of_events = epoll_wait(epollfd, events, 1, 0);
	for(int i=0; i<num_of_events; i++) 
	{
		int eventfd = events[i].data.fd;
		if (handleAccept(eventfd) == 0) 
			continue;
		data = new char[len];
		int ret = read(eventfd, data, len);
		if (ret >= 0)
		{
			return data;
		}
	}
	return NULL;
}

int TcpPiper::handleAccept(int eventfd) 
{
  if (eventfd == sockfd) 
  {
    struct sockaddr_in cliaddr;
    socklen_t socklen = sizeof(struct sockaddr_in);
    int cli_fd = accept(sockfd, (struct sockaddr *)&cliaddr, &socklen);
    setNonblocking(cli_fd);
    addEvent(cli_fd, EPOLLIN);
    char buffer[256];
    sprintf(buffer, "build a new link from: %s:%d", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
    logger -> Info(buffer);
    return 0;
  }
  return -1;
}

int TcpPiper::getOutputStream(void *data, int len) 
{
    Frame f;
    int ret;
    if(is_server == 0)
    {
        ret = write(sockfd, data, sizeof(Frame));
        return 0;
    }
    else 
    {
        memcpy(&f, data, len);
        cout << "f.dest " << f.dest << endl;
        ret = write(f.dest, data, len);
        return 0;
    }
    if(ret == -1)
    {
        char buffer[256];
        sprintf(buffer, "write error");
        logger -> Error(buffer);
      	close(f.dest);
    }
    return -1;
}

int TcpPiper::setNonblocking(int sockfd) 
{
    if(fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) 
    {
      char buffer[256];
      sprintf(buffer, "set nonblock error");
      logger -> Error(buffer);
      return -1;
    }
    return 0;
}

int TcpPiper::addEvent(int& sockfd, int state) 
{
    struct epoll_event ev;
    ev.events = state | EPOLLET;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1){
      char buffer[256];
      sprintf(buffer, "add event error");
      logger -> Error(buffer);
      return -1;
    }
    return 0;
}

int TcpPiper::deleteEvent(int& sockfd, int state) 
{
    struct epoll_event ev;
    ev.events = state | EPOLLET;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, &ev) == -1){
      char buffer[256];
      sprintf(buffer, "delete event error");
      logger -> Error(buffer);
      return -1;
    }
    return 0;
}