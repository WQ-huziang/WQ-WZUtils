/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: huangxiaolin, luoqingming
Description:class template SignalHandler: Monitor the signal from OS when , 
Date: 2018-04-16
***************************************************************************/
#ifndef SIGNALHANDLER_HPP_
#define SIGNALHANDLER_HPP_
#include <csignal>
#include <unistd.h>
#include "memengine.hpp"

class SignalHandler
{
public:
	SignalHandler();
	~SignalHandler();
	int listenSignal(int signalType);
	static void handleSignal(int signalType);
private:

};

SignalHandler::SignalHandler(){
	
}

SignalHandler::~SignalHandler(){

}

int SignalHandler::listenSignal(int signalType){
	if(signal(signalType,handleSignal) == SIG_ERR){
		return -1;
	}
	return 0;
}

void SignalHandler::handleSignal(int signalType){
	switch (signalType){
		case SIGINT:
			printf("Interrupt signal %d(SIGINT) received\n", signalType);
			break;
		case SIGFPE:
			printf("Interrupt signal %d(SIGFPE) received\n", signalType);
			break;
		case SIGILL:
			printf("Interrupt signal %d(SIGILL) received\n", signalType);
			break;
		case SIGABRT:
			printf("Interrupt signal %d(SIGABRT) received\n", signalType);
			break;
		case SIGKILL:
			printf("Interrupt signal %d(SIGKILL) received\n", signalType);
			break;
		case SIGTERM:
			printf("Interrupt signal %d(SIGTERM) received\n", signalType);
			break;
		case SIGSEGV:
			printf("Interrupt signal %d(SIGSEGV) received\n", signalType);
			break;
		default:
			printf("Unkown signal\n");
			break;
	}
}

#endif //SIGNALHANDLER_HPP_