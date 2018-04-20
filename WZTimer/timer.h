//
// Created by wesley on 18-4-19.
//

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

using dur = std::chrono::duration<int64_t, std::micro>;
/*
 * 调用之间返回当前时间点到2018年04月19日 10:29:13的us级别
 * 返回 time_t 实际上是个 long
 */
time_t getTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock,std::chrono::microseconds> tp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    auto tmp=std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch());
    std::time_t timestamp = tmp.count();
    // 以 2018年04月19日 10:29:13 时间点往后计时
    timestamp -= 1524104953746000;
    return timestamp;
}

/*
 * 输入t1 为开始时间，t2为结束时间， 返回long为us数
 * 返回 time_t 实际上是个 long
 */
long getDuration(time_t t1, time_t t2){
    auto mt1 = std::chrono::microseconds(t1);
    auto mt2 = std::chrono::microseconds(t2);
    dur time_span = std::chrono::duration_cast<dur>(mt2-mt1);
    long time = time_span.count();
    return time;
}

#endif
