//
// Author: luojunbin
//

#include "wzsocket_inc/tcp.h"

TcpSocket::TcpSocket()
{
    strcpy(ip, "127.0.0.1");
    port = 0;

    tcpfd = 0;
    epollfd = 0;

    memset(&addr, 0, sizeof(addr));
    memset(events, 0, sizeof(events));

    isClient = 0;
    isSocketBlocking = 1;
    isEpollBlocking = -1;
}

TcpSocket::~TcpSocket()
{
    close(tcpfd);
    close(epollfd);
}

int TcpSocket::init(char file_path[256], int piperMode, int blockMode)
{
    CIni ini;
    if (ini.OpenFile(file_path, "r") == INI_OPENFILE_ERROR)
        return -1;
    char *temp = ini.GetStr("TCPNetInfo", "ip");
    strcpy(ip, temp);
    port = ini.GetInt("TCPNetInfo", "port");

    // create fd
    tcpfd = socket(AF_INET, SOCK_STREAM, 0);

    // set non_blocking
    if(blockMode == WZ_PIPER_BLOCK){
        isSocketBlocking = 1;
        isEpollBlocking = -1;
        if (fcntl(tcpfd, F_SETFL, 0) == -1)
            return -1;
    }
    else{
        isSocketBlocking = 0;
        isEpollBlocking = 0;
        if (fcntl(tcpfd, F_SETFL, fcntl(tcpfd, F_GETFD, 0)|O_NONBLOCK) == -1)
            return -1;
    }

    // set addr
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr.sin_addr);
    addr.sin_port = htons (port);

    isClient = piperMode;

    if (!isClient)
    {
        // init epoll
        epollfd = epoll_create(MAXEPOLLSIZE);
        if (epollfd < 0)
            return -1;
        addEvent(tcpfd, EPOLLIN);
        if (Bind() == -1)
            return -1;
    }
    else if (isClient)
    {
        if (Connect() == -1)
            return -1;
    }
    else{
        return -1;
    }
    
    #ifdef LOGGER
    LOG(INFO) << "Tcp Socket Init Success, IP = " << ip
              << " PORT = " << port << " isClient = " << isClient;
    #endif
    
    return tcpfd;
}

int TcpSocket::Recv(Frame &md)
{
    if (isClient)
    {
        int ret = read(tcpfd, &md, sizeof(Frame));
        if (ret == 0)
        {
            #ifdef LOGGER
            LOG(ERROR) << "Server Close.";
            #endif
            // connection break;
        }
        return ret;
    }
    else
    {
        // 一次最多读一个
        int num_of_events = epoll_wait(epollfd, events, 1, isEpollBlocking);

        // 解除连接时应删除map中的fd待完成
        for (int i=0; i<num_of_events; i++)
        {
            int eventfd = events[i].data.fd;
            if (eventfd == tcpfd)
            {
                if(sockAccept() == 0)  continue;
            }
            else
            {
                int ret = read(eventfd, &md, sizeof(Frame));

                // insert into map
                sock_map.insert(pair<int, int> (md.source, eventfd));
                
                if (ret == 0)
                {
                    #ifdef LOGGER
                    LOG(ERROR) << "Client Close.";
                    #endif
                    deleteEvent(eventfd, EPOLLIN);
                    close(eventfd);
                    // connection break, delete fd;
                }
                return ret;
            }
        }
    }
    return -1;
}

int TcpSocket::Send(Frame &md)
{
    if (isClient)
    {
        if (write(tcpfd, (char*)&md, sizeof(Frame)) == -1)
            return -1;
    }
    else
    {
        map<int, int>::iterator key = sock_map.find(md.dest);
        int sockfd;
        if (key != sock_map.end())
        {
            sockfd = key->second;

            // #ifdef LOGGER
            // LOG(INFO) << "Send a frame to SOURCE: "<<md.dest << " Fd: "<<sockfd;
            // #endif
        }
        else
        {
            return -1;
        }
        if (write(sockfd, (char*)&md, sizeof(Frame)) == -1)
        {
            return -1;
        }
    }
    return 0;
}

int TcpSocket::Bind()
{
    if (bind(tcpfd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1)
        return -1;
    if (listen(tcpfd, MAXLISTENQUEUE) == -1)
        return -1;
    return 0;
}

int TcpSocket::Connect()
{
    connect(tcpfd, (struct sockaddr*)&addr, sizeof(addr));
    return 0;
}

int TcpSocket::addEvent(int sockfd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1)
    {
        return -1;
    }
    return 0;
}

int TcpSocket::deleteEvent(int sockfd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, &ev) == -1)
        return -1;
    return 0;
}

int TcpSocket::sockAccept()
{
    struct sockaddr_in cliaddr;
    socklen_t socklen = sizeof(struct sockaddr_in);

    // accept new link
    int clifd = accept(tcpfd, (struct sockaddr*)&cliaddr, &socklen);

    if(isSocketBlocking){
        if (fcntl(clifd, F_SETFL, 0) == -1)
            return -1;
    }
    else{
        if (fcntl(clifd, F_SETFL, fcntl(clifd, F_GETFD, 0)|O_NONBLOCK) == -1)
            return -1;
    }

    // add event
    if (addEvent(clifd, EPOLLIN) == -1)
        return -1;

    #ifdef LOGGER
    LOG(INFO) << "Build a new link from: "<< inet_ntoa(cliaddr.sin_addr) 
            << " PORT: " << cliaddr.sin_port
            << " Fd = " << clifd;
    #endif

    return 0;
}
