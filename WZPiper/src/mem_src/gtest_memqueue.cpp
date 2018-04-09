/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: luoqingming
Description: simple gtest for the module MemQueue
Date: 2018-03-30
***************************************************************************/

#include "memengine.h"
#include "gtest/gtest.h"
#include <bits/stdc++.h>

#ifndef PRT
//#define PRT(...) printf(__VA_ARGS__)
#define PRT(...)
#endif

class QueueTest : public ::testing::Test{
protected:

	MemQueue<int,4,3> qI;
	MemQueue<double,4,2> q_d;
	int intReader1;
	int intReader2;
	int intReader3;
	int douReader1;

	int int_x;
	double dou_x;

	virtual void SetUp(){

		PRT("qI size is:%d\n", qI.getQueueSize());
		
		// add as reader
		intReader1 = qI.addReader();
		intReader2 = qI.addReader();

		// push 2 value first
		EXPECT_EQ(1,qI.push(1));
		EXPECT_EQ(1,qI.push(2));

	}
};

TEST_F(QueueTest, QueueSize){
	// test queue size is correct 
	EXPECT_EQ(2,qI.getQueueSize());
	EXPECT_EQ(0,q_d.getQueueSize());
}

TEST_F(QueueTest, EnqueueWorks){
	// push works
	EXPECT_EQ(1,qI.push(3));
	EXPECT_EQ(1,qI.push(4));
	EXPECT_EQ(0,qI.push(5));
	// max read index move correctly
	EXPECT_EQ(4,qI.getMaxReadIndex());
}

TEST_F(QueueTest, ReaderFollowWriterWorks){

	// reader pop the data[0]
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,qI.pop(int_x,intReader2));
	EXPECT_EQ(1,int_x);

	// minReadIndex move correctly
	EXPECT_EQ(1,qI.getMinReadIndex());

	// reader pop the data[1]
	EXPECT_EQ(1,qI.pop(int_x,intReader2));
	EXPECT_EQ(2,int_x);
	// reader2 catch writer, read failed, 
	// haven't writen for slot haven't been read by reader1 
	EXPECT_EQ(0,qI.pop(int_x,intReader2));
	EXPECT_EQ(1,qI.getMinReadIndex());
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(2,int_x);

	EXPECT_EQ(2,qI.getMinReadIndex());

	// writeIndex move correctly
	EXPECT_EQ(2,qI.getWriteIndex());

	// writer push a data and 2 reader read the data[2]
	EXPECT_EQ(1,qI.push(3));
	EXPECT_EQ(1,qI.pop(int_x,intReader2));
	EXPECT_EQ(3,int_x);
	// minReadIndex move correctly when just a reader read
	EXPECT_EQ(2,qI.getMinReadIndex());
	// reader2 catch writer, read failed, haven't been read by reader1 so haven't writen
	EXPECT_EQ(0,qI.pop(int_x,intReader2));
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(3,int_x);
	// minReadIndex move correctly
	EXPECT_EQ(3,qI.getMinReadIndex());

	// writer push a data and 2 reader read the data[3]
	EXPECT_EQ(1,qI.push(4));
	EXPECT_EQ(1,qI.pop(int_x,intReader2));
	EXPECT_EQ(4,int_x);
	// reader2 catch writer, read failed
	EXPECT_EQ(0,qI.pop(int_x,intReader2));
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(4,int_x);
	// reader1 catch reader2 and writer, read failed
	EXPECT_EQ(0,qI.pop(int_x,intReader1));
	EXPECT_EQ(4,qI.getMinReadIndex());

	// writer push data[0] and 2 reader read the data[4]
	EXPECT_EQ(1,qI.push(5));
	EXPECT_EQ(1,qI.pop(int_x,intReader2));
	EXPECT_EQ(5,int_x);
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(5,int_x);
	EXPECT_EQ(0,qI.pop(int_x,intReader1));
	
	
	EXPECT_EQ(5,qI.getMinReadIndex());
}

TEST_F(QueueTest, EmptyqueuepopWorks){

	EXPECT_EQ(0,q_d.pop(dou_x,douReader1));

}

