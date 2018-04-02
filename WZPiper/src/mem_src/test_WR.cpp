// Copyright(C) 2018, Wizard Quant
// Author: luoqingming
// Functions: simple gtest for the module MemReader and MemWriter
// Date: 2018-04-2

#include "util/MemReader.h"
#include "util/MemWriter.h"
#include "gtest/gtest.h"
#include <bits/stdc++.h>

#ifndef PRT(...)
#define PRT(...) printf(__VA_ARGS__)
//#define PRT(...)
#endif

class QueueTest : public ::testing::Test{
protected:
	MemEngine * memWriter;
	MemEngine * memReader1;
	MemEngine * memReader2;

	virtual void SetUp(){
		printf("%d\n", sizeof(q_i));

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

int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}