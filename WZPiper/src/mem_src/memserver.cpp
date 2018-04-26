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


WZPiper<MemEngine<DataType, QueueSize, ReaderSize> >  * memServer;

int main(){
	char file[] = "../doc/config.ini";

	memServer = new WZPiper<MemEngine<DataType, QueueSize, ReaderSize> > ();
	memServer->init(file, WZ_PIPER_SERVER, WZ_PIPER_NBLOCK);
	
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
		printf("Recv result:%d\n", memServer->Recv(recvFrame));
		printf("Send result:%d\n", memServer->Send(sendFrame));
	}

	return 0;
}