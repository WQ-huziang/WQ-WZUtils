#include <sys/socket.h>  
#include <netinet/in.h>  
#include <string.h> 
#include <unistd.h>
#include "util/MemEngine.h"
#include "util/MemWriter.h"
#include "util/logger.h"

Logger *logger;
  
int main(int argc,char* argv[])   
{  

   //get parameters
   char ch;
   char* filePathIn;
   char* filePathOut;

   while((ch = getopt(argc, argv, "f:p:h"))!= -1){
      switch(ch){

         case 'f':
            filePathIn = optarg;
            printf("%s\n", filePathIn);
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

   // logger = new Logger(argv[0]);
   // logger->ParseConfigInfo(filePathOut);
   

   // send message
   MemEngine * memWriter = new MemWriter();
   memWriter -> set_config_info(filePathIn);
   memWriter -> init_as_writer();

   Frame frame;

   for (int i = 0; i<4 ; i++ ) {
      frame.source = i;
      frame.msg_type = i;
      frame.error_id = WZ_ERROR_ID_SUCCESS;
      frame.rtn_type = i;
      frame.length = i;

      printf("write result is:%d\n", memWriter -> write_mem(frame));
   }
  
   return 0;  
}  