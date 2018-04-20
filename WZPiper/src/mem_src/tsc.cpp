#include "tsctimer.hpp"

#ifndef IN_CYCLE
#define IN_CYCLE 30000
#endif

#ifndef ME_CYCLE
#define ME_CYCLE 10000
#endif

int main(){
	double fre = getFrequency(ME_CYCLE, IN_CYCLE);
	return 0;
}