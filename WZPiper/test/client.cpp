
#include "wzsocket_inc/udp.h"
#include "wzpiper.hpp"
#include <iostream>

using namespace std;

int main()
{
  WZPiper<UdpSocket> pip;
  int servfd = pip.init("../doc/config.ini", WZ_PIPER_CLIENT, WZ_PIPER_BLOCK);

  Frame my_frame;
  memset(&my_frame, 0, sizeof(my_frame));
  my_frame.dest = servfd;
  my_frame.source = 1;
  cout << my_frame.dest << endl;


  while(1)
  {
    pip.Send(my_frame);
    sleep(1);
  }

  return 0;
}