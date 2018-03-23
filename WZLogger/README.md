## WZLogger
- Objective:
- Member:

### logger.h
```c++
Logger() 			/* InitGoogleLogging */
~Logger()			/* ShutdownGoogleLogging */
Info()			
Warn()
Error()
Fatal()				/* after log the program exit */
Info(struct ...)
...
```

### Configure Logger 

#### step1:
	先新建log文件夹

#### step2:config.ini

```c++
[Logger]
logtostderr=0           #0-输出到文件，1输出到stderr
alsologtostderr=1       #1-同时将日志输出到文件和stderr
minloglevel=0           #限制输出到 stderr 的部分信息，包括此错误级别和更高错误级别的日志信息
stderrthreshold=2       #除了将日志输出到文件之外，还将此错误级别和更高错误级别的日志同时输出到 stderr
colorlogtostderr=0      #将输出到 stderr 上的错误日志显示相应的颜色
log_dir=../log             
```

#### step3:inc

```c++
/* copy  logger.h and logging.h to the folder inc */
logger.h
logging.h

/* if you use your own computer, you must copy glog to your inc */
```

#### step4:lib

```c++
/* copy liblogger.h to the folder lib */
liblogger.so
```

#### step5:CmakeList.txt

```c++
/* add the following instruction to the CMakeList.txt */
TARGET_LINK_LIBRARIES(yourexe logger iniparser glog)
```

### Instructions for use

#### declare and load config anywhere you want

```c++
Logger *logger = new Logger(argv[0]);
char ini[] = {"config.ini"};
logger->ParseConfigInfo(ini);
```

#### different operations

```c++
/* not need newline \n */

logger->Info(string)
logger->Debug(char *buffer);
logger->Info(char *buffer);
logger->Warn(char *buffer);
logger->Error(char *buffer);
logger->Fatal(char *buffer);
logger->Info(WZMarketDataField md);
logger->Info(WZInputOrderField inputOrder);             
logger->Info(WZOrderActionField orderAction);
logger->Info(WZRtnOrderField rtnOrder);
logger->Info(WZRtnTradeField rtnTrade);
logger->Info(TSMarketDataField md);
logger->Info(TSInputOrderField inputorder);
logger->Info(TSOrderActionField orderaction);
logger->Info(TSRtnOrderField rtnOrder);
logger->Info(TSRtnTradeField rtnTrade);
```

If you have other questions, please click https://www.cnblogs.com/tianyajuanke/archive/2013/02/22/2921850.html