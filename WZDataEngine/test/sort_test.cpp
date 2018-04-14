#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include "dataparse.h"
#include "mongodbengine.h"
#include <pthread.h>

using namespace std;

DataEngine *db = NULL;

void *thread_1(void *ptr)
{
	TSMarketDataField pDepthMarketData;
	map<string, string> ts;
	int i=0;
	for (;;){
	  	ts.clear();
	    memset(&pDepthMarketData, 0, sizeof(TSMarketDataField));
	    sprintf(pDepthMarketData.TradingDay, "%d", i);
	    sprintf(pDepthMarketData.UpdateTime, "%d", i);
	    strcpy(pDepthMarketData.InstrumentID, "al1805");
	    pDepthMarketData.Volume = i;
	    parseFrom(ts, pDepthMarketData);
	    int ret = db->insert_one(ts);
	    cout << "isReading: " << ret << endl;
	    i++;
	}
	return 0;
}

void *thread_2(void *ptr)
{
	map<string, string> ts;
	vector<KeyValue> condition;
	KeyValue cond_1;
	cond_1.key = "InstrumentID";
	cond_1.minvalue = "al1805";
	condition.push_back(cond_1);
	for (;;)
	{
		ts.clear();
		int ret = db->find_one(ts, condition);
		cout << "isWriting: " << ret << endl;
	}
	return 0;
}

int main()
{
	db = MongodbEngine::getInstance();
    db->init();
    db->setLibname("test");
    db->setTablename("luo_test");

    pthread_t p_1 = 1;
    int ret = pthread_create(&p_1, NULL, thread_1, NULL);
    pthread_t p_2 = 2;
    ret = pthread_create(&p_2, NULL, thread_2, NULL);


    pthread_join(p_1, NULL);
    pthread_join(p_2, NULL);
	
	return 0;
}