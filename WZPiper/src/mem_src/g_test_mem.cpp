#include "MemQueue.h"
#include <gtest/gtest.h>

class QueueTest : public ::testing::Test{
protected:
	virtual void SetUp(){
		
	}
	MemQueue<int> q_int;
	MemQueue<double> q_double;
	int reader1;
	int reader2;
}

int main(int args, int* argv[]){
	MemQueue<int, 10> testqueue;

	return 0;
}