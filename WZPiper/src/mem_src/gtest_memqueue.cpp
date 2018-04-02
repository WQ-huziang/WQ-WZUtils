// Copyright(C) 2018, Wizard Quant
// Author: luoqingming
// Functions: simple gtest for the module MemQueue
// Date: 2018-03-30

#include "MemQueue.hpp"
#include "gtest/gtest.h"
#include <bits/stdc++.h>

#ifndef PRT(...)
//#define PRT(...) printf(__VA_ARGS__)
#define PRT(...)
#endif

class QueueTest : public ::testing::Test{
protected:

	MemQueue<int,3,3> q_i;
	MemQueue<double,3,2> q_d;
	int int_reader1;
	int int_reader2;
	int int_reader3;
	int dou_reader1;

	int int_x;
	double dou_x;

	virtual void SetUp(){

		PRT("q_i size is:%d\n", q_i.get_queue_size());
		
		// add as reader
		int_reader1 = q_i.add_reader();
		int_reader2 = q_i.add_reader();

		// push 2 value first
		EXPECT_EQ(1,q_i.push(1));
		EXPECT_EQ(1,q_i.push(2));

	}
};

TEST_F(QueueTest, QueueSize){
	// test queue size is correct 
	EXPECT_EQ(2,q_i.get_queue_size());
	EXPECT_EQ(0,q_d.get_queue_size());
}

TEST_F(QueueTest, EnqueueWorks){
	// push works
	EXPECT_EQ(1,q_i.push(3));
	EXPECT_EQ(0,q_i.push(4));
	// max read index move correctly
	EXPECT_EQ(3,q_i.get_max_read_index());
}

TEST_F(QueueTest, ReaderFollowWriterWorks){

	// reader pop the data[0]
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,int_x);

	// min_read_index move correctly
	EXPECT_EQ(1,q_i.get_min_read_index());

	// reader pop the data[1]
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(2,int_x);
	// reader2 catch writer, read failed, 
	// haven't writen for slot haven't been read by reader1 
	EXPECT_EQ(0,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,q_i.get_min_read_index());
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(2,int_x);

	EXPECT_EQ(2,q_i.get_min_read_index());

	// write_index move correctly
	EXPECT_EQ(2,q_i.get_write_index());

	// writer push a data and 2 reader read the data[2]
	EXPECT_EQ(1,q_i.push(3));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(3,int_x);
	// min_read_index move correctly when just a reader read
	EXPECT_EQ(2,q_i.get_min_read_index());
	// reader2 catch writer, read failed, haven't been read by reader1 so haven't writen
	EXPECT_EQ(0,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(3,int_x);
	// min_read_index move correctly
	EXPECT_EQ(3,q_i.get_min_read_index());

	// writer push a data and 2 reader read the data[3]
	EXPECT_EQ(1,q_i.push(4));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(4,int_x);
	// reader2 catch writer, read failed
	EXPECT_EQ(0,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(4,int_x);
	// reader1 catch reader2 and writer, read failed
	EXPECT_EQ(0,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(4,q_i.get_min_read_index());

	// writer push a data and 2 reader read the data[4]
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

// test reader chase and catch writer
TEST_F(QueueTest, ReaderCatchWriterWorks){

	// writer catch slowest reader

	// writer write data[2], ok
	EXPECT_EQ(1,q_i.push(3));
	EXPECT_EQ(3,q_i.get_write_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// writer write a data[3], failed, 
	// slot haven't been read by reader2
	EXPECT_EQ(0,q_i.push(4));
	EXPECT_EQ(3,q_i.get_write_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// reader1 read data[0]
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(0,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// test slot read time
	EXPECT_EQ(1, q_i.get_read_time(0));
	EXPECT_EQ(0, q_i.get_read_time(1));

	// reader1 read data[1]
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(0,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// reader1 read data[2]
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(3,int_x);
	EXPECT_EQ(0,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// reader1 read data[3], failed, catch the writer 
	// slot haven't been read by reader2
	EXPECT_EQ(0,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(3,int_x);
	EXPECT_EQ(0,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// writer write data[3], ok, 
	// slot have been read by reader2
	EXPECT_EQ(1,q_i.push(4));
	EXPECT_EQ(4,q_i.get_write_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	// reader1 follow the writer
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(1,q_i.get_min_read_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	// reader1 read data[3], failed, catch the writer 
	// slot haven't been read by reader2
	EXPECT_EQ(0,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(1,q_i.get_min_read_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	// reader1 read data[3], failed, catch the writer 
	// slot haven't been read by reader2
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

	// writer write data[2], ok
	EXPECT_EQ(1,q_i.push(3));
	EXPECT_EQ(3,q_i.get_write_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// writer write a data[3], failed, 
	// slot haven't been read by reader2
	EXPECT_EQ(0,q_i.push(4));
	EXPECT_EQ(3,q_i.get_write_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// reader1 read data[0]
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(0,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());
	EXPECT_EQ(1, q_i.get_read_time(0));

	// reader1 read data[1]
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(0,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// reader2 read data[0]
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// reader2 read data[1] reader2 catch reader1
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(2,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// reader2 read data[2] and beyond reader1
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(3,int_x);
	// min_read_index is the same (cannot increase)
	EXPECT_EQ(2,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// reader2 read data[0] catch writer, read failed, 
	// haven't been read by reader1 so haven't writen
	EXPECT_EQ(0,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(3,int_x);
	EXPECT_EQ(2,q_i.get_min_read_index());
	EXPECT_EQ(3,q_i.get_max_read_index());

	// write write data[0] 
	EXPECT_EQ(1,q_i.push(4));
	EXPECT_EQ(4,q_i.get_write_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	// reader1 read data[2]
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(3,int_x);
	EXPECT_EQ(3,q_i.get_min_read_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	// reader1 read data[0]
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(3,q_i.get_min_read_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	// reader1 read data[1] catch writer, read failed, 
	// haven't writen
	EXPECT_EQ(0,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(4,int_x);
	EXPECT_EQ(3,q_i.get_min_read_index());
	EXPECT_EQ(4,q_i.get_max_read_index());

	// write write data[1] 
	EXPECT_EQ(1,q_i.push(5));
	EXPECT_EQ(5,q_i.get_write_index());
	EXPECT_EQ(5,q_i.get_max_read_index());

	// write read data[1]
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(5,int_x);
	EXPECT_EQ(3,q_i.get_min_read_index());
	EXPECT_EQ(5,q_i.get_max_read_index());

	// writer write a data[2]
	EXPECT_EQ(1,q_i.push(6));
	EXPECT_EQ(6,q_i.get_write_index());
	EXPECT_EQ(6,q_i.get_max_read_index());	

	// writer write a data[0], failed, 
	// slot haven't been read by reader2
	EXPECT_EQ(0,q_i.push(7));
	EXPECT_EQ(6,q_i.get_write_index());
	EXPECT_EQ(6,q_i.get_max_read_index());

}

int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}