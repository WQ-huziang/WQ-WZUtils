#include "tsctimer.hpp"

#ifndef IN_CYCLE
#define IN_CYCLE 10000
#endif

#ifndef ME_CYCLE
#define ME_CYCLE 10
#endif

int main(){
	unsigned long long fre = getFrequency(ME_CYCLE, IN_CYCLE);
	return 0;
}