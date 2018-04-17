#include <iostream>
#include <cstdio>
#include <chrono>

using Clock = std::chrono::system_clock;
using dur = std::chrono::duration<double, std::micro>;
using tpns = std::chrono::time_point<Clock, dur>;

using std::cout;
using std::endl;
int main(){
    double avg_time = 0;
    int CycleNum = 10000;
    for (int k = 0; k < CycleNum; ++k) {
        tpns begin = Clock::now();

        /* 待测试的代码
        int i = 0;
        for (int j = 0; j < 1024; ++j) {
            ++i;
        }
        tpns end = Clock::now();
        */

        /* 输出当前时间戳 */
        // cout<<end.time_since_epoch().count()<<endl;
        dur time_span = std::chrono::duration_cast<dur>(end-begin);

        /* 输出单次运行的时间 */
        cout<<"Time of one time = "<<time_span.count()<<endl;
        avg_time += time_span.count();
    }
    cout<<"Average Time = "<<avg_time/CycleNum<<endl;

}
