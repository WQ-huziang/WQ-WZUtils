#ifndef MEMPIPER_TYPE_
#define MEMPIPER_TYPE_

#define MAX_QUEUE_NODE_COUNT 10
#define SHM_DATA_SIZE        252

struct SharedMemBlock{
   // 0 means able to read, 1 means able to write 
   int written;
   char data[SHM_DATA_SIZE]; //frame
};

// struct buffer_queue{
// 	SharedMemBlock queue[MAX_QUEUE_NODE_COUNT];
// 	int front,rear;
// 	int lock;
	
// 	bool Init(){
// 		front = rear = 0;
// 		memset(queue, sizeof(SharedMemBlock)*MAX_QUEUE_NODE_COUNT, 0);
// 	}
// 	bool isEmpty(){
// 		return (rear = front);
// 	}
// 	bool isFull(){
// 		return ((rear+1)%MAX_QUEUE_NODE_COUNT == front);
// 	}
// 	bool enqueue(SharedMemBlock *share_s){
// 		if(isFull)	return false;
// 		memcpy(&queue[rear], share_s, sizeof(SharedMemBlock));
// 		rear = (rear+1)%MAX_QUEUE_NODE_COUNT;
// 		return true;
// 	}
// 	SharedMemBlock* dequeue(){
// 		if(isEmpty())	return false;
// 		SharedMemBlock* share_s = &queue[front];
// 		front = (front + 1)%MAX_QUEUE_NODE_COUNT;
// 		return share_s;
// 	}
// }

#endif

