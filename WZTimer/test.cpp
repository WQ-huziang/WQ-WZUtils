#include <iostream>
#include <cstdio>
#include <chrono>

using Clock = std::chrono::system_clock;
using dur = std::chrono::duration<double, std::micro>;
using tpns = std::chrono::time_point<Clock, dur>;

int main(){
    double time = 0;
    for (int k = 0; k < 100; ++k) {
        tpns begin = Clock::now();

        int i = 0;
        for (int j = 0; j < 1024; ++j) {
            ++i;
        }
        tpns end = Clock::now();
        dur time_span = std::chrono::duration_cast<dur>(end-begin);
        cout<<time_span.count()<<endl;
        time += time_span.count();
    }
    cout<<"Time = "<<time/100<<endl;

}
