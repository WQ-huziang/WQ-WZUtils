#include <glog/logging.h>

int main(int argc, char *argv[]){
	google::InitGoogleLogging(argv[0]);
	FLAGS_log_dir = "./log";
	LOG(INFO) << "hello word";

	return 0;
}