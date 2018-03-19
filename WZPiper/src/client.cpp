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

	Frame my_frame_1;
	my_frame_1.source = 3;
	my_frame_1.msg_type = 3;

	int confd;
	int ctp_fd = pip->get_listen_fd();

	for(;;) {
		int num_of_events = pip->wait_event();
		if (num_of_events) {
			// recieve receipt
			printf("num_of_events: %d\n", num_of_events);
			for(int i=0; i<num_of_events; i++) {
				confd = pip->get_event_fd(i);
				pip->set_event_fd(confd);
				pip->do_read();
			}
		} else {
			// calculate and decide when to send what type of order
			// use strategy here
			pip->set_event_fd(ctp_fd);
			pip->do_write(my_frame_1);
			usleep(1000000);
		}
	}

	pip->close_link();
	return 0;
}