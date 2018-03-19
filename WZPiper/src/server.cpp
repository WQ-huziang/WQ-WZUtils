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
		if (num_of_events) {
			// recieve request here
			printf("num_of_events: %d\n", num_of_events);
			for(int i=0; i<num_of_events; i++) {
				int confd = pip->get_event_fd(i);
				pip->set_event_fd(confd);
				if (pip->handle_accept()) 
					continue;
				pip->do_read();
			}
		} else {
			// send receipt here
			
		}
	}

	pip->close_link();
	return 0;
}