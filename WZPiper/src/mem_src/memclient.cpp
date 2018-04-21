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

int main(){
	char file[] = "../doc/config.ini";
	WZPiper<MemEngine<DataType, QueueSize, ReaderSize> >  * memClient1;
	memClient1 = new WZPiper<MemEngine<DataType, QueueSize, ReaderSize> > ();
	memClient1->init(file, WZ_PIPER_CLIENT, WZ_PIPER_NBLOCK);
	
	WZPiper<MemEngine<DataType, QueueSize, ReaderSize> >  * memClient2;
	memClient2 = new WZPiper<MemEngine<DataType, QueueSize, ReaderSize> > ();
	memClient2->init(file, WZ_PIPER_CLIENT, WZ_PIPER_NBLOCK);
	

	// test write
	Frame sendFrame;
	sendFrame.source = 1;
	sendFrame.msg_type = 2;
	sendFrame.error_id = WZ_ERROR_ID_SUCCESS;
	sendFrame.rtn_type = 3;
	sendFrame.length = 4;

	while(1){
		printf("send result:%d\n",memClient1->Send(sendFrame));
	}
	return 0;
}