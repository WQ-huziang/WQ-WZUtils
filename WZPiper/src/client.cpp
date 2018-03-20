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

	Frame rec_frame;

	for(;;) {
		if (pip->do_read(rec_frame)) {
			printf("%d\n",rec_frame.source);
			printf("%d\n",rec_frame.msg_type);
		}
		else {
			// run strategy here
			printf("%s\n", "write frame");
			pip->do_write(my_frame_1);
			usleep(1000000);
		}
	}

	return 0;
}