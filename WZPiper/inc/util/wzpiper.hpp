/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: huangxiaolin, luoqingming
Description: WZPiper class template is the interface of the piper
Date: 2018-04-09
***************************************************************************/
#ifndef WZPIPER_H_
#define WZPIPER_H_

#include "frame.h"

/***************************************************************************
Description: single writer and multiple reader lockless queue with atomic,
    typename T:the type of IPC  (tcp,udp,memengine)
****************************************************************************/
template <typename T>
class WZPiper{
	public:
		/************************************************* 
		Function: WZPiper
		Description: Constructor, calls the constructor of T
		InputParameter: 
			piperMode: the flag to mark server or client, 
				0 or WZ_PIPER_SERVER as server,
				1 or WZ_PIPER_CLIENT as client
		Return: none
		*************************************************/
		WZPiper(int piperMode);

		/************************************************* 
		Function: ~WZPiper
		Description: Destructor, delete T
		InputParameter: none
		Return: none
		*************************************************/		
		~WZPiper();

		/************************************************* 
		Function: init
		Description: read configure file and init the IPC according to server_client_flag
		InputParameter: 
			file_path: the path of the configure file
		Return: positive if init succeed, -1 if failed
		*************************************************/
		int init(char file_path[256]);

		/************************************************* 
		Function: wzBind
		Description: do what???
		InputParameter: none
		Return: positive if bind succeed, -1 if failed
		*************************************************/		
		int wzBind();

		/************************************************* 
		Function: wzConnect
		Description: do what???
		InputParameter: none
		Return: positive if connect succeed, -1 if failed
		*************************************************/
		int wzConnect();

		/************************************************* 
		Function: wzRecv
		Description: read configure file and init the IPC according to server_client_flag
		InputParameter: receive a frame from piper
			frame: read receive datum from piper to frame
		Return: positive if receive succeed, 0 if failed
		*************************************************/
		int wzRecv(Frame &frame);

		/************************************************* 
		Function: wzSend
		Description: read configure file and init the IPC according to server_client_flag
		InputParameter: send a frame to piper
			frame: the datum to send to piper
		Return: positive if send succeed, 0 if failed
		*************************************************/
		int wzSend(Frame &frame);

	private:

		T* t; // pointer point to the IPC
};


template<typename T>
WZPiper<T>::WZPiper(int server_client_flag){
	t = new T(server_client_flag);
}


template<typename T>
WZPiper<T>::~WZPiper(){
	delete t;
}

template<typename T>
int WZPiper<T>::init(char file_path[256]){
	return t->init(file_path);
}

template<typename T>
int WZPiper<T>::wzBind(){
	return t->wzBind();
}

template<typename T>
int WZPiper<T>::wzConnect(){
	return t->wzConnect();
}

template<typename T>
int WZPiper<T>::wzRecv(Frame &frame){
	return t->wzRecv(frame);
}

template<typename T>
int WZPiper<T>::wzSend(Frame &frame){
	return t->wzSend(frame);
}

///////////////////////////////////
// The mode of the piper 
// (server or client)
///////////////////////////////////
//server
#define WZ_PIPER_SERVER          0
//client 
#define WZ_PIPER_CLIENT          1
typedef short WZPiperMode;

#endif