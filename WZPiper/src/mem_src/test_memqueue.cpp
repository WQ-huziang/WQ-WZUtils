#include "MemQueue.hpp"
#include "gtest/gtest.h"
#include <bits/stdc++.h>
using namespace std;

#define PRT(...) printf(__VA_ARGS__)


TEST_F(QueueTest, IsEmptyInitially){
	EXPECT_EQ(2,);
	EXPECT_EQ(0,);
}

TEST_F(QueueTest, EnqueueWorks){
	EXPECT_EQ(1,q_i.push(3));  
	EXPECT_EQ(0,q_i.push(4));
}

TEST_F(QueueTest, DequeueWorks){
	EXPECT_EQ(1,q_i.pop(int_x,int_reader1));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(2,int_x);
	EXPECT_EQ(1,q_i.pop(int_x,int_reader2));
	EXPECT_EQ(1,int_x);
	EXPECT_EQ(0,q_i.pop(int_x,int_reader2));

	EXPECT_EQ(1,q_i.push(3));
	EXPECT_EQ(1,q_i.push(4));
	EXPECT_EQ(0,q_i.push(5));
}

TEST_F(QueueTest, EmptyqueuepopWorks){

	EXPECT_EQ(0,q_d.pop(dou_x,dou_reader1));

}

int main(int argc, char* argv[]){
	MemQueue<int> q_i;
	MemQueue<double> q_d;
	int int_reader1;
	int int_reader2;
	int dou_reader1;

	int int_x;
	double dou_x;

	int_reader1 = q_i.add_reader();
	int_reader2 = q_i.add_reader();

	PRT("first push is:%d\n",q_i.push(1));
	PRT("second push is:%d\n",q_i.push(2));


	PRT("get_queue_size:%d\n",q_i.get_queue_size());
	PRT("second push is:%d\n",q_d.get_queue_size());

	return 0;
}