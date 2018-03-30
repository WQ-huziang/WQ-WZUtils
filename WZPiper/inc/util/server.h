#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include "wzadapter/frame.h"
#include "util/iniparser.h"

using std::string;

class Server {
public:
	Server(){};
	virtual ~Server(){};
	virtual void parseConfigFile(char configFilePath[256]){};
	virtual int readBuffer(Frame s){};
	virtual int writeBuffer(Frame s){};
};
#endif
