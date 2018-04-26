/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: luoqingming
Description: simple gtest for the module MemQueue
Date: 2018-03-30
***************************************************************************/
#include <benchmark/benchmark.h>
#include <pthread.h>
#include "wzmem_inc/memqueue.hpp"
#include "gtest/gtest.h"

#ifndef PRT
//#define PRT(...) printf(__VA_ARGS__)
#define PRT(...)
#endif

bool temp = 0;

MemQueue<int,1024,4> qI;
int intReader1;
int contain = 1;


void init(){
	intReader1 = qI.addReader();
}

static void BM_PUSH(benchmark::State& state)
{
	if (temp ==0)
	{
		init();
		temp = 1;
	}
	for (auto _ : state) {

		qI.push(intReader1);
		qI.push(intReader1);
		state.PauseTiming();
		qI.pop(contain,intReader1);
		qI.pop(contain,intReader1);
		state.ResumeTiming();
	}
}

BENCHMARK(BM_PUSH);

static void BM_POP(benchmark::State& state)
{
	if (temp ==0)
	{
		init();
		temp = 1;
	}
	for (auto _ : state) {
		state.PauseTiming();
		qI.push(contain);
		qI.push(contain);
		state.ResumeTiming();
		qI.pop(contain, intReader1);
	}
}


BENCHMARK(BM_POP);

BENCHMARK_MAIN();