/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: luoqingming
Description: simple gtest for the module MemQueue
Date: 2018-03-30
***************************************************************************/

#include "memengine.h"
#include "gtest/gtest.h"
#include <bits/stdc++.h>
using namespace std;

#ifndef PRT
#define PRT(...) printf(__VA_ARGS__)
//#define PRT(...)
#endif

class QueueTest : public ::testing::Test{
protected:

	MemQueue<int,4,1> q_i;
	int int_reader1;

	int int_x;

	virtual void SetUp(){
		printf("%ld\n", sizeof(q_i));

		int_reader1 = q_i.addReader();

		EXPECT_EQ(1,q_i.push(1));
		EXPECT_EQ(1,q_i.push(2));

	}
};

TEST_F(QueueTest, QueueSize){
	EXPECT_EQ(2,q_i.getQueueSize());
}

TEST_F(QueueTest, PushPop){
	EXPECT_EQ(1,q_i.pop(int_x, int_reader1));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(1,q_i.pop(int_x, int_reader1));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(0,q_i.pop(int_x, int_reader1));
	EXPECT_EQ(1,q_i.push(3));
	EXPECT_EQ(1,q_i.push(4));
	EXPECT_EQ(1,q_i.push(5));
	EXPECT_EQ(1,q_i.push(6));
	EXPECT_EQ(0,q_i.push(7));
}

int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}