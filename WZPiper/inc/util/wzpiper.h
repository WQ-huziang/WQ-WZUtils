#ifndef WZPIPER_H_
#define WZPIPER_H_

#include <iostream>
#include "frame.h"

template <class T>
class WZPiper{
	public:
		WZPiper(int server_client_flag);
		~WZPiper(){};

		bool init(char file_path[256]);
		bool wzBind();
		bool wzConnect();
		bool wzRecv(Frame &frame);
		bool wzSend(Frame &frame);

	private:
		T* t;
};

template<class T>
WZPiper<T>::WZPiper(int server_client_flag){
	t = new T(server_client_flag);
}

template<class T>
bool WZPiper<T>::init(char file_path[256]){
	return t->init(file_path);
}

template<class T>
bool WZPiper<T>::wzBind(){
	return t->wzBind();
}

template<class T>
bool WZPiper<T>::wzConnect(){
	return t->wzConnect();
}

template<class T>
bool WZPiper<T>::wzRecv(Frame &frame){
	return t->wzRecv(frame);
}

template<class T>
bool WZPiper<T>::wzSend(Frame &frame){
	return t->wzSend(frame);
}


#endif