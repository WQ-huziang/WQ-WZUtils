/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: luoqingming
Description: simple gtest for the module MemClient and MemServer
Date: 2018-04-2
***************************************************************************/

#include "wzmem_inc/memengine.hpp"
#include "wzpiper.hpp"
#include "logger.h"
#include "gtest/gtest.h"
#include <bits/stdc++.h>

#ifndef PRT
#define PRT(...) printf(__VA_ARGS__)
//#define PRT(...)
#endif

#ifndef DataType
#define DataType Frame
#endif

#ifndef QueueSize
#define QueueSize 2048
#endif

#ifndef MaxReaderSize
#define MaxReaderSize 2
#endif

Logger * logger;

class ServerClientTest : public ::testing::Test{
protected:

  WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> >  * memServer;
  WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> >  * memClient1;
  WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> >  * memClient2;

  virtual void SetUp(){
    // init all
    char file[] = "../doc/config.ini";
    logger -> ParseConfigInfo(file);

    memServer = new WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> > ();
    memServer -> init(file, WZ_PIPER_SERVER);

    memClient1 = new WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> > ();
    memClient1 -> init(file, WZ_PIPER_CLIENT);

    memClient2 = new WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> > ();
    memClient2 -> init(file, WZ_PIPER_CLIENT);
  }
  virtual void TearDown(){
    delete memServer;
    delete memClient1;
    delete memClient2;
  }
};

TEST_F(ServerClientTest, WriteAndReadWorks){

  // test write
  Frame sendFrame;
  for (int i = 0; i<QueueSize ; i++ ) {
    sendFrame.source = i;
    sendFrame.msg_type = i;
    sendFrame.error_id = WZ_ERROR_ID_SUCCESS;
    sendFrame.rtn_type = i;
    sendFrame.length = i;
    EXPECT_EQ(0 ,memServer -> Send(sendFrame));
    if(i<(QueueSize/2))   EXPECT_EQ(0,memClient1 -> Send(sendFrame));
    else  EXPECT_EQ(0,memClient2 -> Send(sendFrame));
  }

  // test queue full
  EXPECT_EQ(-1,memServer -> Send(sendFrame));
  EXPECT_EQ(-1,memClient1 -> Send(sendFrame));
  EXPECT_EQ(-1,memClient2 -> Send(sendFrame));

  // test Reader1
  Frame recvFrame;

  for (int i = 0; i<QueueSize ; i++ ) {
    EXPECT_EQ(sizeof(sendFrame),memClient1 -> Recv(recvFrame));
    EXPECT_EQ(i,recvFrame.source);
    EXPECT_EQ(i,recvFrame.msg_type );
    EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
    EXPECT_EQ(i,recvFrame.rtn_type);
    EXPECT_EQ(i,recvFrame.length);


    EXPECT_EQ(sizeof(sendFrame),memServer -> Recv(recvFrame));
    EXPECT_EQ(i,recvFrame.source);
    EXPECT_EQ(i,recvFrame.msg_type );
    EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
    EXPECT_EQ(i,recvFrame.rtn_type);
    EXPECT_EQ(i,recvFrame.length);
  }

  // test catch Writer
  EXPECT_EQ(-1,memClient1 -> Recv(recvFrame));
  EXPECT_EQ(QueueSize - 1,recvFrame.source);
  EXPECT_EQ(QueueSize - 1,recvFrame.msg_type );
  EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
  EXPECT_EQ(QueueSize - 1,recvFrame.rtn_type);
  EXPECT_EQ(QueueSize - 1,recvFrame.length);
  EXPECT_EQ(-1,memServer -> Send(sendFrame));

  // test Reader2
  for (int i = 0; i<QueueSize ; i++ ) {
    EXPECT_EQ(sizeof(sendFrame),memClient2 -> Recv(recvFrame));
    EXPECT_EQ(i,recvFrame.source);
    EXPECT_EQ(i,recvFrame.msg_type );
    EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
    EXPECT_EQ(i,recvFrame.rtn_type);
    EXPECT_EQ(i,recvFrame.length);
  }

  // test catch Writer
  EXPECT_EQ(-1,memClient2 -> Recv(recvFrame));
  EXPECT_EQ(QueueSize - 1,recvFrame.source);
  EXPECT_EQ(QueueSize - 1,recvFrame.msg_type );
  EXPECT_EQ(WZ_ERROR_ID_SUCCESS,recvFrame.error_id);
  EXPECT_EQ(QueueSize - 1,recvFrame.rtn_type);
  EXPECT_EQ(QueueSize - 1,recvFrame.length);

  // test write
  EXPECT_EQ(0,memServer -> Send(sendFrame));

}

int main(int argc, char* argv[]){
  logger = new Logger(argv[0]);
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}