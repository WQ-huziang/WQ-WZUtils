//
// Author: luojunbin
//

#include "tcp.h"

int TcpSocket::init(char file_path[256])
{
	CIni ini;
	if (ini.OpenFile(file_path, "r") == INI_OPENFILE_ERROR)
		return -1;
	char *temp = ini.GetStr("TcpNetInfo", "ip");
	strcpy(ip, temp);
	port = ini.GetInt("TcpNetInfo", "port");

	// create fd
	tcpfd = socket(AF_INET, SOCK_STREAM, 0);

	// set non_blocking
	if (fcntl(tcpfd, F_SETFL, fcntl(tcpfd, F_GETFD, 0)|O_NONBLOCK) == -1)
		return -1;

	// set addr
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &addr.sin_addr);
    addr.sin_port = htons (port);

	return tcpfd;
}

int TcpSocket::wzBind()
{
	if (bind(tcpfd, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1)
		return -1;
	if (listen(tcpfd, MAXLISTENQUEUE) == -1)
		return -1;
	return 0;
}

int TcpSocket::wzConnect()
{
	connect(tcpfd, (struct sockaddr*)&addr, sizeof(addr));
	return 0;
}