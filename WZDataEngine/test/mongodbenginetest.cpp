//
//
// Author : huziang
// this is a mongodb engine test file
// test function:
//   insert_one
//   insert_many
//   update_one
//   update_many
//   find_one
//   find_many

#include <iostream>
#include <map>
#include <gtest/gtest.h>
#include "mongodbengine.h"
#include <bsoncxx/json.hpp>
#include <mongocxx/instance.hpp>
#include "dataparse.h"
#include <vector>
using namespace std;

DataEngine *db = NULL;
FILE *fp = NULL;
TSMarketDataField *pDepthMarketData = NULL;

mongocxx::client conn;
mongocxx::collection collection;

map<string, string> ts;
//map<KeyValue> kv;

void find_all() {

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

    conn = mongocxx::client(mongocxx::uri("mongodb://localhost:27017"));
    collection = conn["test"]["TSMarketDataField"];
    // fp = fopen("../test/data.csv", "r");
    // if (fp == NULL) {
    //   perror("no file");
    //   exit(1);
    // }
    pDepthMarketData = new TSMarketDataField();
  }
  static void TearDownTestCase()
  {
    collection.delete_many({});
    fclose(fp);
  }
  virtual void SetUp()
  {
  }
  virtual void TearDown()
  {
    delete pDepthMarketData;
    ts.clear();
    //kv.clear();
  }
};

TEST_F(TestMongodbEngine, insert_one)
{
  //fread(pDepthMarketData, sizeof(TSMarketDataField), 1, fp);
  for (int i=0; i<100; i++){
    //memset(pDepthMarketData, 0, sizeof(TSMarketDataField));
    // pDepthMarketData->Volume = i;
    // parseFrom(ts, *pDepthMarketData);
    //db->insert_one(ts);
  }
}

/*TEST_F(TestMongodbEngine, insert_many)
{
  //fread(pDepthMarketData, sizeof(TSMarketDataField), 1, fp);
  TSMarketDataField temp[100];
  vector<map<string, string>> my_map;
  for (int i=100; i<200; i++){
    memset(&temp[i], 0, sizeof(temp[i]));
    temp[i].Volume = i;
    parseFrom(ts, temp[i]);
    my_map.push_back(ts);
  }
  db->insert_many(my_map);
}*/

/*TEST_F(TestMongodbEngine, update_many)
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
}*/

/*TEST_F(TestMongodbEngine, find_many)
{
  //fread(pDepthMarketData, sizeof(TSMarketDataField), 1, fp);

  vector<map<string, string>> my_map;

  vector<KeyValue> my_vector;
  KeyValue my_key_value_update;
  my_key_value_update.key = "LastPrice";
  my_key_value_update.minvalue = "1";
  my_vector.push_back(my_key_value_update);

  db->find_many(my_map, my_vector);
}*/

int main(int argc,char *argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}