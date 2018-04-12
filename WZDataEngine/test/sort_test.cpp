#include <iostream>
#include <cmath>
#include <map>
#include "dataparse.h"
#include "mongodbengine.h"

using namespace std;

int main()
{
	DataEngine *db = NULL;
	TSMarketDataField pDepthMarketData;
	map<string, string> ts;
	db = MongodbEngine::getInstance();
    db->init();
    db->setLibname("test");
    db->setTablename("luo_test");

    for (int i=0; i<10; i++){
	  	ts.clear();
	    memset(&pDepthMarketData, 0, sizeof(TSMarketDataField));
	    pDepthMarketData.Volume = i;
	    parseFrom(ts, pDepthMarketData);
	    db->insert_one(ts);
	}

	map<string, string> my_map;
	db->get_max_item(my_map, "Volume");
	map<string, string>::iterator it = my_map.find("Volume");
	if (it != my_map.end())
	{
		cout << atoi(it->second.c_str()) << endl;
	}
	return 0;
}
