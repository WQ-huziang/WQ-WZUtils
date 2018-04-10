## WZPiper to communicate between proccess

### Include sokect and shared memory, see more details in the WZUtils/WZPiper/doc folder

#### The top WZPiper class template interface is:

```
/************************************************* 
Function: WZPiper
Description: Constructor, calls the constructor of T
InputParameter: none
Return: none
*************************************************/
WZPiper();

/************************************************* 
Function: ~WZPiper
Description: Destructor, delete T
InputParameter: none
Return: none
*************************************************/		
~WZPiper();

/************************************************* 
Function: init
Description: read configure file and init the IPC according to server_client_flag
InputParameter: 
	file_path: the path of the configure file
	piperMode: the flag to mark server or client, 
		0 or WZ_PIPER_SERVER as server,
		1 or WZ_PIPER_CLIENT as client
Return: positive if init succeed, -1 if failed
*************************************************/
int init(char file_path[256], int piperMode);

/************************************************* 
Function: wzRecv
Description: read configure file and init the IPC according to server_client_flag
InputParameter: receive a frame from piper
	frame: read receive datum from piper to frame
Return: positive if receive succeed, 0 if failed
*************************************************/
int Recv(Frame &frame);

/************************************************* 
Function: wzSend
Description: read configure file and init the IPC according to server_client_flag
InputParameter: send a frame to piper
	frame: the datum to send to piper
Return: positive if send succeed, 0 if failed
*************************************************/
int Send(Frame &frame);
```

#### To use WZPiper:

```
// use shared memory IPC
WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> >  * memPiper;
// declare as server
memPiper = new WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> > (WZ_PIPER_SERVER);
// configuration file path
memPiper -> init(filePath);

// use tcp IPC
WZPiper<TcpSocket >  * memPiper;
// declare as server
memPiper = new WZPiper<TcpSocket > (WZ_PIPER_SERVER);
// configuration file path
memPiper -> init(filePath);
```