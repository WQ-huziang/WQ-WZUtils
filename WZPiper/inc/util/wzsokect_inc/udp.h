//
// Author: luojunbin
//

#include "wzsocket.h"
#include "iniparser.h"

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

class UdpSocket: public WZSocket
{
public:
	int init(char file_path[256]);
	int wzBind();
	int wzConnect();
private:

};

#endif
