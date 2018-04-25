//
// Author: luojunbin
// 

#include "udp.h"
#include "wzpiper.hpp"
#include "gtest/gtest.h"
#include <bits/stdc++.h>

WZPiper<UdpSocket> pip;
Frame send_frame;
Frame recv_frame;

void setFrame()
{
	/*
	*	send_frame
	*/
	memset(&send_frame, 0, sizeof(send_frame));
	send_frame.source = 1;

	/*
	*	recv_frame
	*/
	memset(&recv_frame, 0, sizeof(recv_frame));
}

class UdpTest : public ::testing::Test{
protected:
    static void SetUpTestCase()
    {
        setFrame();
    }
    static void TearDownTestCase()
    {
                
    }
    // virtual void SetUp()
    // {

    // }
    // virtual void TearDown()
    // {

    // }
};

TEST_F(UdpTest, InitServer){
	EXPECT_NE(-1, pip.init("../doc/config.ini", WZ_PIPER_SERVER, WZ_PIPER_BLOCK));
}

TEST_F(UdpTest, InitClient){
	EXPECT_NE(-1, pip.init("../doc/config.ini", WZ_PIPER_CLIENT, WZ_PIPER_BLOCK));
}

TEST_F(UdpTest, Send)
{
	EXPECT_NE(-1, pip.Send(send_frame));
}

TEST_F(UdpTest, Recv)
{
	EXPECT_EQ(-1, pip.Recv(recv_frame));
}

int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}