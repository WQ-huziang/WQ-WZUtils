#include "TcpPiper.h"
#include "WZPiper.h"

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
	pip->init_as_client();

	Frame my_frame;

	// for(;;) {
	// 	int nfds = pip->handle_event();
	// 	for (int i=0; i<nfds; i++) {
	// 		pip->test();
	// 	}
	// }

	pip->close_server();
	return 0;
}