#include<bits/stdc++.h>
#include "signalhandler.hpp"

int main(){
	SignalHandler sh;
	sh.listenSignal(SIGINT);
	sh.listenSignal(SIGQUIT);
	sh.listenSignal(SIGTERM);
	while(1){
		sh.addToMap(getpid(),1,2);
		printf("going to sleep\n");
		sleep(1);
	}
	return 0;
}