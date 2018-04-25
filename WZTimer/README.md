# 计时器写法

### Include timer.h
    #include <timer.h>

### Usage
    目前统一使用steady_clock, 精度为us级别

    time_t time_point = getTimeStamp();  //获取时间, time_t可直接转化为long类型

    long time_span = getDuration(begin_time, end_time);  //获取时间差

### 同一计时次数 1024 次：

    建议声明一个全局变量 long time_points[1024];

    再计时完成之后再写入到对应文件：
    ofstream fout;
    fout.open("time_points.txt", ios::binary);
    for(int i = 0; i < 1024; ++i)
	fout<<time_points[i]<<'\t';

### TSC计时

        int64_t readtsc(void)
        {
        #if defined(__GNUC__)
        #   if defined(__i386__)
            uint64_t x;
            __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
            return x;
        #   elif defined(__x86_64__)
            uint32_t hi, lo;
            __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
            return ((uint64_t)lo) | ((uint64_t)hi << 32);
        #   else
        #       error Unsupported architecture.
        #   endif
        #elif defined(_MSC_VER)
            __asm {
                return __rdtsc();
            }
        #else
        #   error Other compilers not supported...
        #endif
        }



