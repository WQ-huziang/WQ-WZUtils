/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: luoqingming
Description: simple gtest for the module MemQueue reader
Date: 2018-03-30
***************************************************************************/

#include "memengine.hpp"
#include "gtest/gtest.h"
#include <bits/stdc++.h>
using namespace std;

#ifndef PRT
#define PRT(...) printf(__VA_ARGS__)
//#define PRT(...)
#endif
Logger *logger;

class QueueTest : public ::testing::Test{
protected:

	MemQueue<int,4,4> q_i;
	int int_reader1;
	int int_reader2;
	int int_reader3;
	int int_reader4;

	int int_x;
	static void SetUpTestCase(){
		logger = new Logger("test_reader");
		char file[] = "../doc/config.ini";
		logger -> ParseConfigInfo(file);
	}

	virtual void SetUp(){
		LOG(INFO) << "sizeof(q_i):" << sizeof(q_i);

		int_reader1 = q_i.addReader();
		int_reader2 = q_i.addReader();

		EXPECT_EQ(1,q_i.push(0));
		EXPECT_EQ(1,q_i.push(1));

	}
};

TEST_F(QueueTest, AddHangReader1){
	EXPECT_EQ(0,int_reader1);
	EXPECT_EQ(1,int_reader2);
	EXPECT_EQ(2,q_i.getReaderSize());
	q_i.removeReader(int_reader2);
	int_reader3 = q_i.addReader();
	EXPECT_EQ(1,int_reader3);
	EXPECT_EQ(1,q_i.pop(int_x,int_reader3));
	EXPECT_EQ(0,int_x);
	EXPECT_EQ(1,q_i.pop(int_x,int_reader3));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,q_i.getReadTime(0));
	EXPECT_EQ(1,q_i.getReadTime(1));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(0,int_x);
	EXPECT_EQ(0,q_i.getReadTime(0));
	EXPECT_EQ(1,q_i.getReadTime(1));
	EXPECT_EQ(1,q_i.getReadIndex(0));
	EXPECT_EQ(2,q_i.getReadIndex(1));
	q_i.removeReader(int_reader3);
	EXPECT_EQ(0,q_i.getReadTime(0));
	EXPECT_EQ(0,q_i.getReadTime(1));
	EXPECT_EQ(1,q_i.getQueueSize());
}

TEST_F(QueueTest, AddHangReader2){
	EXPECT_EQ(0,int_reader1);
	EXPECT_EQ(1,int_reader2);
	EXPECT_EQ(2,q_i.getReaderSize());
	q_i.removeReader(int_reader2);
	int_reader3 = q_i.addReader();
	EXPECT_EQ(1,int_reader3);
	EXPECT_EQ(1,q_i.pop(int_x,int_reader3));
	EXPECT_EQ(0,int_x);
	EXPECT_EQ(1,q_i.pop(int_x,int_reader3));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,q_i.getReadTime(0));
	EXPECT_EQ(1,q_i.getReadTime(1));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(0,int_x);
	EXPECT_EQ(0,q_i.getReadTime(0));
	EXPECT_EQ(1,q_i.getReadTime(1));
	EXPECT_EQ(1,q_i.getReadIndex(0));
	EXPECT_EQ(2,q_i.getReadIndex(1));

	int_reader2 = q_i.addReader();
	EXPECT_EQ(1,q_i.getReadIndex(2));

	q_i.removeReader(int_reader3);
	EXPECT_EQ(0,q_i.getReadTime(0));
	EXPECT_EQ(0,q_i.getReadTime(1));
	EXPECT_EQ(1,q_i.getQueueSize());
	int_reader3 = q_i.addReader();
	EXPECT_EQ(1,int_reader3);

	EXPECT_EQ(1,q_i.push(2));
	EXPECT_EQ(1,q_i.push(3));
	EXPECT_EQ(1,q_i.push(4));
	EXPECT_EQ(0,q_i.push(5));
	
	EXPECT_EQ(1,q_i.pop(int_x,int_reader3));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader3));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	q_i.removeReader(int_reader2);
	EXPECT_EQ(0,q_i.getReadTime(0));
	EXPECT_EQ(1,q_i.getReadTime(1));
	EXPECT_EQ(1,q_i.getReadTime(2));
	EXPECT_EQ(0,q_i.getReadTime(3));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(2,q_i.getMinReadIndex());
}


TEST_F(QueueTest, AddHangReader3){
	EXPECT_EQ(0,int_reader1);
	EXPECT_EQ(1,int_reader2);
	EXPECT_EQ(2,q_i.getReaderSize());
	q_i.removeReader(int_reader2);
	int_reader3 = q_i.addReader();
	EXPECT_EQ(1,int_reader3);
	EXPECT_EQ(1,q_i.pop(int_x,int_reader3));
	EXPECT_EQ(0,int_x);
	EXPECT_EQ(1,q_i.pop(int_x,int_reader3));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,q_i.getReadTime(0));
	EXPECT_EQ(1,q_i.getReadTime(1));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(0,int_x);
	EXPECT_EQ(0,q_i.getReadTime(0));
	EXPECT_EQ(1,q_i.getReadTime(1));
	EXPECT_EQ(1,q_i.getReadIndex(0));
	EXPECT_EQ(2,q_i.getReadIndex(1));

	int_reader2 = q_i.addReader();
	EXPECT_EQ(1,q_i.getReadIndex(2));

	q_i.removeReader(int_reader3);
	EXPECT_EQ(0,q_i.getReadTime(0));
	EXPECT_EQ(0,q_i.getReadTime(1));
	EXPECT_EQ(1,q_i.getQueueSize());
	int_reader3 = q_i.addReader();
	EXPECT_EQ(1,int_reader3);

	EXPECT_EQ(1,q_i.push(2));
	EXPECT_EQ(1,q_i.push(3));
	EXPECT_EQ(1,q_i.push(4));
	EXPECT_EQ(0,q_i.push(5));
	
	EXPECT_EQ(1,q_i.pop(int_x,int_reader3));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader3));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader3));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	q_i.removeReader(int_reader1);
	EXPECT_EQ(2,q_i.getMinReadIndex());
	EXPECT_EQ(0,q_i.getReadTime(0));
	EXPECT_EQ(0,q_i.getReadTime(1));
	EXPECT_EQ(1,q_i.getReadTime(2));
	EXPECT_EQ(1,q_i.getReadTime(3));

	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(0,q_i.getReadTime(0));
	EXPECT_EQ(0,q_i.getReadTime(1));
	EXPECT_EQ(0,q_i.getReadTime(2));
	EXPECT_EQ(1,q_i.getReadTime(3));
	EXPECT_EQ(3,q_i.getMinReadIndex());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));

	EXPECT_EQ(4,q_i.getMinReadIndex());
}

int main(int argc, char* argv[]){
	
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}