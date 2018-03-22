#include "Logger.h"
#include <iostream>
#include <string>
#include <sstream>
#include "iniparser.h"
using std::string;
using std::to_string;
using std::stringstream;

Logger::Logger(char *programname)
{
	google::InitGoogleLogging(programname);
}

Logger::~Logger()
{
	google::ShutdownGoogleLogging();
}

void Logger::ParseConfigInfo(char *configFilePath)
{
	CIni ini;
	ini.OpenFile(configFilePath,"r");
	/* Log */
	FLAGS_logtostderr = ini.GetInt("Logger","logtostderr");
	FLAGS_alsologtostderr = ini.GetInt("Logger","alsologtostderr");
	FLAGS_minloglevel = ini.GetInt("Logger","minloglevel");
	FLAGS_stderrthreshold = ini.GetInt("Logger","stderrthreshold");
	FLAGS_colorlogtostderr = ini.GetInt("Logger", "colorlogtostderr");
	FLAGS_v = ini.GetInt("Logger", "v");
	string buf = ini.GetStr("Logger", "log_dir");
	FLAGS_log_dir = buf;
}

void Logger::Debug(char *buffer)
{
	DLOG(INFO) << buffer;
}

void Logger::Info(char *buffer)
{
	 LOG(INFO) << buffer;
}

void Logger::Warn(char *buffer)
{
	LOG(WARNING) << buffer;
}

void Logger::Error(char *buffer)
{
	LOG(ERROR) << buffer;
}

void Logger::Fatal(char *buffer)
{
	LOG(FATAL) << buffer;
}

string tostr(double d){
	stringstream ss;
	ss << d;
	return ss.str();
}

string tostr(const char *c){
	string str;
	str = c;
	return str;
}

void Logger::Info(WZMarketDataField md){
	string info = "交易日 : " + tostr(md.TradingDay) + "\n" +
	 			  "合约代码 ：" + tostr(md.InstrumentID )+ "\n" +
	              "数量 : " + to_string(md.Volume) + "\n" +
	              "最新价 : " + tostr(md.LastPrice) + "\n" +
	              "上次结算价 : " + tostr(md.PreSettlementPrice) + "\n" +
	              "昨收盘 : " + tostr(md.PreClosePrice) + "\n" +
	              "昨持仓 : " + tostr(md.PreOpenInterest) + "\n" +
	              "今开盘 : " + tostr(md.OpenPrice) + "\n" +
	              "最高价 : " + tostr(md.HighestPrice) + "\n" +
	              "最低价 : "+ tostr(md.LowestPrice) + "\n" +
	              "成交金额 : " + tostr(md.Turnover) + "\n" +
	              "持仓量 : " + tostr(md.OpenInterest) + "\n" +
	              "今收盘 : " +tostr(md.ClosePrice) + "\n" +
	              "本次结算价 : " + tostr(md.SettlementPrice);
	LOG(INFO) << info;
 
}

void Logger::Info(WZInputOrderField inputOrder){
	string info = "合约代码 : " + tostr(inputOrder.InstrumentID) + "\n" + 
	              "价格 : "+ tostr(inputOrder.LimitPrice) + "\n" +
	              "数量 : " + to_string(inputOrder.Volume) + "\n" +
	              "买卖方向 : " + inputOrder.Direction + "\n" +
	              "开平标志 : " + inputOrder.OffsetFlag;
	LOG(INFO) << info;
}
             
void Logger::Info(WZOrderActionField orderAction){
	string info = "合约代码 : " + tostr(orderAction.InstrumentID) + "\n" +
	              "报单引用 : " + tostr(orderAction.OrderRef) + "\n" + + "\n" +
	              "报单操作标志 : " + tostr(orderAction.ActionFlag) + "\n" +
	              "价格 : " + tostr(orderAction.LimitPrice) + "\n" +
	              "数量变化 : " + to_string(orderAction.VolumeChange);
	LOG(INFO) << info;
}

void Logger::Info(WZRtnOrderField rtnOrder){
	string info = "合约代码 : " + tostr(rtnOrder.InstrumentID ) + "\n" +
	              "报单引用 : " + tostr(rtnOrder.OrderRef) + "\n" +
	              "价格 : " + tostr(rtnOrder.LimitPrice) + "\n" +
	              "今成交量 : " + to_string(rtnOrder.VolumeTraded) + "\n" +
	              "剩余数量 : " + to_string(rtnOrder.VolumeTotal) + "\n" +
	              "数量 : " + to_string(rtnOrder.VolumeTotalOriginal) + "\n" +
	              "买卖方向 : " + rtnOrder.Direction + "\n" +
	              "开平标志 : " + rtnOrder.OffsetFlag + "\n" +
	              "报单状态 : " + rtnOrder.OrderStatus;
	LOG(INFO) << info;
}

void Logger::Info(WZRtnTradeField rtnTrade){
	string info = "合约代码 : " + tostr(rtnTrade.InstrumentID) + "\n" +
	              "报单引用 : " + tostr(rtnTrade.OrderRef) + "\n" +
	              "价格 : " + tostr(rtnTrade.Price) + "\n" +
	              "数量 : " + to_string(rtnTrade.Volume) + "\n" +
	              "交易日 : " + tostr(rtnTrade.TradingDay) + "\n" +
	              "成交时间 : " + tostr(rtnTrade.TradeTime) + "\n" +
	              "买卖方向 : " + rtnTrade.Direction + "\n" +
	              "开平标志 : " + rtnTrade.OffsetFlag;
	LOG(INFO) << info;
}

// static Logger* Logger::Instance()
// {
// 	if(m_pInstance == NULL)
// 	{
// 		m_pInstance = new Logger();
// 	}
// 	return m_pInstance;
// }

// Logger* Logger::LogInstance = NULL;
