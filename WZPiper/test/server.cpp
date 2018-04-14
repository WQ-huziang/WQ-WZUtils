//
// Author: luojunbin

#include <iostream>
#include "wzpiper.hpp"
#include "wzsocket_inc/udp.h"

using namespace std;

int main()
{
  WZPiper<UdpSocket> pip;
  pip.init("../doc/config.ini", WZ_PIPER_SERVER, WZ_PIPER_BLOCK);

  Frame my_frame;
  while(1)
  {
    int ret = pip.Recv(my_frame);
    if (ret > 0)
      cout << my_frame.source << endl;
    else {

    }
  }
  return 0;
}