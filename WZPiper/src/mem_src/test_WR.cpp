/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: luoqingming
Description: simple gtest for the module MemReader and MemWriter
Date: 2018-04-2
***************************************************************************/

#include "MemReader.h"
#include "MemWriter.h"
#include "logger.h"
#include "gtest/gtest.h"
#include <bits/stdc++.h>

#ifndef PRT
#define PRT(...) printf(__VA_ARGS__)
//#define PRT(...)
#endif

Logger * logger;

class WriterReaderTest : public ::testing::Test{
protected:
	MemEngine * memWriter;
	MemEngine * memReader1;
	MemEngine * memReader2;

	virtual void SetUp(){
		// init all
		char file[] = "../doc/config.ini";
		logger -> ParseConfigInfo(file);

		memWriter = new MemWriter();
		memWriter -> setConfigInfo(file);
		memWriter -> initAsWriter();

		memReader1 = new MemReader();
		memReader1 -> setConfigInfo(file);
		memReader1 -> initAsReader();

		memReader2 = new MemReader();
		memReader2 -> setConfigInfo(file);
		memReader2 -> initAsReader();
	}
	virtual void TearDown(){
		delete memWriter;
		delete memReader1;
		delete memReader2;
	}
};

TEST_F(WriterReaderTest, WriteAndReadWorks){

	// test write
	Frame sendFrame;
	for (int i = 0; i<DataQueueSize ; i++ ) {
		sendFrame.source = i;
		sendFrame.msg_type = i;
		sendFrame.error_id = WZ_ERROR_ID_SUCCESS;
		sendFrame.rtn_type = i;
		sendFrame.length = i;
		EXPECT_EQ(0,memWriter -> writeMem(sendFrame));
	}

	// test queue full
	EXPECT_EQ(-1,memWriter -> writeMem(sendFrame));

	// test reader1
	Frame recvFrame;

	for (int i = 0; i<DataQueueSize ; i++ ) {
		EXPECT_EQ(0,memReader1 -> readMem(recvFrame));
		EXPECT_EQ(i,recvFrame.source);
		EXPECT_EQ(i,recvFrame.msg_type );
		EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
		EXPECT_EQ(i,recvFrame.rtn_type);
		EXPECT_EQ(i,recvFrame.length);

	}
	// test catch writer
	EXPECT_EQ(-1,memReader1 -> readMem(recvFrame));
	EXPECT_EQ(DataQueueSize - 1,recvFrame.source);
	EXPECT_EQ(DataQueueSize - 1,recvFrame.msg_type );
	EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
	EXPECT_EQ(DataQueueSize - 1,recvFrame.rtn_type);
	EXPECT_EQ(DataQueueSize - 1,recvFrame.length);

	// test reader2
	for (int i = 0; i<DataQueueSize ; i++ ) {
		EXPECT_EQ(0,memReader2 -> readMem(recvFrame));
		EXPECT_EQ(i,recvFrame.source);
		EXPECT_EQ(i,recvFrame.msg_type );
		EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
		EXPECT_EQ(i,recvFrame.rtn_type);
		EXPECT_EQ(i,recvFrame.length);
	}
	// test catch writer
	EXPECT_EQ(-1,memReader2 -> readMem(recvFrame));
	EXPECT_EQ(DataQueueSize - 1,recvFrame.source);
	EXPECT_EQ(DataQueueSize - 1,recvFrame.msg_type );
	EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
	EXPECT_EQ(DataQueueSize - 1,recvFrame.rtn_type);
	EXPECT_EQ(DataQueueSize - 1,recvFrame.length);

	// test write
	EXPECT_EQ(0,memWriter -> writeMem(sendFrame));
}

int main(int argc, char* argv[]){
	logger = new Logger(argv[0]);
	testing::InitGoogleTest(&argc, argv);
	
	return RUN_ALL_TESTS();
}