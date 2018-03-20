#include "TcpPiper.h"

TcpPiper::TcpPiper() 
{
  memset(&ip, 0, sizeof(ip));
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&events, 0, sizeof(events));

  port = 0;;
  listen_fd = -1;
  server_fd = -1;
  epoll_fd = -1;
  event_fd = -1;

  is_server = 0;
}

TcpPiper::~TcpPiper() 
{
  close(listen_fd);
  close(server_fd);
  close(epoll_fd);
}

void TcpPiper::init_as_server() 
{
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  epoll_fd = epoll_create(MAXEPOLLSIZE);
  is_server = 1;

  int opt = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  set_nonblocking(listen_fd);
  add_event(epoll_fd, listen_fd, EPOLLIN);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET; 
  inet_pton(AF_INET, ip, &servaddr.sin_addr);
  servaddr.sin_port = htons (port);

  if (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(struct sockaddr)) == -1) 
  {
  	PRINTF("bind error");
  	return;
  }
  if (listen(listen_fd, MAXLISTENQUEUR) == -1) 
  {
  	PRINTF("listen error");
  	return;
  }
}

void TcpPiper::set_config_info(char file_path[256]) 
{
  CIni ini;
  ini.OpenFile(file_path, "r");
  char *temp = ini.GetStr("NetInfo", "ip");
  strcpy(ip, temp);
  port = ini.GetInt("NetInfo", "port");
}

void TcpPiper::init_as_client() 
{
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  set_nonblocking(server_fd);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET; 
  inet_pton(AF_INET, ip, &servaddr.sin_addr);
  servaddr.sin_port = htons (port);
  connect(server_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));

  epoll_fd = epoll_create(MAXEPOLLSIZE);
  add_event(epoll_fd, server_fd, EPOLLIN);
}

int TcpPiper::do_read(Frame &mail) 
{
  int num_of_events = wait_event();
  for(int i=0; i<num_of_events; i++) 
  {
    event_fd = events[i].data.fd;
    if (handle_accept()) 
      continue;
    memset(&mail, 0, sizeof(Frame));
    read(event_fd, &mail, sizeof(mail));
    return 1;
  }
  return 0;
}

void TcpPiper::do_write(Frame mail) 
{
  int ret;
  if (is_server == 0)
  {
    ret = write(server_fd, (char*)&mail, sizeof(Frame));
  }
  else 
  {
    ret = write(mail.dest, (char*)&mail, sizeof(Frame));
  }
  PRINTINT(ret);
  if (ret == -1) 
  {
  	PRINTF("write error!");
  	close(event_fd);
  	delete_event(epoll_fd, event_fd, EPOLLOUT);
  }
}

int TcpPiper::set_nonblocking(int sockfd) 
{
  if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) 
  {
    PRINTF("listen error");
    return -1;
  }
  return 0;
}

int TcpPiper::wait_event() 
{
  return  epoll_wait(epoll_fd, events, 1, 0);
}

bool TcpPiper::handle_accept() 
{
  if (event_fd == listen_fd) 
  {
    struct sockaddr_in cliaddr;
    socklen_t socklen = sizeof(struct sockaddr_in);
    int cli_fd = accept(listen_fd, (struct sockaddr *)&cliaddr, &socklen);
    set_nonblocking(cli_fd);
    add_event(epoll_fd, cli_fd, EPOLLIN);
    printf("build a new link from: %s:%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
    return 1;
  }
  return 0;
}

void TcpPiper::add_event(int epollfd, int sockfd, int state) 
{
  struct epoll_event ev;
  ev.events = state | EPOLLET;
  ev.data.fd = sockfd;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
}

void TcpPiper::delete_event(int epollfd, int sockfd, int state) 
{
  struct epoll_event ev;
  ev.events = state | EPOLLET;
  ev.data.fd = sockfd;
  epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, &ev);
}

