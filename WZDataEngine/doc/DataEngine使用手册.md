# DataEngine使用手册

## 模块总体介绍

- 本模块以数据持久化存储为需求而开发。目前实现了mongodb数据库的接口。使用者可以通过本模块的api连接mongodb数据库并进行增删查改操作。

### 环境配置：

- 本模块依赖于bsoncxx和mongocxx两个外部模块，需要添加外部模块路径：

  ~~~bash
  outside include directory:
  /usr/local/include/mongocxx/v_noabi
  /usr/local/include/libmongoc-1.0
  /usr/local/include/bsoncxx/v_noabi
  /usr/local/include/libbson-1.0

  outside lib: (located in /usr/local/lib)
  mongocxx.so
  bsoncxx.so
  ~~~

  头文件在inc文件夹中，使用时，将如下文件复制到使用者头文件目录下：

  ~~~bash
  rapidjson(directory)
  DataParse.h
  ToDocuments.h
  transportstruct.h
  WZDataEngine.h
  WZMongodbEngine.h
  ~~~

  链接库文件在lib文件夹中，使用时，将如下文件复制到使用者库文件目录下：

  ~~~bash
  libWZMongodbEngine.a (static)
  or
  libWZMongodbEngine.so.1.x (dynamic, x is newest version)
  ~~~

- CMakeLists.txt中写法：

  ```
  # add mongocxx and bson include dir
  SET(SHARE_LIB_PREFIX /usr/local)
  INCLUDE_DIRECTORIES(${SHARE_LIB_PREFIX}/include/mongocxx/v_noabi)
  INCLUDE_DIRECTORIES(${SHARE_LIB_PREFIX}/include/libmongoc-1.0)
  INCLUDE_DIRECTORIES(${SHARE_LIB_PREFIX}/include/bsoncxx/v_noabi)
  INCLUDE_DIRECTORIES(${SHARE_LIB_PREFIX}/include/libbson-1.0)

  # add libs
  SET(DATAENGINE_LIBS WZMongodbEngine mongocxx bsoncxx)
  ADD_EXECUTABLE(main main.cpp)
  TARGET_LINK_LIBRARIES(main ${DATAENGINE_LIBS} your_libs)
  ```

### 程序接口说明

```c++
void setLibname(const char lib[20]);
```

- 设置数据库库名，之后所有操作将在该库内进行。

```c++
void setTablename(const char table[20])
```

- 设置数据库表名，之后所有操作将在该表内进行。

```c++
void init();
```

- 初始化函数，此时会建立到数据库的连接。 <br>
  PS：此函数在一个进程内只能使用一次，否则会出现段错误。

```c++
int insert_one(const map<string, string> &);
int insert_many(const vector<map<string, string>> &);
```

- 插入一个或多个条目。其中map中的两个string代表key-value键值对。如果插入多个条目，每个条目为vector中的一项。返回值代表插入成功数量。

```c++
int update_one(const KeyValue &, const vector<KeyValue> &);
int update_many(const KeyValue &, const vector<KeyValue> &);
```

- 更新一个或多个条目。第一个参数代表筛选条件，用于确定更新对象，第二个参数代表要更新的内容，由多个key-value对组成。返回值代表更新成功的条目数量。

```c++
int find_one(map<string, string> &, const vector<KeyValue> &, const char ID[20] = "\0");
int find_many(vector<map<string, string>> &, const vector<KeyValue> &, const char ID[20] = "\0");
```

- 查找一个或多个条目。第一个参数代表返回值，第二个参数代表筛选条件，最后一个参数代表想查找的InstrumentID，默认查找所有股票。返回值代表符合查找条件的条目数量。

```c++
int delete_one(const vector<KeyValue> &, const char ID[20] = "\0");
int delete_many(const vector<KeyValue> &, const char ID[20] = "\0");
```

- 删除一个或多个条目。第一个参数代表筛选条件，第二个参数代表想删除的InstrumentID。

```c++
int set_index(string, bool isascending = true);
```

- 设置索引，isascending代表索引方向，true为升序，false为降序。

### 程序使用样例

- 初始化

~~~c++
// 实例化
DataEngine *db = MongodbEngine::getInstance();
// 初始化
db->init();
// 设置库名表名
db->setLibname("test");
db->setTablename("test");
~~~

- 插入任意条目

~~~c++
map<string, string> md;
md["ID"] = "123";
md["price"] = "567.789";
md["volume"] = "12345";
if (db->insert_one(md)) {
    cout << "Insert Successly!" << endl;
}
~~~

- 插入TS结构

~~~c++
map<string, string> md;
TSMarketDataField tsmd;
parseFrom(md, tsmd);
if (db->insert_one(md)) {
    cout << "Insert TSMarketDataField Successly!" << endl;
}
~~~

- 更新条目

~~~c++
//筛选条件
KeyValue md;
md.key = "LastPrice";
md.minvalue = "0";
//更新内容
vector<keyValue> mds;
KeyValue tmd;
tmd.key = "LastPrice";
tmd.minvalue = "1";
mds.push_back(tmd);
if (db->update_one(md, mds)) {
	cout << "Update One Item Successly!" << endl;
}
~~~

- 查询某股票5分钟区间的最新价

~~~c++
char givenID[20] = "l1805";
string begintime = "14:35:00";
string endtime = "14:40:59";
vector<map<string, string>> mds;
vector<KeyValue> cond;
cond.push_back(KeyValue("UpdateTime", begintime, endtime));
if (db->find_many(mds, cond, givenID)) {
    for (auto &md : mds) {
        cout << atof(md["LastPrice"]) << endl;
    }
}
~~~

- 查询某股票某天的最新价

~~~c++
char givenID[20] = "l1805";
string day = "20180403";
vector<map<string, string>> mds;
vector<KeyValue> cond;
cond.push_back(KeyValue("TradingDay", day));
if (db->find_many(mds, cond, givenID)) {
    for (auto &md : mds) {
        cout << atof(md["LastPrice"]) << endl;
    }
}
~~~

- 删除某个股票的持仓量小于等于10的记录

~~~c++
char givenID[20] = "l1805";
KeyValue md;
md.key = "Position";
md.sides = true;
md.maxvalue = "10";
vector<KeyValue> mds;
mds.push_back(md);
if (db->delete_many(mds, givenID)) {
	cout << "Delete Successly!" << endl;
}
~~~

- 设置索引

~~~c++
string md = "LastPrice";
if (db->set_index(md, 1) == 0) {
	cout << "Set Ascending Index Successly!" << endl;
}
~~~