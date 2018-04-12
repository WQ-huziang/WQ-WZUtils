//
// Author: luojunbin
//

#include "wzsocket.h"
#include "iniparser.h"

#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

class TcpSocket: public WZSocket
{
public:
	TcpSocket();
	~TcpSocket();
	int init(char file_path[256], int piperMode);
	int Bind();
	int Connect();
private:

};

#endif
