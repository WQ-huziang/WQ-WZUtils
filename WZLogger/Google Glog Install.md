### Google Glog 日志类的安装和使用

------

#### Glog：

1. 下载

```
git clone https://github.com/google/glog
```

2. 配置

```
sudo apt-get install autoconf automake libtool
```

3. 编译和安装

```
进入源码根目录
./autogen.sh
./configure
make -j 24
sudo make install

PS：这样还会出现找不到libglog的错误，需要在root模式下再执行：ldconfig语句激活libglog库
```

#### Gflags:

1. 下载

```
git clone https://github.com/gflags/gflags
```

2. 编译和安装

```
进入源码目录
cmake .
make -j 24
sudo make install
```
