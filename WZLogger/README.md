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

### Improve Performance

#### performance test

c++版本 glog-0.3.4 , 测试数据总量为1.5g， 起12个线程循环写133个字节的日志条目，测试结果耗时175s，每秒

大约8-9MB的吞吐量。

#### optimization

- ##### 去localtime函数调用

  查看glog源码，在获取日期的时候使用了localtime, localtime_r这两个函数，而这两个函数调用_tz_convert， 

  __tz_convert有tzset_lock全局锁，每次获取时间都会使用到kernel级别的futex锁，所以优化第一步是去掉

  glibc的localtime函数，使用getimeofday获取秒数和时区，用纯耗cpu的方式算出日期，稍微复杂一点的计算

  就是闰年闰月的转换。将这段函数替换后，耗时从175s减少成46s，性能瞬间提高4-5倍。

- ##### 减少锁粒度

  再翻看glog的源码，glog是一个多线程同步写的操作，简化代码就是 lock();dosomething();fwrite();unlock(); 

  fwrite本身就是线程安全的，缩小锁粒度需要改成lock();dosomething();unlock();fwrite(); 其他变量都比较好

  处理，比如文件名之类的，不好处理的是轮转的时候会更改fd， fwrite()会使用到fd。我使用了指针托管和引

  用计数的办法，当轮转文件时，将current_fd_ 赋值给old_fd_, 不直接delete或fclose, 简化代码等于：_

  _lock();dosomething();if(true) old_fd_ = current_fd_; currnt_fd_.incr();unlock();fwrite();currnt_fd_.decr(); 

  _当old_fd_ = 0时，才会真正delete 和fclose 这个fd指针。优化后压测耗时30s。

- ##### 引入无锁队列异步IO化

  从第二次优化来看。锁热点已经很少了，性能也有不少提升，已经能满足OCS的需求，但是这种多线程同步堵

  塞写io的模式，一旦出现io hang住的情况，所有worker线程都会堵住。可以看下IO_fwrite 这个函数，在写

  之前会进行IO_acquire_lock() 锁住，写完后解锁。

  为了避免所有线程卡住的情况，需要将多线程同步堵塞转换成单线程异步的io操作，同时避免引入新的锁消耗

  性能，所以引入无锁队列，算法复杂度为O(1)，结构如图所示：
  [![screenshot](https://oss.aliyuncs.com/yqfiles/baa95bae7b6e4daa01a5ac25e1fa7e7b702a5592.png)]()
  每个生产者线程都有独自的无锁队列，生产者线程做日志的序列化处理等，整个glog有一个单线程的消费线

  程，消费线程只处理真正的io请求，无锁队列使用环形数组实现，引入tcmalloc做内存管理。消费线程也会有

  hang住的可能，因为无锁队列使用CAS，当队列满了的时候并不会无限增长内存，而是会重试几次后放弃本

  次操作，避免内存暴涨。改造后耗时33s。