#include <iostream>
#include <MongodbEngine.h>
#include <map>
#include <vector>
#include "DataParse.h"
#include "transportstruct.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

DataEngine *db = NULL;
FILE *fp = NULL;
TSMarketDataField *pDepthMarketData = NULL;

WZRtnOrderField my_data_1;

map<string, string> ts;
map<string, string> ts2;
vector<map<string, string>> dataset;
vector<KeyValue> filter;

bool ione(){
  map<string, string>::iterator it1 = ts.begin();
  pDepthMarketData = new TSMarketDataField();
    memset(pDepthMarketData, 0, sizeof(pDepthMarketData));
  for(int i=0; i<100; i++){
    cout << i << endl;
    pDepthMarketData->Volume = i;
    parseFrom(ts, *pDepthMarketData);
    cout << pDepthMarketData->Volume << endl;
    for(it1 = ts.begin(); it1 != ts.end(); it1++){
    cout << it1->first << ": " << it1->second << endl;
  }
    db->insert_one(ts);
  }
  map<string, string> ts2;
  KeyValue kv;
  kv.key = "Volume";
  kv.minvalue = "50";
  kv.maxvalue = "";
  filter.push_back(kv);
  char *instrumentid = {"\0"};
  db->find_one(ts2, filter, instrumentid);

  map<string, string>::iterator it2 = ts2.begin();

  for(; it1 != ts.end(), it2 != ts2.end(); it1++, it2++){
    if(it1->second != it2->second){
      return false;
    }
  }
  return true;
}

int main()
{

  db = MongodbEngine::getInstance();
  db->init();
  db->setLibname("test");
  db->setTablename("WZRtnOrderField");
  // pDepthMarketData = new TSMarketDataField();

  // int i;
  // map<string, string> my_map;
  // for (i=0; i<10; i++)
  // {
  //  my_map.clear();
  //  memset(&my_data_1, 0, sizeof(my_data_1));
  //  my_data_1.VolumeTotal = i;
  //  parseFrom(my_map, my_data_1);
  //  db->insert_one(my_map);
  // }

  ione();

  // vector<map<string, string>> my_find_result;
  // vector<KeyValue> my_find_condition;
  // my_find_condition.clear();
  // KeyValue temp ;
  // temp.key = "VolumeTotal";
  // temp.minvalue = "0";
  // temp.maxvalue = "9";
  // my_find_condition.push_back(temp);
  // db->find_many(my_find_result, my_find_condition);
  // for (auto &it_1 :my_find_result)
  // {
  //  for (auto &it_2 : it_1)
  //  {
  //    cout << it_2.first << " " << it_2.second << endl;
  //  }

  // }

  return 0;
}