// test reader chase and catch writer
TEST_F(QueueTest, ReaderCatchWriterWorks){

	// writer catch slowest reader

	// writer write data[2], ok
	EXPECT_EQ(1,qI.push(3));
	EXPECT_EQ(3,qI.getWriteIndex());
	EXPECT_EQ(3,qI.getMaxReadIndex());

	// writer write a data[3]
	EXPECT_EQ(1,qI.push(4));
	EXPECT_EQ(4,qI.getWriteIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// writer write a data[0], failed, 
	// slot haven't been read by reader2 and reader1
	EXPECT_EQ(0,qI.push(5));
	EXPECT_EQ(4,qI.getWriteIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// reader1 read data[0]
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(0,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// test slot read time
	EXPECT_EQ(1, qI.getReadTime(0));
	EXPECT_EQ(0, qI.getReadTime(1));

	// reader1 read data[1]
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(0,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// reader1 read data[2]
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(3,int_x);
	EXPECT_EQ(0,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// reader1 read data[3]
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(0,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// reader1 read data[0], failed, catch the writer 
	// slot haven't been read by reader2
	EXPECT_EQ(0,qI.pop(int_x,intReader1));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(0,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// reader2 read data[0]
	EXPECT_EQ(1,qI.pop(int_x,intReader2));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// writer write data[0], ok, 
	// slot have been read by reader2
	EXPECT_EQ(1,qI.push(5));
	EXPECT_EQ(5,qI.getWriteIndex());
	EXPECT_EQ(5,qI.getMaxReadIndex());

	// reader1 follow the writer
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(5,int_x);
	EXPECT_EQ(1,qI.getMinReadIndex());
	EXPECT_EQ(5,qI.getMaxReadIndex());

	// reader1 read data[1], failed, catch the writer 
	// slot haven't been read by reader2
	EXPECT_EQ(0,qI.pop(int_x,intReader1));
	EXPECT_EQ(5,int_x);
	EXPECT_EQ(1,qI.getMinReadIndex());
	EXPECT_EQ(5,qI.getMaxReadIndex());

	// writer write data[1], failed, catch the writer 
	// slot haven't been read by reader2
	EXPECT_EQ(0,qI.push(6));
	EXPECT_EQ(5,qI.getWriteIndex());
	EXPECT_EQ(5,qI.getMaxReadIndex());

	// reader2 read data[1]
	EXPECT_EQ(1,qI.pop(int_x,intReader2));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(2,qI.getMinReadIndex());
	EXPECT_EQ(5,qI.getMaxReadIndex());

	// writer write data[1]
	EXPECT_EQ(1,qI.push(6));
	EXPECT_EQ(6,qI.getWriteIndex());
	EXPECT_EQ(6,qI.getMaxReadIndex());

}

TEST_F(QueueTest, ReaderBeyondReaderWorks){

	// writer write data[2], ok
	EXPECT_EQ(1,qI.push(3));
	EXPECT_EQ(3,qI.getWriteIndex());
	EXPECT_EQ(3,qI.getMaxReadIndex());

	// writer write a data[3]
	EXPECT_EQ(1,qI.push(4));
	EXPECT_EQ(4,qI.getWriteIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// writer write a data[0], failed, 
	// slot haven't been read by reader2 and reader1
	EXPECT_EQ(0,qI.push(5));
	EXPECT_EQ(4,qI.getWriteIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// reader1 read data[0]
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(0,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());
	EXPECT_EQ(1, qI.getReadTime(0));

	// reader1 read data[1]
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(0,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// reader2 read data[0]
	EXPECT_EQ(1,qI.pop(int_x,intReader2));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// reader2 read data[1] reader2 catch reader1
	EXPECT_EQ(1,qI.pop(int_x,intReader2));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(2,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// reader2 read data[2] and beyond reader1
	EXPECT_EQ(1,qI.pop(int_x,intReader2));
	EXPECT_EQ(3,int_x);
	// minReadIndex is the same (cannot increase)
	EXPECT_EQ(2,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// reader2 read data[3]
	EXPECT_EQ(1,qI.pop(int_x,intReader2));
	EXPECT_EQ(4,int_x);
	// minReadIndex is the same (cannot increase)
	EXPECT_EQ(2,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// reader2 read data[0] catch writer, read failed, 
	// haven't writen
	EXPECT_EQ(0,qI.pop(int_x,intReader2));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(2,qI.getMinReadIndex());
	EXPECT_EQ(4,qI.getMaxReadIndex());

	// write write data[0] 
	EXPECT_EQ(1,qI.push(5));
	EXPECT_EQ(5,qI.getWriteIndex());
	EXPECT_EQ(5,qI.getMaxReadIndex());

	// reader1 read data[2]
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(3,int_x);
	EXPECT_EQ(3,qI.getMinReadIndex());
	EXPECT_EQ(5,qI.getMaxReadIndex());

	// reader1 read data[3]
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(4,qI.getMinReadIndex());
	EXPECT_EQ(5,qI.getMaxReadIndex());

	// reader1 read data[0], beyond reader2
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(5,int_x);
	EXPECT_EQ(4,qI.getMinReadIndex());
	EXPECT_EQ(5,qI.getMaxReadIndex());

	// reader1 read data[1] catch writer, read failed, 
	// haven't writen
	EXPECT_EQ(0,qI.pop(int_x,intReader1));
	EXPECT_EQ(5,int_x);
	EXPECT_EQ(4,qI.getMinReadIndex());
	EXPECT_EQ(5,qI.getMaxReadIndex());

	// write write data[1] 
	EXPECT_EQ(1,qI.push(6));
	EXPECT_EQ(6,qI.getWriteIndex());
	EXPECT_EQ(6,qI.getMaxReadIndex());

	// reader1 read data[1]
	EXPECT_EQ(1,qI.pop(int_x,intReader1));
	EXPECT_EQ(6,int_x);
	EXPECT_EQ(4,qI.getMinReadIndex());
	EXPECT_EQ(6,qI.getMaxReadIndex());

	// writer write a data[2]
	EXPECT_EQ(1,qI.push(7));
	EXPECT_EQ(7,qI.getWriteIndex());
	EXPECT_EQ(7,qI.getMaxReadIndex());	

	// writer write a data[3]
	EXPECT_EQ(1,qI.push(8));
	EXPECT_EQ(8,qI.getWriteIndex());
	EXPECT_EQ(8,qI.getMaxReadIndex());

	// writer write a data[3], failed, 
	// slot haven't been read by reader2
	EXPECT_EQ(0,qI.push(9));
	EXPECT_EQ(8,qI.getWriteIndex());
	EXPECT_EQ(8,qI.getMaxReadIndex());

}

int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}