
#include "udp.h"
#include "wzclient.hpp"
#include <iostream>

using namespace std;

int main()
{
	WZClient<UdpSocket> pip;
	int servfd = pip.init("../doc/config.ini");
	pip.wzConnect();

	Frame my_frame;
	memset(&my_frame, 0, sizeof(my_frame));
	my_frame.dest = servfd;
	my_frame.source = 1;
	cout << my_frame.dest << endl;
	pip.wzSend(my_frame);

	while(1)
	{

	}

	return 0;
}