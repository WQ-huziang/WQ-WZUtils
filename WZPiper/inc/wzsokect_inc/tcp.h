//
// Author: luojunbin
//

#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

#include "iniparser.h"
#include "wzsokect_inc/wzsocket.h"

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
