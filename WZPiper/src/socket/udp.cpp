//
// Author: luojunbin

#include "udp.h"

UdpSocket::UdpSocket()
{

}

UdpSocket::~UdpSocket()
{

}

int UdpSocket::init(char file_path[256], int piperMode)
{
	CIni ini;
	if (ini.OpenFile(file_path, "r") == INI_OPENFILE_ERROR)
		return -1;
	char *temp = ini.GetStr("UDPNetInfo", "ip");
	strcpy(ip, temp);
	port = ini.GetInt("UDPNetInfo", "port");

	// create fd
	udpfd = socket(AF_INET, SOCK_DGRAM, 0);

	// set non_blocking
	if (fcntl(udpfd, F_SETFL, fcntl(udpfd, F_GETFD, 0)|O_NONBLOCK) == -1)
		return -1;

	// set addr
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr.sin_addr);
    addr.sin_port = htons (port);

    server_client_flag = piperMode;

    if (server_client_flag == 0)
    {
    	if (WZSocket::epollInit() == -1)
    		return -1;
    	if (Bind() == -1)
    		return -1;
    }
    else if (server_client_flag == 1)
    {
    	if (Connect() == -1)
    		return -1;
    }

	return udpfd;
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