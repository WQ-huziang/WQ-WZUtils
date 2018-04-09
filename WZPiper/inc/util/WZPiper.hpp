/***************************************************************************
Copyright(C) 2018, Wizard Quant
Author: luoqingming
Description: the top class template of the piper
Date: 2018-03-30
****************************************************************************/

#include "frame.h"
#include "wzsocket.h"

#ifndef WZPIPER_H_
#define WZPIPER_H_

template <class T>
class WZClient
{
public:
	WZClient();
	int init(char file_path[256]);
	int epollInit();
	int wzRecv(Frame &frame);
	int wzSend(Frame &frame);
	int wzConnect();

private:
	T* t;
};

template <class T>
WZClient<T>::WZClient()
{
	t = new T();
}

template <class T>
int WZClient<T>::init(char file_path[256])
{
	return t->init(file_path);
}

template <class T>
int WZClient<T>::epollInit()
{
	if (t->epollInit() == -1)
		return -1;
	return 0;
}

template <class T>
int WZClient<T>::wzRecv(Frame &frame)
{
	if (t->wzRecv(frame) == -1)
		return -1;
	return 0;
}

template <class T>
int WZClient<T>::wzSend(Frame &frame)
{
	if (t->wzSend(frame) == -1)
		return -1;
	return 0;
}

template <class T>
int WZClient<T>::wzConnect()
{
	return t->wzConnect();
}

#endif //WZPIPER_H_