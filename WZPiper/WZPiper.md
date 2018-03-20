#### WZPiper to communicate between modules

### Configure file must be included

```
[NetInfo]
ip=192.168.0.6
port=22520

```

### Include .h file in folder inc
```

```

### .so file is in folder lib, remember add them to CMakeLists
```

```

### For UDPPiper user:

- If use as client(such as Md module) to send data, follow the next step:

```
// send message
// declare
WZPiper * piper = new UDPPiper();

// read configuer file and set private variable port and ip
piper -> set_config_info(filePathOut);

// init
piper -> init_as_client();

while (1) {
	// create a frame and set data to send 
	Frame frame;
	frame.source = 0;
	frame.msg_type = 1;
	frame.error_id = 2;
	frame.rtn_type = 3;
	frame.length = 4;
	char* data = new char [5];
	strcpy(data, "1234");
	memcpy(frame.data, data, 5);

	// send data
	piper -> do_write(frame);
}
```

- If use as server(such as Strategy module) to receive data, follow the next step:

```
// receive message
// declare
WZPiper * piper = new UDPPiper();

// read configuer file and set private variable port and ip
piper -> set_config_info(filePathIn);

// init
piper -> init_as_server();

// create a frame and set data to receive data
Frame recvFrame;

// receive data to recvFrame
piper -> do_read(recvFrame);

```

### For TcpPiper user:

- If use as client(such as Strategy module), follow the next step:

```
// declare
WZPiper *pip = new TcpPiper();

// read configuer file and set private variable port and ip
pip->set_config_info(config_file_path);

// init
pip->init_as_client();

// create a frame to receive data
Frame rec_frame;

for(;;) {
	// receive data to rec_frame
	if (pip->do_read(rec_frame)) {
		printf("%d\n",rec_frame.source);
		printf("%d\n",rec_frame.msg_type);
	}
	else {
		// run strategy here
		printf("%s\n", "write frame");
		// send data
		pip->do_write(my_frame_1);
		usleep(1000000);
	}
}
```

- If use as server(such as Td module), follow the next step:

```
// declare
WZPiper *pip = new TcpPiper();

// read configuer file and set private variable
pip->set_config_info(config_file_path);

// init
pip->init_as_server();

// create a frame and receive data
Frame my_frame;
for(;;) {
	// receive data to rec_frame
	if (pip->do_read(my_frame)) {
		printf("%d\n",my_frame.source);
		printf("%d\n",my_frame.msg_type);
		// spi take advantage of my_frame
	}
	else {
		
	}
}
```