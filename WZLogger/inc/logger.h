#ifndef LOGGER_H_
#define LOGGER_H_

#include <logging.h>
#include <wzdatastruct.h>
#include "frame.h"
#include "iniparser.h"
#include "transportstruct.h"

class Logger
{
public:
	Logger(char *programname);
	~Logger();
	void ParseConfigInfo(char *configFilePath);
	void Debug(char *buffer);
	void Info(char *buffer);
	void Warn(char *buffer);
	void Error(char *buffer);
	void Fatal(char *buffer);
	void Info(WZMarketDataField md);
	void Info(WZInputOrderField inputOrder);             
	void Info(WZOrderActionField orderAction);
	void Info(WZRtnOrderField rtnOrder);
	void Info(WZRtnTradeField rtnTrade);
	void Info(Frame f);
	void Info(TSMarketDataField md);
	void Info(TSInputOrderField inputorder);
	void Info(TSOrderActionField orderaction);
	void Info(TSRtnOrderField rtnOrder);
	void Info(TSRtnTradeField rtnTrade);
};

#endif
