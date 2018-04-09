//
// Created by w2w on 18-3-29.
//

#include "util/fqueue/fqueue.h"
#include <gtest/gtest.h>

class QueueTest : public ::testing::Test{

protected:
    virtual void SetUp(){
        q1_.push(2.0);
        q1_.push(3.0);
    }
    FQueue<int> q0_;
    FQueue<double> q1_;
};

TEST_F(QueueTest, IsEmptyInitially){
    EXPECT_EQ(0, q0_.size());
}

TEST_F(QueueTest, DequeueWorks){
    int x;
    EXPECT_EQ(false, q0_.pop(x));

    double y;
    EXPECT_EQ(true, q1_.pop(y));
    EXPECT_EQ(2.0, y);
    EXPECT_EQ(true, q1_.pop(y));
    EXPECT_EQ(3.0, y);
    EXPECT_EQ(0, q1_.size());
}

int main(int argc, char *argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}