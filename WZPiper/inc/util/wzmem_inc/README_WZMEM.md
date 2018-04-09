## WZMEM use shared memory to communicate between proccess

##### Members: luoqingming huangxiaolin

----

### Interface Instructions:

- MemEngine class template interface:

```
// To use this, the Server and client should decide the QueueDataType, DataQueueSize and MaxReaderSize at first
/***************************************************************************
Description: MemEngine reads data from shared memory(pop data from the MemQueue)
             MemEngine writes data into shared memory(push data to the MemQueue)
****************************************************************************/
template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
class MemEngine{

  /************************************************* 
  Function: MemEngine
  Description: Constructor, read key and size from configure file 
     create or attach the shared memory
     and init the queue_manager according to piperMode
  InputParameter: 
     piperMode:  the flag to mark server or client, 
       0 or WZ_PIPER_SERVER as server,
       1 or WZ_PIPER_CLIENT as client
  Return: none
  *************************************************/
  MemEngine(int piperMode);

  /************************************************* 
  Function: init
  Description: read configure file and init as server or client,
    server will create the shared memory and init the QueueManager
  InputParameter: none
  Return: true if create succeed, false if failed
  *************************************************/
  bool init(char file_path[256]);

  /************************************************* 
  Function: wzRecv
  Description: read a frame from shared memory queue
  InputParameter: 
    frame: pop(memcpy) a datum in queue to mail
  Return: 1 if receive succeed, 0 if failed
  *************************************************/
  int wzRecv(QueueDataType &data);

  /************************************************* 
  Function: wzSend
  Description: write a frame to shared memory queue
  InputParameter: 
    frame: the datum to push(memcpy) into queue
  Return: 1 if send succeed, 0 if failed
  *************************************************/
  int wzSend(QueueDataType &data);
};
```

- QueueManager use in MemEngine:

```
// To use this, the MemEngine should decide the DataQueueSize and MaxReaderSize first
/***************************************************************************
Description: QueueManager manage the MemQueue used in shared memory
****************************************************************************/
template <typename QueueDataType, int DataQueueSize, int MaxReaderSize>
struct QueueManager
{

   // the MemQueue<DataType, DataQueueSize(must be 2^n), MaxReaderSize>
   MemQueue<QueueDataType, DataQueueSize, MaxReaderSize > frame_rec_queue;
   MemQueue<QueueDataType, DataQueueSize, MaxReaderSize > frame_req_queue;

   // Return: 1 if initManager succeed, 0 if failed
   bool initManager(){

      // call different queue's init
      if (!frame_req_queue.initQueue()) {
         return false;
      }

      if (!frame_rec_queue.initQueue()) {
         return false;
      }

      return true;
   }
};
```

----

### Usage Instructions:

#### Configure file must be included:

```
[MemInfo]
# key of the shared memory, better not to use 0
key=12
```

#### Include .h and .hpp file in folder WZUtils/WZPiper/inc/util/wzmem_inc/, or remember to copy them to your include folder:

```
memengine.h
memqueue.hpp
```

#### no need to link .so file!!!


#### The CmakeLists.txt should add following statements:

```
SET(UTILS_SO iniparser logger glog)

INCLUDE_DIRECTORIES(${INC_FOLDER_PATH}) # the include folder contains two h&hpp file above

LINK_DIRECTORIES(${LIB_FOLDER_PATH})

ADD_EXECUTABLE(main main.cpp)

// link UTILS_SO before link MEM_FILE!!!
TARGET_LINK_LIBRARIES(main ${UTILS_SO})
```

#### For MemEngine user:

- If use it as server, such as Td and Md:

```
// declare
WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> >  * memServer;

// new a MemWriter object
memServer = new WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> > (WZ_PIPER_SERVER);

// read configuer file, create or attach shared memory
memServer -> init(file);

// write message
Frame sendFrame;
sendFrame.source = 1;
sendFrame.msg_type = 1;
sendFrame.error_id = WZ_ERROR_ID_SUCCESS;
sendFrame.rtn_type = 1;
sendFrame.length = 1;

// send message
int rtn = memServer -> wzSend(sendFrame);
if(rtn == -1) {
	printf("write failed\n");
}
else if (rtn == 0) {
	printf("write succeed\n");
}

// receive message
int rtn = memServer -> wzRecv(recvFrame);
if(rtn == -1) {
  printf("write failed\n");
}
else if (rtn == 0) {
  printf("write succeed\n");
}

// delete the memServer pointer to detach the memory
delete memServer;

```

- If use it as client, such as strategy:

```
// declare
WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> >  * memClient;

// new a MemWriter object
memClient = new WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> > (WZ_PIPER_CLIENT);

// read configuer file, create or attach shared memory
memClient -> init(file);

// write message
Frame sendFrame;
sendFrame.source = 1;
sendFrame.msg_type = 1;
sendFrame.error_id = WZ_ERROR_ID_SUCCESS;
sendFrame.rtn_type = 1;
sendFrame.length = 1;

// receive message
int rtn = memClient -> wzRecv(recvFrame);
if(rtn == -1) {
  printf("write failed\n");
}
else if (rtn == 0) {
  printf("write succeed\n");
}

// send message
int rtn = memClient -> wzSend(sendFrame);
if(rtn == -1) {
  printf("write failed\n");
}
else if (rtn == 0) {
  printf("write succeed\n");
}

// delete the memClient pointer to detach the memory
delete memClient;

```
