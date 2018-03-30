#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "util/logger.h"
#include "util/UdpPiper.h"

extern Logger *logger;


UdpPiper::UdpPiper(){
    strcpy(ip,"");
    port = 12345;
    sockfd = 0;
}


UdpPiper::~UdpPiper(){
    strcpy(ip,"");
    port = 12345;
    sockfd = 0;
    close(sockfd);
}

int UdpPiper::socketInit(bool sockType)
{
	is_server = sockType;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	return sockfd;
}

int UdpPiper::socketConnect()
{
	char buffer[256];
	if(is_server == 1)
	{
	    if(sockfd == -1)
	    {
	        sprintf(buffer, "Server socket create error");
	        logger -> Error(buffer);
	        return -1;
	    } 
	    else 
	    {
	        sprintf(buffer,"Server socket create success");
	        logger -> Info(buffer);  
	    }

	    // struct sockaddr_in addr;
	    bzero(&addr,sizeof(addr));
	    addr.sin_family = AF_INET;
	    addr.sin_port = htons(port);
	    addr.sin_addr.s_addr = inet_addr(ip); 

	    if(bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0 )
	    {
	        sprintf(buffer,"UDP server bind error");
	        logger -> Error(buffer);
	        return -1;
	    } 
	    else 
	    {
	        sprintf(buffer,"UDP server bind success");
	        logger -> Info(buffer);
	        printf("call logger Info end\n");
	        return 0;
	    }
	}
	else
	{
	    if(sockfd == -1) 
	    {
	        sprintf(buffer,"UDP client socket init error");
	        logger -> Error(buffer);
	        return -1;
	    } 
	    else 
	    {
	        sprintf(buffer,"UDP client socket init success");
	        logger -> Info(buffer);
	    }

	    int is_broadcast = 1;
	    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &is_broadcast, sizeof(is_broadcast));

	    // struct  sockaddr_in addr;
	    bzero(&addr,sizeof(addr));
	    addr.sin_family = AF_INET;
	    addr.sin_port = htons(port);
	    addr.sin_addr.s_addr = inet_addr(ip);
	    return 0;
	}
}

// set the ip and port according to the config file
int UdpPiper::parseConfigFile(char file_path[256]){

    CIni ini;
    if (ini.OpenFile(file_path, "r") == INI_OPENFILE_ERROR) 
        return -1;
    char* ini_ip = ini.GetStr("UDPNetInfo","ip");
    strcpy(ip,ini_ip);
    port = ini.GetInt("UDPNetInfo","port");
    char buffer[256];
    sprintf(buffer, "config port = %d, config ip = %s", port, ip);
    logger -> Info(buffer);
    return 0;
}
// read data from the pipe
char* UdpPiper::getInputStream(int len)
{

	char *data = new char[500];
    int length;
    char buffer[256];
    
    socklen_t in_socklen = sizeof(addr);
    length = recvfrom(sockfd, data, len, 0, (struct sockaddr* ) &addr, &in_socklen);

    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"recv from port :%d", addr.sin_port);
    logger -> Info(buffer);

    if (length < 0) 
    {
        cout << "length " << length << endl;
        sprintf(buffer,"UDP read error");
        logger -> Error(buffer);
        return NULL;
    } 
    else if (length > 0) 
    {
        return data;
    }
    else 
    {
        sprintf(buffer,"receive wrong data");
        logger -> Warn(buffer);
        return NULL;
    }
    return NULL;
}

// write data to the pipe
int UdpPiper::getOutputStream(void *data, int len){
	char buffer[256];
    int length;
    memset(buffer, 0, sizeof(buffer));

    length = sendto(sockfd, data, len, 0, (struct sockaddr*) &addr, sizeof(addr) );

    sprintf(buffer,"writing to port :%d", addr.sin_port);
    logger -> Info(buffer);

    if (!length) 
    {
        sprintf(buffer,"writing error");
        logger -> Error(buffer);
        return -1;
    } 
    else 
    {
        sprintf(buffer,"wrote successfully, len = %d", length);
        logger -> Info(buffer);
        return 0;
    }
}
