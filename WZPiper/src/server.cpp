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
	pip->init_as_server();

	for(;;) {
		int num_of_events = pip->wait_event();
		for(int i=0; i<num_of_events; i++) {
			pip->set_event_fd(i);
			if (pip->handle_accept()) 
				continue;
			Frame myframe = pip->do_read();
		}
	}

	pip->close_server();
	return 0;
}