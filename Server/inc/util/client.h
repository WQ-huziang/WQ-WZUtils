#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include "wzadapter/frame.h"
#include "util/iniparser.h"

using std::string;


class Client
{
public:
	Client(){};
	virtual ~Client(){};
	virtual void parseConfigFile(char configFilePath[256]){};
	virtual void closeConnection(){};
	virtual bool isConnected(){};
	virtual int readBuffer(Frame s){};
	virtual int writeBuffer(Frame s){};
};
#endif
