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





