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
#include <cmath>
#include <map>
#include <gtest/gtest.h>
#include "mongodbengine.h"
#include "dataparse.h"
using namespace std;

#define ASSERT_DOUBLE(a, b) ASSERT_TRUE(abs(a - b) < 0.00001)

DataEngine *db = NULL;
FILE *fp = NULL;
TSMarketDataField *pDepthMarketData = NULL;

mongocxx::client conn;
mongocxx::collection collection;

map<string, string> ts;

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
    collection.delete_many({});
  }
  static void TearDownTestCase()
  {
    collection.delete_many({});
    fclose(fp);
  }
  virtual void SetUp()
  {
    fp = fopen("../test/data.csv", "r");
    if (fp == NULL) {
      perror("no file");
      exit(1);
    }
  }
  virtual void TearDown()
  {
    //delete pDepthMarketData;
    ts.clear();
  }
};

TEST_F(TestMongodbEngine, insert_one)
{
  //fread(pDepthMarketData, sizeof(TSMarketDataField), 1, fp);
  for (int i=0; i<10; i++){
    ts.clear();
    memset(pDepthMarketData, 0, sizeof(TSMarketDataField));
    pDepthMarketData->Volume = i;
    parseFrom(ts, *pDepthMarketData);
    db->insert_one(ts);
  }
}

TEST_F(TestMongodbEngine, insert_many)
{
  //fread(pDepthMarketData, sizeof(TSMarketDataField), 1, fp);
  TSMarketDataField temp[100];
  vector<map<string, string>> my_map;
  for (int i=0; i<10; i++){
    ts.clear();
    memset(&temp[i], 0, sizeof(temp[i]));
    temp[i].Volume = i+10;
    parseFrom(ts, temp[i]);
    my_map.push_back(ts);
  }
  db->insert_many(my_map);
}

TEST_F(TestMongodbEngine, update_many)
{
  //fread(pDepthMarketData, sizeof(TSMarketDataField), 1, fp);
  KeyValue my_key_value;
  my_key_value.key = "LastPrice";
  my_key_value.minvalue = "0";

  vector<KeyValue> my_vector;
  KeyValue my_key_value_update;
  my_key_value_update.key = "LastPrice";
  my_key_value_update.minvalue = "1";
  my_vector.push_back(my_key_value_update);

  db->update_many(my_key_value, my_vector);
}

TEST_F(TestMongodbEngine, find_many)
{
  //fread(pDepthMarketData, sizeof(TSMarketDataField), 1, fp);

  vector<map<string, string>> my_map;

  vector<KeyValue> my_vector;
  KeyValue my_key_value_update;
  my_key_value_update.key = "LastPrice";
  my_key_value_update.minvalue = "1";
  my_vector.push_back(my_key_value_update);

  db->find_many(my_map, my_vector);

  for (auto &it : my_map) {
    for (auto &map_it : it){
      cout << map_it.first << " " << map_it.second << endl;
    }
  }
}


int main(int argc,char *argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}