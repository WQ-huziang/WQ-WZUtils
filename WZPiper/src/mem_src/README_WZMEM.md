#### WZMEM use shared memory to communicate between proccess

### Configure file must be included

```
[MemInfo]
# key of the shared memory, better not to use 0
key=12
# shared memory size, must be enough to contain the QueueManager
memorysize=1024000

```

### Include .h file in folder inc, remember to copy them to your include folder
```
MemEngine.h
MemWriter.h
MemReader.h
MemQueue.hpp
```

### .so file is in folder lib, remember to copy them to your lib folder add them to CMakeLists
```
libMemEngine.so
libMemReader.so
libMemWriter.so
libiniparser.so
liblogger.so
```

### For MemEngine user:

- If use as MemWriter to send data, follow the next step:

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
if(memWriter -> writeMem() == -1) {
	printf("write failed\n");
}
else if (memWriter -> writeMem() == 0) {
	printf("write succeed\n");
}

// optional, if you want to destroy the shared memory created, add this, else don't.
// delete memWriter;

```

- If use as server(such as Strategy module) to receive data, follow the next step:

```
// receive message
// declare
WZPiper * piper = new UdpPiper();

// read configuer file and set private variable port and ip
piper -> set_config_info(filePathIn);

// init
piper -> init_as_server();

// create a frame and set data to receive data
Frame recvFrame;

// receive data to recvFrame
piper -> do_read(recvFrame);

```