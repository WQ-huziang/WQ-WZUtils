<<<<<<< HEAD:WZPiper/src/tcp_src/client.cpp
#include "util/logger.h"
// #include "util/client.h"
// #include "util/socketclient.h"
#include "util/WZPiper.h"
#include "util/TcpPiper.h"
=======
#include "util/TcpPiper.h"
#include "util/WZPiper.h"
#include "util/logger.h"
>>>>>>> ee4aef9acb823f0e0a64c913562f1e4a823c4464:WZPiper/src/tcp_src/client.cpp

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

    bool sockType = 0;
    WZPiper *c = new TcpPiper();
	c->parseConfigFile(config_file_path);
	int sockfd = c->socketInit(sockType);
	c->socketConnect();

	Frame my_frame_1;
	my_frame_1.source = 3;
	my_frame_1.msg_type = 3;
	my_frame_1.dest = sockfd;

	int len = sizeof(my_frame_1);
	char *rep = new char[len + 1];
	memset(rep, '\0', len + 1);
	memcpy(rep, &my_frame_1, len);
	
	Frame rec_frame;
	char* rep1;

	for(;;) {
<<<<<<< HEAD:WZPiper/src/tcp_src/client.cpp
		rep1 = c->getInputStream(500);
		if (rep1) {
			memcpy(&rec_frame, &rep1, len);
=======
		if (pip->do_read(rec_frame) > 0) {
>>>>>>> ee4aef9acb823f0e0a64c913562f1e4a823c4464:WZPiper/src/tcp_src/client.cpp
			printf("%d\n",rec_frame.source);
			printf("%d\n",rec_frame.msg_type);
		}
		else {
			printf("%s\n", "write frame");
			c->getOutputStream(rep, len);
			usleep(1000000);
		}
	}

	return 0;
}