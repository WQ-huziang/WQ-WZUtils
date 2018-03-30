# 无锁队列 - FQueue

### Include fqueue.h in inc/ directory
    fqueue.h
    The implement of the queue is also in the fqueue.h, so there is no .cpp or .so files.

### Usage
    // Type can use any type you want, and you can set the size of the queue through constructor.
    FQueue<Type> queue; // default size is 1024
    FQueue<Type> queue(2048);
    
    // push
    queue.push(data);
    
    // pop
    queue.pop(data);
    
    // get size
    queue.size();


无锁队列原理:www.cnblogs.com/sniperHW/p/4172248.html
