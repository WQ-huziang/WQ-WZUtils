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

#ifndef DataType
#define DataType Frame
#endif

#ifndef QueueSize
#define QueueSize 1024
#endif

#ifndef MaxReaderSize
#define MaxReaderSize 4
#endif

/***************************************************************************
Description: save pipeMode and readerId will be used when remove the reader
****************************************************************************/
struct SignalMapData
{
  int pipeMode;
  int readerId;
};

static std::map<int, SignalMapData> signalMap;

static QueueManager<QueueDataType, DataQueueSize, MaxReaderSize> *signal_queue_manager;

/***************************************************************************
Description: catch the signal and handle it
****************************************************************************/
class SignalHandler
{
public:

	SignalHandler();
	~SignalHandler();

    /*************************************************
    Function: listenSignal
    Description: call the signal fuction to listen the SIG of current proccess
    InputParameter:
       signalType: signal type (SIGABRT or SIGSEGV or SIGTERM or..).
    Return: 1 if detach succeed, 0 if failed
    *************************************************/
	int listenSignal(int signalType);

	static int initQueueManager();

    /*************************************************
    Function: handleSignal
    Description: handle the signal according to signal type
    InputParameter:
       signalType: signal type (SIGABRT or SIGSEGV or SIGTERM or..).
    Return: none
    *************************************************/
	static int handleSignal(int signalType);

	static int addToMap(int pid,int pipeMode,int readerId);

	static int releaseReader(QueueManager<DataType, QueueSize, MaxReaderSize> *queue_manager, int reader_id, int piperMode);
	
private:

};


SignalHandler::SignalHandler(){
	
}

SignalHandler::~SignalHandler(){

}

bool MemEngine<QueueDataType, DataQueueSize, MaxReaderSize>::createMemory(const int &m_key, const int &m_size, const int &m_flag, int &m_shmid, char* & m_memory_addr) {
  // call shmget and use return value to initialize shared memory address pointer
  m_shmid = shmget(m_key, m_size, m_flag);

  if ( m_shmid == -1 ) {
    sprintf(logger_buf, "shared memory create failed");
    logger -> Error(logger_buf);
    return false;
  }

  // call shmat to attach shared memory
  m_memory_addr = reinterpret_cast<char*>(shmat(m_shmid, NULL, 0));

  if ( m_memory_addr == (char*)SHM_FAILED ) {
    sprintf(logger_buf, "shared memory m_memory_addr attach failed");
    logger -> Error(logger_buf);
    return false;
  }

  sprintf(logger_buf, "shared memory create succeed, key = %d, size = %d, shmid = %d", m_key, m_size, m_shmid);
  logger -> Info(logger_buf);

  return true;
}

int SignalHandler::initQueueManager(){

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

int SignalHandler::addToMap(int pid,int pipeMode,int readerId){
	SignalMapData sd;
	sd.pipeMode = pipeMode;
	sd.readerId = readerId;
	signalMap[pid] = sd;
}

int SignalHandler::releaseReader(QueueManager<DataType, QueueSize, MaxReaderSize> *queue_manager, int reader_id, int piperMode){
  if(piperMode == 0) { // server
    // hang reader of the frame_req_queue and release the reader_id
    queue_manager -> frame_req_queue.removeReader(reader_id);
    printf("call releaseReader()\n");
  }
  else if(piperMode == 1){ // client
    // hang reader of the frame_rec_queue and release the reader_id
    queue_manager -> frame_rec_queue.removeReader(reader_id);
    printf("call releaseReader()\n");
  }
}
#endif //SIGNALHANDLER_HPP_