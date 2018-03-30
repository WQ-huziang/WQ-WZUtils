#include "util/socketserver.h"
#include "util/socketclient.h"
#include "util/logger.h"
#include "util/WZPiper.h"
#include "util/TcpPiper.h"

Logger *logger;

int main(int argc, char *argv[])
{
	char config_file_path[256];
	char ch;
    while((ch = getopt(argc, argv, "f:h")) != -1){
        switch(ch){
            case 'f':
                strcpy(config_file_path, optarg);
                break;
            default:
                printf("%s\n","input -h to check usage of the code");
                break;
        }
    }
    bool sockType = 1;
    WZPiper *c = new TcpPiper();
	c->parseConfigFile(config_file_path);
	int sockfd = c->socketInit(sockType);
	c->socketConnect();

	Frame my_frame;
	char *rep;
	for(;;) {
		rep = c->getInputStream(500);
		if (rep) {
			memcpy(&my_frame, rep, 500);
			printf("my_frame.source: %d\n",my_frame.source);
			printf("my_frame.msg_type: %d\n",my_frame.msg_type);
			// spi take advantage of my_frame
		}
		else {
		}
	}
	
	return 0;
}