//
//
// Author : huziang
// this is a mongodb engine test file
// test function:
//   insert_one
//   insert_many
//   find_one
//   find_many

#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <gtest/gtest.h>
#include "MongodbEngine.h"
#include "DataParse.h"
using namespace std;

#define ASSERT_DOUBLE(a, b) ASSERT_TRUE(abs(a - b) < 0.000001)
#define TEST_NUM 300

DataEngine *db = NULL;
ifstream fin;
TSMarketDataField *pDepthMarketData = NULL;

mongocxx::client conn;
mongocxx::collection collection;

void scanTS(TSMarketDataField *pDepthMarketData) {
  fin >> pDepthMarketData->TradingDay
     >> pDepthMarketData->InstrumentID
     >> pDepthMarketData->LastPrice
     >> pDepthMarketData->PreSettlementPrice
     >> pDepthMarketData->PreClosePrice
     >> pDepthMarketData->PreOpenInterest
     >> pDepthMarketData->OpenPrice
     >> pDepthMarketData->HighestPrice
     >> pDepthMarketData->LowestPrice
     >> pDepthMarketData->Volume
     >> pDepthMarketData->Turnover
     >> pDepthMarketData->OpenInterest
     >> pDepthMarketData->ClosePrice
     >> pDepthMarketData->SettlementPrice
     >> pDepthMarketData->UpperLimitPrice
     >> pDepthMarketData->LowerLimitPrice
     >> pDepthMarketData->UpdateTime
     >> pDepthMarketData->UpdateMillisec
     >> pDepthMarketData->BidPrice1
     >> pDepthMarketData->BidVolume1
     >> pDepthMarketData->AskPrice1
     >> pDepthMarketData->AskVolume1;
}

void printTS(TSMarketDataField *pDepthMarketData) {
  cout << pDepthMarketData->TradingDay << endl
       << pDepthMarketData->InstrumentID << endl
       << pDepthMarketData->LastPrice << endl
       << pDepthMarketData->PreSettlementPrice << endl
       << pDepthMarketData->PreClosePrice << endl
       << pDepthMarketData->PreOpenInterest << endl
       << pDepthMarketData->OpenPrice << endl
       << pDepthMarketData->HighestPrice << endl
       << pDepthMarketData->LowestPrice << endl
       << pDepthMarketData->Volume << endl
       << pDepthMarketData->Turnover << endl
       << pDepthMarketData->OpenInterest << endl
       << pDepthMarketData->ClosePrice << endl
       << pDepthMarketData->SettlementPrice << endl
       << pDepthMarketData->UpperLimitPrice << endl
       << pDepthMarketData->LowerLimitPrice << endl
       << pDepthMarketData->UpdateTime << endl
       << pDepthMarketData->UpdateMillisec << endl
       << pDepthMarketData->BidPrice1 << endl
       << pDepthMarketData->BidVolume1 << endl
       << pDepthMarketData->AskPrice1 << endl
       << pDepthMarketData->AskVolume1 << endl;
}

class TestMongodbEngine : public testing::Test
{
 public:
  static void SetUpTestCase()
  {
    db = MongodbEngine::getInstance();
    db->init();
    db->setLibname("test");
    db->setTablename("TSMarketDataField");

    pDepthMarketData = new TSMarketDataField();
    conn = mongocxx::client(mongocxx::uri("mongodb://localhost:27017"));
    collection = conn["test"]["TSMarketDataField"];
  }
  static void TearDownTestCase()
  {
  }
  virtual void SetUp()
  {
    fin.open("../test/data.csv");
    if (!fin) {
      perror("no file");
      exit(1);
    }
  }
  virtual void TearDown()
  {
    collection.delete_many({});
    fin.close();
  }
};

