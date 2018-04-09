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

	MemQueue<int,4,3> q_i;
	MemQueue<double,4,2> q_d;
	int int_reader1;
	int int_reader2;
	int int_reader3;
	int dou_reader1;

	int int_x;
	double dou_x;

	virtual void SetUp(){
		printf("%d\n", sizeof(q_i));

		int_reader1 = q_i.addReader();
		int_reader2 = q_i.addReader();

		EXPECT_EQ(1,q_i.push(1));
		EXPECT_EQ(1,q_i.push(2));

	}
};

TEST_F(QueueTest, QueueSize){
	EXPECT_EQ(2,q_i.getQueueSize());
	EXPECT_EQ(0,q_d.getQueueSize());
}

int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}