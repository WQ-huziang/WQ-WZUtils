#include "MemQueue.h"
#include "gtest/gtest.h"
#include <bits/stdc++.h>
using namespace std;

#define PRT(...) printf(__VA_ARGS__)

class QueueTest : public ::testing::Test{
protected:

	QueueTest():q_i(3,2),q_d(2,2){}
	MemQueue<int> q_i;
	MemQueue<double> q_d;
	int int_reader1;
	int int_reader2;
	int dou_reader1;

	int int_x;
	double dou_x;

	virtual void SetUp(){
		int_reader1 = q_i.add_reader();
		int_reader2 = q_i.add_reader();

		PRT("first push is:%d\n",q_i.push(1));
		PRT("get_write_index:%d\n", q_i.get_write_index());
		PRT("get_max_read_index:%d\n", q_i.get_max_read_index());
		PRT("get_min_read_index:%d\n", q_i.get_min_read_index());
		PRT("slowest_read_time:%d\n", q_i.get_slowest_read_time());

		PRT("second push is:%d\n",q_i.push(2));
		PRT("get_write_index:%d\n", q_i.get_write_index());
		PRT("get_max_read_index:%d\n", q_i.get_max_read_index());
		PRT("get_min_read_index:%d\n", q_i.get_min_read_index());
		PRT("slowest_read_time:%d\n", q_i.get_slowest_read_time());
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
}

TEST_F(QueueTest, DequeueWorks){

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

int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}