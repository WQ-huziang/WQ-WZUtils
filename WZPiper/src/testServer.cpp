#include "Ini.h"
#include<stdio.h>  
#include<unistd.h>  
#include<stdlib.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include<string.h>  
#include<iostream>
#include "UDPPiper.h"

#define PRINTSTR(str) printf("%s\n", str);
#define PRINTINT(num) printf("%d\n", num);
  
  
int main(int argc,char* argv[])   
{  

   //get parameters
   char ch;
   char* filePathIn;
   char* filePathOut;

   while((ch = getopt(argc, argv, "i:o:p:h"))!= -1){
      switch(ch){

         case 'i':
            filePathIn = optarg;
            printf("%s\n", filePathIn);
            break;

         case 'o':
            filePathOut = optarg;
            printf("%s\n", filePathOut);
            break;

         case 'h':
            printf("Options:\n");
            printf("Usage: ./Md -opt1 para1 -opt2 para2 ...\n");
            printf("-f: login config filePath\n");
            // printf("-n: Sum of instrument\n");
            printf("-h: Help to list the options\n");
            exit(0);
            break;

         default:
            printf("Usage: ./ -opt1 para1 -opt2 para2 ...\n");
            printf("Use \"./ -h\" to see the options\n");
            exit(0);
            break;
      }
   }
   WZPiper * piper2 = new UDPPiper();
   piper2 -> set_config_info(filePathIn);
   piper2 -> init_as_server();

   Frame recvFrame;

   for (int i = 0; i < 10; i++) {
      piper2 -> do_read(recvFrame);

      PRINTSTR("recv source = ");
      PRINTINT(recvFrame.source);

      PRINTSTR("recv msg_type = ");
      PRINTINT(recvFrame.msg_type);

      PRINTSTR("recv error_id = ");
      PRINTINT(recvFrame.error_id);

      PRINTSTR("recv rtn_type = ");
      PRINTINT(recvFrame.rtn_type);

      PRINTSTR("recv length = ");
      PRINTINT(recvFrame.length);

      PRINTSTR("recv data = ");
      PRINTSTR(recvFrame.data);
   }


   WZPiper * piper1 = new UDPPiper();
   piper1 -> set_config_info(filePathOut);
   piper1 -> init_as_client();
   
   Frame frame;
   frame.source = 0;
   frame.msg_type = 1;
   frame.error_id = 2;
   frame.rtn_type = 3;
   frame.length = 4;
   char* data = new char [5];
   strcpy(data, "4321");
   memcpy(frame.data, data, 5);
   piper1 -> do_write(frame);
   


   
   exit(0);  
   return 0;  
}  