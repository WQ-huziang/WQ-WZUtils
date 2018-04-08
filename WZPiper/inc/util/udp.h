//
// Author: luojunbin
//

#include "util/wzsocket.h"
#include "util/iniparse.h"

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