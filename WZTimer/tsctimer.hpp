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

int64_t readTSC(void) {
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

#endif // TSCTIMER_HPP_