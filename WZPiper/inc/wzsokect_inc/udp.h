//
// Author: luojunbin
//

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

#include "iniparser.h"
#include "wzsokect_inc/wzsocket.h"

class UdpSocket: public WZSocket
{
public:
  UdpSocket();
  ~UdpSocket();

  int init(char file_path[256], int piperMode);
  int Bind();
  int Connect();
private:

};

#endif
