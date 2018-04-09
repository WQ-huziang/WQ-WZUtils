/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: luoqingming
Description: simple gtest for the module MemClient and MemServer
Date: 2018-04-2
***************************************************************************/

#include "memengine.h"
#include "wzpiper.hpp"
#include "logger.h"
#include "gtest/gtest.h"
#include <bits/stdc++.h>

#ifndef PRT
#define PRT(...) printf(__VA_ARGS__)
//#define PRT(...)
#endif

Logger * logger;

class ServerClientTest : public ::testing::Test{
protected:
	WZPiper<MemEngine> * memServer;
	WZPiper<MemEngine> * memClient1;
	WZPiper<MemEngine> * memClient2;

	virtual void SetUp(){
		// init all
		char file[] = "../doc/config.ini";
		logger -> ParseConfigInfo(file);

		memServer = new WZPiper<MemEngine>(0);
		memServer -> init(file);

		memClient1 = new WZPiper<MemEngine>(1);
		memClient1 -> init(file);

		memClient2 = new WZPiper<MemEngine>(1);
		memClient2 -> init(file);
	}
	virtual void TearDown(){
		delete memServer;
		delete memClient1;
		delete memClient2;
	}
};

TEST_F(ServerClientTest, WriteAndReadWorks){

	// test write
	Frame sendFrame;
	for (int i = 0; i<DataQueueSize ; i++ ) {
		sendFrame.source = i;
		sendFrame.msg_type = i;
		sendFrame.error_id = WZ_ERROR_ID_SUCCESS;
		sendFrame.rtn_type = i;
		sendFrame.length = i;
		EXPECT_EQ(0 ,memServer -> wzSend(sendFrame));
		if(i<(DataQueueSize/2)) 	EXPECT_EQ(0,memClient1 -> wzSend(sendFrame));
		else 	EXPECT_EQ(0,memClient2 -> wzSend(sendFrame));
	}

	// test queue full
	EXPECT_EQ(-1,memServer -> wzSend(sendFrame));
	EXPECT_EQ(-1,memClient1 -> wzSend(sendFrame));
	EXPECT_EQ(-1,memClient2 -> wzSend(sendFrame));
	
	// test Reader1
	Frame recvFrame;

	for (int i = 0; i<DataQueueSize ; i++ ) {
		EXPECT_EQ(0,memClient1 -> wzRecv(recvFrame));
		EXPECT_EQ(i,recvFrame.source);
		EXPECT_EQ(i,recvFrame.msg_type );
		EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
		EXPECT_EQ(i,recvFrame.rtn_type);
		EXPECT_EQ(i,recvFrame.length);


		EXPECT_EQ(0,memServer -> wzRecv(recvFrame));
		EXPECT_EQ(i,recvFrame.source);
		EXPECT_EQ(i,recvFrame.msg_type );
		EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
		EXPECT_EQ(i,recvFrame.rtn_type);
		EXPECT_EQ(i,recvFrame.length);
	}
	
	// test catch Writer
	EXPECT_EQ(-1,memClient1 -> wzRecv(recvFrame));
	EXPECT_EQ(DataQueueSize - 1,recvFrame.source);
	EXPECT_EQ(DataQueueSize - 1,recvFrame.msg_type );
	EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
	EXPECT_EQ(DataQueueSize - 1,recvFrame.rtn_type);
	EXPECT_EQ(DataQueueSize - 1,recvFrame.length);
	EXPECT_EQ(-1,memServer -> wzSend(sendFrame));

	// test Reader2
	for (int i = 0; i<DataQueueSize ; i++ ) {
		EXPECT_EQ(0,memClient2 -> wzRecv(recvFrame));
		EXPECT_EQ(i,recvFrame.source);
		EXPECT_EQ(i,recvFrame.msg_type );
		EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
		EXPECT_EQ(i,recvFrame.rtn_type);
		EXPECT_EQ(i,recvFrame.length);
	}

	// test catch Writer
	EXPECT_EQ(-1,memClient2 -> wzRecv(recvFrame));
	EXPECT_EQ(DataQueueSize - 1,recvFrame.source);
	EXPECT_EQ(DataQueueSize - 1,recvFrame.msg_type );
	EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
	EXPECT_EQ(DataQueueSize - 1,recvFrame.rtn_type);
	EXPECT_EQ(DataQueueSize - 1,recvFrame.length);

	// test write
	EXPECT_EQ(0,memServer -> wzSend(sendFrame));

}

int main(int argc, char* argv[]){
	logger = new Logger(argv[0]);
	testing::InitGoogleTest(&argc, argv);
	
	return RUN_ALL_TESTS();
}