#include<stdio.h>  
#include<unistd.h>  
#include<stdlib.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include<string.h>  
#include<iostream>
#include "util/UdpPiper.h"
#include "util/logger.h"

Logger *logger;
  
int main(int argc,char* argv[])   
{  

   //get parameters
   char ch;
   char* filePathIn;
   char* filePathOut;

   filePathOut = argv[1];

   logger = new Logger(argv[0]);
   logger->ParseConfigInfo(filePathOut);

   bool sockType = 1;
   WZPiper * piper2 = new UdpPiper();
   piper2 -> parseConfigFile(filePathOut);
   piper2 -> socketInit(sockType);
   piper2 -> socketConnect(); 

   Frame recvFrame;
   
   printf("get here\n");
   for (;;) {
      usleep(10000);
      char* rep = piper2 -> getInputStream(500);

      memcpy(&recvFrame, rep, 500);
      
      printf("recv source = %d\n", recvFrame.source);

      printf("recv msg_type = %d\n", recvFrame.msg_type );

      printf("recv error_id = %d\n", recvFrame.error_id);

      printf("recv rtn_type = %d\n", recvFrame.rtn_type);

      printf("recv length = %d\n", recvFrame.length);

   }


   // WZPiper * piper1 = new UdpPiper();
   // piper1 -> set_config_info(filePathOut);
   // piper1 -> init_as_client();
   
   // usleep(100);
   // Frame frame;
   // frame.source = 0;
   // frame.msg_type = 1;
   // frame.error_id = 2;
   // frame.rtn_type = 3;
   // frame.length = 4;
   // char* data = new char [5];
   // strcpy(data, "4321");
   // memcpy(frame.data, data, 5);
   // piper1 -> do_write(frame);
   
   return 0;  
}  