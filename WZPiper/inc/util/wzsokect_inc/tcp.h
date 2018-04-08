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
	int init(char file_path[256]);
	int wzBind();
	int wzConnect();
private:

};

#endif
