//
// Author: luojunbin
//

#include "wzsocket_inc/udp.h"

UdpSocket::UdpSocket()
{
    strcpy(ip, "127.0.0.1");
    port = 0;

    udpfd = 0;
    epollfd = 0;

    isSocketBlocking = 1;
    isClient = 0;
    isEpollBlocking = -1;

    memset(&addr, 0, sizeof(addr));
    memset(events, 0, sizeof(events));
}

UdpSocket::~UdpSocket()
{
    close(udpfd);
    close(epollfd);
}

int UdpSocket::init(char file_path[256], int piperMode, int blockMode)
{
    CIni ini;
    if (ini.OpenFile(file_path, "r") == INI_OPENFILE_ERROR)
        return -1;
    char *temp = ini.GetStr("UDPNetInfo", "ip");
    strcpy(ip, temp);
    port = ini.GetInt("UDPNetInfo", "port");

    isClient = piperMode;

    // create fd
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);

    // set non_blocking
    if(blockMode == WZ_PIPER_BLOCK){
        isSocketBlocking = 1;
        isEpollBlocking = -1;
        if (fcntl(udpfd, F_SETFL, 0) == -1)
            return -1;
    }
    else{
        isSocketBlocking = 0;
        isEpollBlocking = 0;
        if (fcntl(udpfd, F_SETFL, fcntl(udpfd, F_GETFD, 0)|O_NONBLOCK) == -1)
            return -1;
    }

    // set addr
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr.sin_addr);
    addr.sin_port = htons (port);

    if (!isClient)
    {
        epollfd = epoll_create(MAXEPOLLSIZE);
        if (epollfd < 0)
            return -1;
        addEvent(udpfd, EPOLLIN);
        if (Bind() == -1)
            return -1;
    }
    else if (isClient)
    {
        if (Connect() == -1)
            return -1;
    }
    // LOG(INFO) << "Udp Socket Init Success, IP = " << ip
    //           << " PORT = " << port << " isClient = " << isClient;
    return udpfd;
}

int UdpSocket::Recv(Frame &md)
{
    int num_of_events = epoll_wait(epollfd, events, 1, isEpollBlocking);
    for (int i=0; i<num_of_events; i++)
    {
        int eventfd = events[i].data.fd;
        if (eventfd == udpfd)
        {
            struct sockaddr_in cliaddr;
            socklen_t socklen = sizeof(struct sockaddr_in);
            int ret = recvfrom(udpfd, &md, sizeof(Frame), 0, (struct sockaddr* )&cliaddr, &socklen);
            // LOG(INFO) << "Receive a frame from SOURCE: "<<md.source;
            return ret;
        }
    }
    return -1;
}

int UdpSocket::Send(Frame &md)
{
    if (write(udpfd, (char*)&md, sizeof(Frame)) == -1)
        return -1;
    return 0;
}

int UdpSocket::Bind()
{
    if (bind(udpfd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1)
        return -1;
    return 0;
}

int UdpSocket::Connect()
{
    connect(udpfd, (struct sockaddr *)&addr, sizeof(addr));
    return 0;
}

int UdpSocket::addEvent(int sockfd, int state)
{
    struct epoll_event ev;
    ev.events = state | EPOLLET;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1)
    {
        return -1;
    }
    return 0;
}
