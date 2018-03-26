#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "util/logger.h"
#include "util/UdpPiper.h"

extern Logger *logger;

// initialize some private variable
UdpPiper::UdpPiper(){
    strcpy(this->UDP_ip,"");
    this->UDP_port = 12345;
    this->UDP_sockfd = 0;
    memset(this->buffer, 0, sizeof(this->buffer));
}

// destructor
UdpPiper::~UdpPiper(){
    strcpy(this->UDP_ip,"");
    this->UDP_port = 12345;
    this->UDP_sockfd = 0;
    memset(this->buffer, 0, sizeof(this->buffer));
    close(this->UDP_sockfd);
}

// initialize the server
int UdpPiper::init_as_server(){
    
    this->UDP_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(this->UDP_sockfd == -1){
        sprintf(buffer,"server socket create error");
        logger -> Error(buffer);
        return -1;
    }

    // struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->UDP_port);
    addr.sin_addr.s_addr = inet_addr(this->UDP_ip);

    if(bind(this->UDP_sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0 ){
        sprintf(buffer,"UDP server bind error");
        logger -> Error(buffer);
        return -1;
    } else {
        sprintf(buffer,"UDP server bind success");
        logger -> Info(buffer);
        printf("call logger Info end\n");
        return 0;
    }
    return -1;
}

// set the ip and port according to the config file
int UdpPiper::set_config_info(char file_path[256]){
    CIni ini;

    if (ini.OpenFile(file_path, "r") == INI_OPENFILE_ERROR)
    	return -1;

    char* ini_ip = ini.GetStr("UDPNetInfo","ip");
    strcpy(this->UDP_ip,ini_ip);

    this->UDP_port = ini.GetInt("UDPNetInfo","port");

    sprintf(buffer, "config port = %d, config ip = %s", this->UDP_port, this->UDP_ip);
    logger -> Info(buffer);

    return 0;
}

// client link to the server
int UdpPiper::init_as_client(){

    if( (this->UDP_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        sprintf(buffer,"UDP client socket init error");
        logger -> Error(buffer);
        return -1;
    } else {
        sprintf(buffer,"UDP client socket init success");
        logger -> Info(buffer);
    }


    int is_broadcast = 1;
    setsockopt(this->UDP_sockfd, SOL_SOCKET, SO_BROADCAST, &is_broadcast, sizeof(is_broadcast));

    // struct  sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->UDP_port);
    addr.sin_addr.s_addr = inet_addr(this->UDP_ip);

    return 0;

}

// read data from the pipe
int UdpPiper::do_read(Frame& mail){

    int length;

    memset(&mail, 0, sizeof(Frame));
    
    socklen_t in_socklen ;

    length = recvfrom(this->UDP_sockfd, &mail, sizeof(Frame), 0, (struct sockaddr* ) &addr, &in_socklen);

    memset(this->buffer, 0, sizeof(this->buffer));
    sprintf(buffer,"recv from port :%d", addr.sin_port);
    logger -> Info(buffer);

    if (length < 0) {
        sprintf(buffer,"UDP read error");
        logger -> Error(buffer);
        return -1;
    } else if (length >= 0) {
        // memcpy(&mail, this->buffer, sizeof(this->buffer));
        if(mail.error_id == WZ_ERROR_ID_SUCCESS) {
            return 0;
        } else {
            sprintf(buffer,"receive wrong data");
            logger -> Warn(buffer);
            return -1;
        }
    }
    return 0;
}

// write data to the pipe
int UdpPiper::do_write(Frame& write_frame){
    int length;

    memset(this->buffer, 0, sizeof(this->buffer));

    length = sendto(this->UDP_sockfd, (char* ) &write_frame, sizeof(Frame), 0, (struct sockaddr*) &addr, sizeof(addr) );

    sprintf(buffer,"writing to port :%d", addr.sin_port);
    logger -> Info(buffer);

    if (!length) {

        sprintf(buffer,"writing error");
        logger -> Error(buffer);
        return -1;

    } else {
        sprintf(buffer,"wrote successfully, len = %d", length);
        logger -> Info(buffer);
        return 0;
    }
}
