#include "TcpPiper.h"

TcpPiper::TcpPiper() {
  memset(&ip, 0, sizeof(ip));
  port = 0;;

  listen_fd = 0;
  epoll_fd = 0;
  event_fd = 0;

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&events, 0, sizeof(events));

  cur_fd = 1;
}

void TcpPiper::init_as_server() {
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  epoll_fd = epoll_create(MAXEPOLLSIZE);

  int opt = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, 
  			 &opt, sizeof(opt));
  
  set_nonblocking(listen_fd);
  
  add_event(epoll_fd, listen_fd, EPOLLIN);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET; 
  inet_pton(AF_INET, ip, &servaddr.sin_addr);
  servaddr.sin_port = htons (port);

  if (bind(listen_fd, (struct sockaddr *) &servaddr, 
  					   sizeof(struct sockaddr)) == -1) {
  	PRINTF("bind error");
  	return;
  }

  if (listen(listen_fd, MAXLISTENQUEUR) == -1) {
  	PRINTF("listen error");
  	return;
  }
}

void TcpPiper::set_config_info(char file_path[256]) {
  CIni ini;
  ini.OpenFile(file_path, "r");

  char *temp = ini.GetStr("NetInfo", "ip");
  strcpy(ip, temp);
  port = ini.GetInt("NetInfo", "port");
}

void TcpPiper::init_as_client() {
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  set_nonblocking(listen_fd);

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET; 
  inet_pton(AF_INET, ip, &servaddr.sin_addr);
  servaddr.sin_port = htons (port);
  connect(listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));

  epoll_fd = epoll_create(MAXEPOLLSIZE);
  add_event(epoll_fd, listen_fd, EPOLLIN);
}

Frame TcpPiper::do_read() {
  Frame order;
  memset(&order, 0, sizeof(order));

  char buf[1024];
  memset(&buf, 0, sizeof(buf));

  int ret = read(event_fd, &buf, sizeof(buf));
  if (ret == -1) {
  	PRINTF("read error!");
  	close(event_fd);
  	delete_event(epoll_fd, event_fd, EPOLLIN);
  }
  else if (ret == 0) {
  	PRINTF("link close!");
  	close(event_fd);
  	delete_event(epoll_fd, event_fd, EPOLLIN);
  }
  else {
  	memcpy(&order, buf, sizeof(Frame));
  }

  PRINTINT(order.source);
  PRINTINT(order.msg_type);

  return order;
}

void TcpPiper::do_write(Frame mail) {
  int ret = write(event_fd, (char*)&mail, sizeof(Frame));
  PRINTINT(ret);
  if (ret == -1) {
  	PRINTF("write error!");
  	close(event_fd);
  	delete_event(epoll_fd, event_fd, EPOLLOUT);
  }
}

int TcpPiper::wait_event() {
  return  epoll_wait(epoll_fd, events, cur_fd, 0);
}

bool TcpPiper::handle_accept() {
  if (event_fd == listen_fd) {
  	struct sockaddr_in cliaddr;
    socklen_t socklen = sizeof(struct sockaddr_in);
    int cli_fd = accept(listen_fd, (struct sockaddr *)&cliaddr, &socklen);
    //set_nonblocking(cli_fd);
    
    add_event(epoll_fd, cli_fd, EPOLLIN);

    printf("build a new link from: %s:%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);

    return 1;
  }
  return 0;
}

void TcpPiper::close_link() {
  close(listen_fd);
  close(epoll_fd);
}

void TcpPiper::set_event_fd(int sockfd) {
  event_fd = sockfd;
}

int TcpPiper::get_event_fd(int pos) {
  return events[pos].data.fd;
}

int TcpPiper::get_listen_fd() {
  return listen_fd;
}



/*************/

int TcpPiper::set_nonblocking(int sockfd) {
  if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) {
    PRINTF("listen error");
    return -1;
  }
  return 0;
}

void TcpPiper::add_event(int epollfd, int sockfd, int state) {
  struct epoll_event ev;
  //ev.events = EPOLLIN | EPOLLET;
  ev.events = state | EPOLLET;
  ev.data.fd = sockfd;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
}

void TcpPiper::delete_event(int epollfd, int sockfd, int state) {
  struct epoll_event ev;
  ev.events = state | EPOLLET;
  ev.data.fd = sockfd;
  epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, &ev);
}

void TcpPiper::modify_event(int epollfd, int sockfd, int state) {
  struct epoll_event ev;
  ev.events = state | EPOLLET;
  ev.data.fd = sockfd;
  epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
}
