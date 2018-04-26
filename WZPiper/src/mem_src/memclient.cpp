#include<bits/stdc++.h>
#include "memengine.hpp"
#include "signalhandler.hpp"

#ifndef DataType
#define DataType Frame
#endif

#ifndef QueueSize
#define QueueSize 1024
#endif

#ifndef ReaderSize
#define ReaderSize 4
#endif

Logger * logger;

char logger_buf[1024];

WZPiper<MemEngine<DataType, QueueSize, ReaderSize> >  * memClient1;

int main(){
	char file[] = "../doc/config.ini";
	
	memClient1 = new WZPiper<MemEngine<DataType, QueueSize, ReaderSize> > ();
	memClient1->init(file, WZ_PIPER_CLIENT, WZ_PIPER_NBLOCK);
	
	// test write
	Frame recvFrame;

	// test write
	Frame sendFrame;
	sendFrame.source = 1;
	sendFrame.msg_type = 2;
	sendFrame.error_id = WZ_ERROR_ID_SUCCESS;
	sendFrame.rtn_type = 3;
	sendFrame.length = 4;

	while(1){
		printf("Send result:%d\n",memClient1->Send(sendFrame));
		printf("Recv result:%d\n",memClient1->Recv(sendFrame));
	}
	return 0;
}