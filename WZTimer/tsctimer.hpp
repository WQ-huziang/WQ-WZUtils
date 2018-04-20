/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: luoqingming
Description: get timestamp by TSC
Date: 2018-04-20
***************************************************************************/


#ifndef TSCTIMER_HPP_
#define TSCTIMER_HPP_

#include <unistd.h>
#include <sys/types.h>

#ifndef TIME_INTERVAL
#define TIME_INTERVAL 
#endif

unsigned long long getTimeByTSC(){
	unsigned int high, low;
	asm("cpuid");
	__asm__ __volatile__ ("rdtsc" : "=a" (low));
	__asm__ __volatile__ ("rdtsc" : "=d" (high));
	unsigned long long stamp;
	stamp = (((uint64_t) low) | ((uint64_t) high) << 32)
	return stamp;
}



unsigned long long getDurationByTSC(unsigned long long &start, unsigned long long &end){
	return (end - start) /  TIME_INTERVAL;
}

#endif // TSCTIMER_HPP_