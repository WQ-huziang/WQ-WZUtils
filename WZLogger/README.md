## WZLogger
- Objective:
- Member:

### Logger.h
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

#### create a folder to save logging file

#### config.ini

```c++
[Logger]
logtostderr=0           #0-输出到文件，1输出到stderr
alsologtostderr=1       #1-同时将日志输出到文件和stderr
minloglevel=0           #限制输出到 stderr 的部分信息，包括此错误级别和更高错误级别的日志信息
stderrthreshold=2       #除了将日志输出到文件之外，还将此错误级别和更高错误级别的日志同时输出到 stderr
colorlogtostderr=0      #将输出到 stderr 上的错误日志显示相应的颜色
log_dir=../log          #log文件夹的路径，需要先新建log文件夹
```

#### inc

```c++
/* copy  Logger.h and folder glog to the folder inc */
Logger.h
glog
```

#### lib

```c++
/* copy libLogger.h to the folder lib */

libLogger.h
```

#### CmakeList.txt

```c++
/* add the following instruction to the CMakeList.txt */
TARGET_LINK_LIBRARIES(yourexe Logger iniparser glog)
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
```

If you have other questions, please click https://www.cnblogs.com/tianyajuanke/archive/2013/02/22/2921850.html