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
	WZPiper<MemEngine<DataType, QueueSize, ReaderSize> >  * memServer;
	memServer = new WZPiper<MemEngine<DataType, QueueSize, ReaderSize> > ();
	memServer->init(file, WZ_PIPER_SERVER, WZ_PIPER_NBLOCK);
	
	// test write
	Frame recvFrame;

	while(1){
		printf("recv result:%d\n", memServer->Recv(recvFrame));
	}

	return 0;
}