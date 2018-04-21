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

using std::cout;
using std::endl;

#define TESTCASE

#ifdef TESTCASE
#define PRT(...) printf(__VA_ARGS__)
#endif

static double circlePerUs = 0.0;

inline unsigned long long getTimeByTSC(){
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

inline double getDurationByTSC(unsigned long long &start, unsigned long long &end){
	return ((double)end - (double)start) / (circlePerUs) ;
}

inline unsigned long long getInterval(int cycle){
	unsigned long long tsc_start = getTimeByTSC();
	usleep(cycle);
	unsigned long long tsc_end = getTimeByTSC();
	// cout << " (int) interval:" << cycle << " tsc_start:" << tsc_start << " tsc_end:" << tsc_end << " tsc duration:" << tsc_end - tsc_start <<endl;
	unsigned long long tsc_interval = (tsc_end - tsc_start) / ((unsigned long long) cycle);
	// TODO: tsc_interval = 

	return tsc_interval;
}

inline unsigned long long getFrequency(int me_cycle,int in_cycle){

	unsigned long long mean;
	mean = getInterval(in_cycle);
	double meanDou = (double) mean;
	for (int i =0; i < me_cycle; i++)
		meanDou = (meanDou + ((double) getInterval(in_cycle))) / ((double) 2);

	double freG = ((double) mean / (double) 1000);

	cout << "mean:" << mean << "circle/us " << "fre:" << freG << "GHz\n";
	return mean;
}

#endif // TSCTIMER_HPP_