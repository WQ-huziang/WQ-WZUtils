#include <sys/socket.h>  
#include <netinet/in.h>  
#include <string.h> 
#include <unistd.h>
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
   

   // send message
   bool sockType = 0;
   WZPiper * piper1 = new UdpPiper();
   piper1 -> parseConfigFile(filePathOut);
   piper1 -> socketInit(sockType);
   piper1 -> socketConnect(); 


   Frame frame;
   frame.source = 0;
   frame.msg_type = 1;
   frame.error_id = WZ_ERROR_ID_SUCCESS;
   frame.rtn_type = 3;
   frame.length = 4;
<<<<<<< HEAD:WZPiper/src/udp_src/testClient.cpp

   int len = sizeof(frame);
   char* rep = new char[500];
   memcpy(rep, &frame, len);

   for (;;) {
      piper1 -> getOutputStream(rep, 500);
      usleep(10000);

=======

   for (;;) {
      piper1 -> do_write(frame);
>>>>>>> ee4aef9acb823f0e0a64c913562f1e4a823c4464:WZPiper/src/udp_src/testClient.cpp
   }
   
   
   // // receive message
   // WZPiper * piper2 = new UdpPiper();
   // piper2 -> set_config_info(filePathIn);
   // piper2 -> init_as_server();

   // // declare a frame to receive data;
   // Frame recvFrame;
   // piper2 -> do_read(recvFrame);

   // PRINTSTR("recv source = ");
   // PRINTINT(recvFrame.source);

   // PRINTSTR("recv msg_type = ");
   // PRINTINT(recvFrame.msg_type);

   // PRINTSTR("recv error_id = ");
   // PRINTINT(recvFrame.error_id);

   // PRINTSTR("recv rtn_type = ");
   // PRINTINT(recvFrame.rtn_type);

   // PRINTSTR("recv length = ");
   // PRINTINT(recvFrame.length);

   // PRINTSTR("recv data = ");
   // PRINTSTR(recvFrame.data);

   exit(0);  
   return 0;  
}  