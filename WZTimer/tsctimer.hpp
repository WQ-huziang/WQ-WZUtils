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
#include <iostream>
#include "timer.h"

#ifndef TIME_INTERVAL
#define TIME_INTERVAL 
#endif

using std::cout;
using std::endl;

unsigned long long getTimeByTSC(){
	#if defined(__GNUC__)
    #   if defined(__i386__)
        uint64_t x;
        __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
        return x;
    #   elif defined(__x86_64__)
        uint32_t hi, lo;
        __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
        return ((uint64_t)lo) | ((uint64_t)hi << 32);
    #   else
    #       error Unsupported architecture.
    #   endif
    #elif defined(_MSC_VER)
        __asm {
            return __rdtsc();
        }
    #else
    #   error Other compilers not supported...
    #endif
}



unsigned long long getDurationByTSC(unsigned long long &start, unsigned long long &end){
	return (end - start) /  TIME_INTERVAL;
}

unsigned long long getInterval(int cycle){
	long start = getTimeStamp();
	unsigned long long tsc_start = getTimeByTSC();
	for (int i = 0; i< cycle; i++ ) {
	}
	long end = getTimeStamp();
	unsigned long long tsc_end = getTimeByTSC();
	long interval = getDuration(start, end);
	cout << "interval by timer:" << interval << "tsc_start:" << tsc_start << "tsc_end:" << tsc_end << "tsc duration:" << tsc_end - tsc_start <<endl;
	
	unsigned long long tsc_interval = 0;
	// TODO: tsc_interval = 

	return tsc_interval;
}

#endif // TSCTIMER_HPP_