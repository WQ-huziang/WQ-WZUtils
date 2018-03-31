// Copyright(C) 2018, Wizard Quant
// Author: luoqingming
// Functions: simple gtest for the module MemQueue
// Date: 2018-03-30

#include "MemQueue.hpp"
#include "gtest/gtest.h"
#include <bits/stdc++.h>
using namespace std;

#ifndef PRT(...)
#define PRT(...) printf(__VA_ARGS__)
//#define PRT(...)
#endif

class QueueTest : public ::testing::Test{
protected:

	QueueTest():q_i(3,3),q_d(2,2){}
	MemQueue<int> q_i;
	MemQueue<double> q_d;
	int int_reader1;
	int int_reader2;
	int int_reader3;
	int dou_reader1;

	int int_x;
	double dou_x;

	virtual void SetUp(){
		
		int_reader1 = q_i.add_reader();
		int_reader2 = q_i.add_reader();

		EXPECT_EQ(1,q_i.push(1));
		EXPECT_EQ(1,q_i.push(2));

	}
};

TEST_F(QueueTest, QueueSize){
	EXPECT_EQ(2,q_i.get_queue_size());
	EXPECT_EQ(0,q_d.get_queue_size());
}

TEST_F(QueueTest, EnqueueWorks){
	PRT("q_i size is:%d\n", q_i.get_queue_size());
	EXPECT_EQ(1,q_i.push(3));
	EXPECT_EQ(0,q_i.push(4));
	EXPECT_EQ(3,q_i.get_max_read_index());
}

TEST_F(QueueTest, ReaderFollowWriterWorks){


	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,int_x);

	EXPECT_EQ(1,q_i.get_min_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(0,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,q_i.get_min_read_index());
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(2,int_x);

	EXPECT_EQ(2,q_i.get_min_read_index());

	EXPECT_EQ(2,q_i.get_write_index());

	EXPECT_EQ(1,q_i.push(3));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(3,int_x);
	EXPECT_EQ(0,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(3,int_x);

	EXPECT_EQ(3,q_i.get_min_read_index());

	EXPECT_EQ(1,q_i.push(4));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(0,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(4,int_x);
	
	EXPECT_EQ(4,q_i.get_min_read_index());

	EXPECT_EQ(1,q_i.push(5));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(5,int_x);
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(5,int_x);
	EXPECT_EQ(0,q_i.pop(int_x,int_reader1));
	
	
	EXPECT_EQ(5,q_i.get_min_read_index());
}

TEST_F(QueueTest, EmptyqueuepopWorks){

	EXPECT_EQ(0,q_d.pop(dou_x,dou_reader1));

}

TEST_F(QueueTest, ReaderCatchWriterWorks){

	// writer catch slowest reader
	EXPECT_EQ(1,q_i.push(3));
	EXPECT_EQ(3,q_i.get_write_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(0,q_i.push(4));
	EXPECT_EQ(3,q_i.get_write_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(0,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());
	EXPECT_EQ(1, q_i.get_read_time(0));

	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(0,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(3,int_x);
	EXPECT_EQ(0,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(0,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(3,int_x);
	EXPECT_EQ(0,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.push(4));
	EXPECT_EQ(4,q_i.get_write_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(1,q_i.get_min_read_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	EXPECT_EQ(0,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(1,q_i.get_min_read_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	EXPECT_EQ(0,q_i.push(5));
	EXPECT_EQ(4,q_i.get_write_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(2,q_i.get_min_read_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.push(5));
	EXPECT_EQ(5,q_i.get_write_index());
	EXPECT_EQ(5,q_i.get_max_read_index());

}

TEST_F(QueueTest, ReaderBeyondReaderWorks){

	// writer catch slowest reader
	EXPECT_EQ(1,q_i.push(3));
	EXPECT_EQ(3,q_i.get_write_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(0,q_i.push(4));
	EXPECT_EQ(3,q_i.get_write_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(0,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());
	EXPECT_EQ(1, q_i.get_read_time(0));

	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(0,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(2,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(3,int_x);
	EXPECT_EQ(2,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(0,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(3,int_x);
	EXPECT_EQ(2,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.push(4));
	EXPECT_EQ(4,q_i.get_write_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(3,int_x);
	EXPECT_EQ(3,q_i.get_min_read_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(3,q_i.get_min_read_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	EXPECT_EQ(0,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(3,q_i.get_min_read_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.push(5));
	EXPECT_EQ(5,q_i.get_write_index());
	EXPECT_EQ(5,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(5,int_x);
	EXPECT_EQ(3,q_i.get_min_read_index());
	EXPECT_EQ(5,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.push(6));
	EXPECT_EQ(6,q_i.get_write_index());
	EXPECT_EQ(6,q_i.get_max_read_index());

	EXPECT_EQ(0,q_i.push(7));
	EXPECT_EQ(6,q_i.get_write_index());
	EXPECT_EQ(6,q_i.get_max_read_index());

}

int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}