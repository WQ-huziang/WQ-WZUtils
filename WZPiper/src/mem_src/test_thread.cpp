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

	//QueueTest():q_i(3,3),q_d(2,2){}
	MemQueue<int>* q_i;
	MemQueue<double>* q_d;
	int int_reader1;
	int int_reader2;
	int int_reader3;
	int dou_reader1;

	int int_x;
	double dou_x;

	virtual void SetUp(){
		q_i = new MemQueue<int>(1000,3);
		q_d = new MemQueue<double>(2,2);

		printf("%d\n", sizeof(*q_i));

		int_reader1 = q_i->add_reader();
		int_reader2 = q_i->add_reader();

		EXPECT_EQ(1,q_i->push(1));
		EXPECT_EQ(1,q_i->push(2));

	}
};

TEST_F(QueueTest, QueueSize){
	EXPECT_EQ(2,q_i->get_queue_size());
	EXPECT_EQ(0,q_d->get_queue_size());
}

int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}