// test insert_one:
//    whether a TSMarketDataField instance is same
//  when it insert and take out
TEST_F(TestMongodbEngine, insert_one)
{
  map<string, string> md;
  vector<KeyValue> cond;
  for (int i = 0; i < TEST_NUM; i++) {
    md.clear();
    scanTS(pDepthMarketData);
    parseFrom(md, *pDepthMarketData);

    // insert one
    int num = db->insert_one(md);

    // test return value
    ASSERT_TRUE(num == 1);

    // test string value
    ASSERT_STREQ(pDepthMarketData->TradingDay, md["TradingDay"].c_str());
    ASSERT_STREQ(pDepthMarketData->InstrumentID, md["InstrumentID"].c_str());
    ASSERT_STREQ(pDepthMarketData->UpdateTime, md["UpdateTime"].c_str());

    // test double value
    double d;
    sscanf(md["LastPrice"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->LastPrice);
    sscanf(md["PreSettlementPrice"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->PreSettlementPrice);
    sscanf(md["PreClosePrice"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->PreClosePrice);
    sscanf(md["PreOpenInterest"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->PreOpenInterest);
    sscanf(md["OpenPrice"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->OpenPrice);
    sscanf(md["HighestPrice"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->HighestPrice);
    sscanf(md["LowestPrice"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->LowestPrice);
    sscanf(md["Turnover"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->Turnover);
    sscanf(md["OpenInterest"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->OpenInterest);
    sscanf(md["ClosePrice"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->ClosePrice);
    sscanf(md["SettlementPrice"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->SettlementPrice);
    sscanf(md["UpperLimitPrice"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->UpperLimitPrice);
    sscanf(md["LowerLimitPrice"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->LowerLimitPrice);
    sscanf(md["BidPrice1"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->BidPrice1);
    sscanf(md["AskPrice1"].c_str(), "%lf", &d);
    ASSERT_DOUBLE(d, pDepthMarketData->AskPrice1);

    // test int value
    int in;
    sscanf(md["Volume"].c_str(), "%d", &in);
    ASSERT_EQ(in, pDepthMarketData->Volume);
    sscanf(md["UpdateMillisec"].c_str(), "%d", &in);
    ASSERT_EQ(in, pDepthMarketData->UpdateMillisec);
    sscanf(md["BidVolume1"].c_str(), "%d", &in);
    ASSERT_EQ(in, pDepthMarketData->BidVolume1);
    sscanf(md["AskVolume1"].c_str(), "%d", &in);
    ASSERT_EQ(in, pDepthMarketData->AskVolume1);

    db->delete_many(cond);
  }
}

// test insert_many:
//    whether inserted number is same when
//  many data insert
TEST_F(TestMongodbEngine, insert_many)
{
  map<string, string> md;
  vector<map<string, string>> mds;
  vector<KeyValue> cond;
  map<string, int> mapIDtimes;

  for (int i = 0; i < TEST_NUM; i++){
    md.clear();
    scanTS(pDepthMarketData);

    if (mapIDtimes.find(pDepthMarketData->InstrumentID) == mapIDtimes.end()) {
      mapIDtimes[pDepthMarketData->InstrumentID] = 0;
    }
    mapIDtimes[pDepthMarketData->InstrumentID]++;

    parseFrom(md, *pDepthMarketData);
    mds.push_back(md);
  }

  // insert many
  int num = db->insert_many(mds);

  // test return value
  ASSERT_TRUE(num == TEST_NUM);

  // test find IDs' number
  for (auto &kv : mapIDtimes) {
    mds.clear();
    ASSERT_EQ(kv.second, db->find_many(mds, cond, kv.first.c_str()));
  }
}

// test find_one, set_index:
//    find the first one
TEST_F(TestMongodbEngine, find_one)
{
  int firstvolumn = 0;
  map<string, string> md;
  vector<map<string, string>> mds;
  vector<KeyValue> cond;

  scanTS(pDepthMarketData);
  firstvolumn = pDepthMarketData->Volume;
  parseFrom(md, *pDepthMarketData);
  mds.push_back(md);
  for (int i = 1; i < TEST_NUM; i++){
    md.clear();
    scanTS(pDepthMarketData);
    parseFrom(md, *pDepthMarketData);
    mds.push_back(md);
  }

  // insert many
  db->insert_many(mds);

  // find many
  md.clear();
  cond.clear();
  int num = db->find_one(md, cond);

  // test return value
  ASSERT_EQ(num, 1);

  // test whether the find volume is the first volume
  ASSERT_EQ(firstvolumn, atoi(md["Volume"].c_str()));
}

// test find_many:
//    find some stock between one time and other time
TEST_F(TestMongodbEngine, find_many)
{
  char givenID[20] = "l1805";
  int sumnum = 0;
  long actvolume = 0;
  string begintime = "14:36:58";
  string endtime = "14:41:05";
  map<string, string> md;
  vector<map<string, string>> mds;
  vector<KeyValue> cond;

  for (int i = 0; i < TEST_NUM; i++){
    md.clear();

    do {
      scanTS(pDepthMarketData);
    } while (strcmp(pDepthMarketData->InstrumentID, givenID) != 0);

    parseFrom(md, *pDepthMarketData);
    string time(pDepthMarketData->UpdateTime);
    if (time >= begintime && time <= endtime) {
      sumnum++;
      actvolume += pDepthMarketData->Volume;
    }
    mds.push_back(md);
  }

  // insert many
  db->insert_many(mds);

  // find many
  mds.clear();
  cond.clear();
  cond.push_back(KeyValue("UpdateTime", begintime, endtime));
  int num = db->find_many(mds, cond, givenID);

  // test return value
  ASSERT_EQ(num, sumnum);

  // test find time
  long testvolumn = 0;
  for (auto &md : mds) {
    string testtime = md["UpdateTime"];
    // cerr << "Update time: " << testtime << endl;
    ASSERT_LE(begintime, testtime);
    ASSERT_GE(endtime, testtime);
    testvolumn += atoi(md["Volume"].c_str());
  }

  // test some sum value
  ASSERT_EQ(actvolume, testvolumn);
}

// !!!!!!!!!! undefined
// test set_index:
//    set descending index, test whether can I find bigest one,
//  and then set ascending index, test whether can I find smallest one
TEST_F(TestMongodbEngine, set_index)
{
  // int minvolume = 2147483647;
  // int maxvolume = 0;
  // map<string, string> md;
  // vector<map<string, string>> mds;
  // vector<KeyValue> cond;

  // for (int i = 0; i < TEST_NUM; i++){
  //   md.clear();
  //   scanTS(pDepthMarketData);
  //   if (pDepthMarketData->Volume == 0) {
  //     continue;
  //   }
  //   minvolume = min(minvolume, pDepthMarketData->Volume);
  //   maxvolume = max(maxvolume, pDepthMarketData->Volume);
  //   parseFrom(md, *pDepthMarketData);
  //   mds.push_back(md);
  // }

  // // insert many
  // db->insert_many(mds);

  // // set index and find one
  // md.clear();
  // cond.clear();
  // cond.push_back(KeyValue("Volume", "-1", "2147483647"));
  // db->set_index("Volume", true);
  // db->find_one(md, cond);

  // // test whether the find volume is the bigest volume
  // ASSERT_EQ(minvolume, atoi(md["Volume"].c_str()));

  // // set index and find one
  // md.clear();
  // cond.clear();
  // cond.push_back(KeyValue("Volume", "-1", "2147483647"));
  // db->set_index("Volume", false);
  // db->find_one(md, cond);

  // // test whether the find volume is the smallest volume
  // ASSERT_EQ(maxvolume, atoi(md["Volume"].c_str()));
}


int main(int argc,char *argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}