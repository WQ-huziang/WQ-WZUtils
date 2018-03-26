#include "util/TcpPiper.h"
#include "util/logger.h"

Logger *logger;

int main(int argc, char *argv[])
{
	char config_file_path[256];
	WZPiper *pip = new TcpPiper();

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

	pip->set_config_info(config_file_path);
	pip->init_as_server();

	Frame my_frame;
	for(;;) {
		if (pip->do_read(my_frame) > 0) {
			printf("my_frame.source: %d\n",my_frame.source);
			printf("my_frame.msg_type: %d\n",my_frame.msg_type);
			// spi take advantage of my_frame
		}
		else {
			
		}
	}
	
	return 0;
}