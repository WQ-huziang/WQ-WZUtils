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
	blockMode: WZ_PIPER_BLOCK for Recv and Send block the proccess
			   WZ_PIPER_NBLOCK for Recv and Send return without block
Return: positive if create succeed, -1 if failed
*************************************************/
int init(char file_path[256], int piperMode, int blockMode);

/************************************************* 
Function: Recv
Description: read configure file and init the IPC according to server_client_flag
InputParameter: 
	frame: read receive datum from piper to frame
Return: size of receive data(positive) if receive succeed, -1 if failed
*************************************************/
int Recv(Frame &frame);

/************************************************* 
Function: Send
Description: read configure file and init the IPC according to server_client_flag
InputParameter:
	frame: the datum to send to piper
Return: 0 if send succeed, -1 if failed
*************************************************/
int Send(Frame &frame);

```

#### Some constant type use in piper

```
///////////////////////////////////
// The mode of the piper 
// (server or client)
///////////////////////////////////
//server
#define WZ_PIPER_SERVER          0
//client 
#define WZ_PIPER_CLIENT          1
typedef short WZPiperMode;

///////////////////////////////////
// The mode of blocking 
// (blocking or nonblocking)
///////////////////////////////////
//blocking
#define WZ_PIPER_BLOCK           0
//nonblocking
#define WZ_PIPER_NBLOCK			 1
typedef short WZBlockMode;
```

#### To use WZPiper:

```
// use shared memory IPC
WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> >  * memPiper;
// declare as server
memPiper = new WZPiper<MemEngine<DataType, QueueSize, MaxReaderSize> > ();
// configuration file path
memPiper -> init(filePath, WZ_PIPER_SERVER, WZ_PIPER_NBLOCK);

// use tcp IPC
WZPiper<TcpSocket >  * tcpPiper;
// declare as server
tcpPiper = new WZPiper<TcpSocket > ();
// configuration file path
tcpPiper -> init(filePath, WZ_PIPER_SERVER, WZ_PIPER_NBLOCK);

```