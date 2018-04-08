//
// Author: luojunbin

#include <iostream>
#include "wzserver.hpp"
#include "udp.h"

using namespace std;

int main()
{
	WZServer<UdpSocket> pip;
	pip.init("../doc/config.ini");
	pip.epollInit();
	pip.wzBind();

	Frame my_frame;
	while(1)
	{
		int ret = pip.wzRecv(my_frame);
		if (ret > 0)
			cout << my_frame.source << endl;
	}
	return 0;
}