//
// Author: luojunbin
// 

#include "tcp.h"
#include "wzpiper.hpp"
#include "gtest/gtest.h"
#include <pthread.h>
#include <bits/stdc++.h>

WZPiper<TcpSocket> pip;
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

class TcpTest : public ::testing::Test{
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

void *server_thread(void *ptr)
{
 	int dest = pip.init("../doc/config.ini", WZ_PIPER_SERVER, WZ_PIPER_BLOCK);
	pip.Recv(recv_frame);
    return 0;
}

void *client_thread(void *ptr)
{
 	int dest = pip.init("../doc/config.ini", WZ_PIPER_CLIENT, WZ_PIPER_BLOCK);
	send_frame.dest = dest;
	pip.Send(send_frame);
	return 0;
}

TEST_F(TcpTest, Test){
	pthread_t thread_1 = 1;
    int ret = pthread_create(&thread_1, NULL, server_thread, NULL);
    pthread_t thread_2 = 2;
    ret = pthread_create(&thread_2, NULL, client_thread, NULL);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
}

int main(int argc, char* argv[]){
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}