#ifndef SOCKET_H_
#define SOCKET_H_

#include <string>
#include "wzadapter/frame.h"
#include "util/iniparser.h"

using std::string;

class Socket {
public:
	Socket(){};
	virtual ~Socket(){};
	virtual int readBuffer(Frame &){};
	virtual int writeBuffer(Frame &){};
};
#endif
