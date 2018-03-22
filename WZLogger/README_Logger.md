## README Logger

### Configure file 

```
[Logger]
logtostderr=0           #0-输出到文件，1输出到stderr
alsologtostderr=1       #1-同时将日志输出到文件和stderr
minloglevel=0           #限制输出到 stderr 的部分信息，包括此错误级别和更高错误级别的日志信息
stderrthreshold=2       #除了将日志输出到文件之外，还将此错误级别和更高错误级别的日志同时输出到 stderr
colorlogtostderr=0      #将输出到 stderr 上的错误日志显示相应的颜色
log_dir=log             #log文件夹的路径，需要先新建log文件夹
```

### Include .h file in folder inc, remember to copy them to your include folder
```
Logger.h
```

### .so file is in folder lib, remember to copy them to your lib folder add them to CMakeLists
```
liblogger.so
```

### Edit  your  CmakeList.txt

```
add:
TARGET_LINK_LIBRARIES(yourexe logger iniparser glog)
```

### First declare and load config in  main

```
Logger *logger;

int main(int argc, char *argv[]){
    logger = new Logger(argv[0]);
    char ini[] = {"config.ini"};
    logger->ParseConfigInfo(ini);
}
```

### To  output  log

```
extern Logger *logger;

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

### Other  output  method

https://www.cnblogs.com/tianyajuanke/archive/2013/02/22/2921850.html： 二、使用方法  ： 4、日志类型