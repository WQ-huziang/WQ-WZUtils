## WZPiper to communicate between proccess

### Include sokect and shared memory, see more details in the WZUtils/WZPiper/doc folder

#### The top WZPiper class template interface is:

```
/************************************************* 
Function: WZPiper
Description: Constructor, calls the constructor of T
InputParameter: 
	piperMode: the flag to mark server or client, 
		0 or WZ_PIPER_SERVER as server,
		1 or WZ_PIPER_CLIENT as client
Return: none
*************************************************/
WZPiper(int piperMode);

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
Return: true if create succeed, false if failed
*************************************************/
bool init(char file_path[256]);

/************************************************* 
Function: wzBind
Description: do what???
InputParameter: none
Return: true if bind succeed, false if failed
*************************************************/		
bool wzBind();

/************************************************* 
Function: wzConnect
Description: do what???
InputParameter: none
Return: true if connect succeed, false if failed
*************************************************/
bool wzConnect();

/************************************************* 
Function: wzRecv
Description: read configure file and init the IPC according to server_client_flag
InputParameter: receive a frame from piper
	frame: read receive datum from piper to frame
Return: positive if receive succeed, 0 if failed
*************************************************/
int wzRecv(Frame &frame);

/************************************************* 
Function: wzSend
Description: read configure file and init the IPC according to server_client_flag
InputParameter: send a frame to piper
	frame: the datum to send to piper
Return: positive if send succeed, 0 if failed
*************************************************/
int wzSend(Frame &frame);
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