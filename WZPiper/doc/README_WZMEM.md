## WZMEM use shared memory to communicate between proccess

##### Members: luoqingming huangxiaolin

----

### Interface Instructions:

- MemEngine class interface:

```

/************************************************* 
Function: createMemory
Description: create shared memory function
InputParameter: 
  m_key: the key of to-create shared memory
  m_size: the size of to-create shared memory
  m_flag: the shm flag of to-create shared memory
  m_shmid: a initial int id, will be set to
     the shared memory id after calling the function.
  m_memory_addr: a initial address, will be set point to
     the shared memory first address after calling the function.
Return: 1 if create succeed, 0 if failed
*************************************************/ 
bool createMemory(const int &m_key, const int &m_size, const int &m_flag, int &m_shmid, char* & m_memory_addr);

/************************************************* 
Function: destroyMemory
Description: destroy shared memory function
InputParameter: 
  m_shmid: the to-destroy shared memory id, will be set to -1 if succeed.
  m_memory_addr: the pointer point to to-destroy shared memory address, will be set to NULL if succeed.
Return: 1 if create succeed, 0 if failed
*************************************************/
bool destroyMemory(int & m_shmid, char* & m_memory_addr);

/************************************************* 
Function: attachMemory
Description: attach shared memory function
InputParameter:
  m_key: the to-attach shared memory key.
  m_shmid: the to-attach shared memory id.
  m_flag: the to-attach shared memory flag.
  m_memory_addr: the pointer will be point to to-attach shared memory address.
Return: 1 if create succeed, 0 if failed
*************************************************/
bool attachMemory(const int & m_key, int & m_shmid, const int & m_flag, char*& m_memory_addr);

/************************************************* 
Function: detachMemory
Description: detach shared memory function
InputParameter:
  m_shmid: the to-detach shared memory id.
  m_memory_addr: the pointer point to to-detach shared memory address.
Return: 1 if create succeed, 0 if failed
*************************************************/
bool detachMemory(const int & m_shmid, char*& m_memory_addr);

```

- QueueManager use in MemWriter and MemReader:

```
// To use this, the writer and readers should decide the DataQueueSize and MaxReaderSize first
/***************************************************************************
Description: QueueManager manage different kind of MemQueue s used in shared memory
****************************************************************************/
typedef struct QueueManager_
{

   // the MemQueue<DataType, DataQueueSize(must be 2^n), MaxReaderSize>
   MemQueue<Frame, 4, 2> frame_mem_queue;
   // MemQueue<Frame, 1024, 2 > frame_mem_queue;

   // Return: 1 if initManager succeed, 0 if failed
   bool initManager(){
      // init MemQueue
      int rtn = true;

      // call different queue's init
      if (!frame_mem_queue.initQueue()) {
         rtn = false;
      }
      
      return rtn;
   }
   
}QueueManager;
```

- MemWriter class interface:

```
/************************************************* 
Function: initAsWriter
Description: init as writer to read
InputParameter: none
Return: 0 if create succeed, -1 if failed
*************************************************/
int initAsWriter();

/************************************************* 
Function: setConfigInfo
Description: read configure file and set key and size
InputParameter: none
Return: 0 if create succeed, -1 if failed
*************************************************/
int setConfigInfo(char file_path[256]);

/************************************************* 
Function: writeMem
Description: write a frame to shared memory queue
InputParameter: 
	mail: push (memcpy) a datum to push into queue
Return: 0 if create succeed, -1 if failed
*************************************************/
int writeMem(Frame &mail);
```

- MemReader class interface:

```
/************************************************* 
Function: initAsReader
Description: init as reader to read
InputParameter: none
Return: 0 if create succeed, -1 if failed
*************************************************/
int initAsReader();

/************************************************* 
Function: setConfigInfo
Description: read configure file and set key and size
InputParameter: none
Return: 0 if create succeed, -1 if failed
*************************************************/
int setConfigInfo(char file_path[256]);

/************************************************* 
Function: readMem
Description: read a frame from shared memory queue
InputParameter: 
	mail: pop(memcpy) a datum in queue to mail
Return: 0 if create succeed, -1 if failed
*************************************************/
int readMem(Frame &mail);
```

----

### Usage Instructions:

#### Configure file must be included:

```
[MemInfo]
# key of the shared memory, better not to use 0
key=12
# shared memory size, must be enough to contain the QueueManager
memorysize=1024000

```

#### Include .h file in folder WZUtils/WZPiper/inc/util/wzmem_inc/, remember to copy them to your include folder:

```
MemEngine.h
MemWriter.h
MemReader.h
MemQueue.hpp
```

#### .so file is in folder WZUtils/WZPiper/lib, remember to copy them to your lib folder:

```
libMemEngine.so
libMemReader.so
libMemWriter.so
libiniparser.so
liblogger.so
```

#### The CmakeLists.txt should add following statements:

```
SET(MEM_FILE MemReader MemWriter MemEngine)
SET(UTILS_SO iniparser logger glog)

INCLUDE_DIRECTORIES(${INC_FOLDER_PATH})

LINK_DIRECTORIES(${LIB_FOLDER_PATH})

ADD_EXECUTABLE(main main.cpp)

// link UTILS_SO before link MEM_FILE!!!
TARGET_LINK_LIBRARIES(main ${MEM_FILE} ${UTILS_SO})
```

#### For MemEngine user:

- If use MemWriter to send data, follow the next step:

```
// send message
// declare
MemEngine * memWriter;

// new a MemWriter object
memWriter = new MemWriter();

// read configuer file and set private variable shared memory key and shared memory size
memWriter -> setConfigInfo(file);

// initialize as writer
memWriter -> initAsWriter();

// write message
Frame sendFrame;
sendFrame.source = 1;
sendFrame.msg_type = 1;
sendFrame.error_id = WZ_ERROR_ID_SUCCESS;
sendFrame.rtn_type = 1;
sendFrame.length = 1;

int rtn = memWriter -> writeMem();
if(rtn == -1) {
	printf("write failed\n");
}
else if (rtn == 0) {
	printf("write succeed\n");
}

// optional, if you want to destroy the shared memory created, add this, else don't.
// delete memWriter;

```

- If use MemReader to receive data, follow the next step:

```
// receive message
// declare
MemEngine * memReader;

// new a MemWriter object
memReader = new MemReader();

// read configuer file and set private variable shared memory key and shared memory size
memReader -> setConfigInfo(file);

// initialize as reader
memReader -> initAsReader();

// write message
Frame recvFrame;
recvFrame.length = 1;
if(memReader -> readMem(recvFrame) == -1) {
	printf("write failed\n");
}
else {
	printf("write succeed\n");
}

// delete the Reader pointer to detach the memory
delete memReader;

```

------

### Related Knowledge:

- ##### 无锁队列：
    
  - www.cnblogs.com/sniperHW/p/4172248.html

- ##### C++类继承：

  - **public 继承：**基类的public，protected成员在子类中访问属性不变，子类新增的成员函数可以直接访问，但是对于基类的private成员依然是基类的私有，子类无法直接进行访问。
  - **private 继承：**基类的public，protected成员转变为子类的private成员，子类新增的成员函数可以进行访问，对于基类的private成员依然是基类的私有，子类无法直接进行访问。
  - **protected 继承：**基类的public，protected成员转变为子类的protected成员，子类新增的成员函数可以进行访问，对于基类的private成员依然是基类的私有，子类无法直接进行访问。





