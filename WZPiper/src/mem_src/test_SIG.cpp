#include<bits/stdc++.h>
#include "signalhandler.hpp"

#ifndef SHM_FLAG
#define SHM_FLAG IPC_CREAT|0666
#endif

Logger * logger;

char logger_buf[1024];

int main(){
	char file[] = "../doc/config.ini";
	logger -> ParseConfigInfo(file);

	int shmid;
	char * char_pointer;
	int reader_id;

	SignalHandler sh;
	SignalHandler::initSignalQueueManager(10,102400,SHM_FLAG,shmid,char_pointer);

	signal_queue_manager->initManager();
	reader_id = signal_queue_manager->frame_req_queue.addReader();
	LOG(INFO) << "reader_id:" << reader_id;

	int pid = getpid();
	sh.addToMap(pid,0,reader_id);

	reader_id = signal_queue_manager->frame_rec_queue.addReader();
	sh.addToMap(pid, 1, reader_id);
	LOG(INFO) << "pid:" << pid;

	sh.listenSignal(SIGINT);
	sh.listenSignal(SIGQUIT);
	sh.listenSignal(SIGTERM);
	while(1){
		printf("going to sleep\n");
		sleep(1);
	}
	return 0;
}