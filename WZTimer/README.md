# 计时器写法

### Include chrono
    #include <chrono>
    并且在要使用的地方加上：
    
    using Clock = std::chrono::system_clock;   // 确定使用哪种表
    using dur = std::chrono::duration<double, std::micro>;  // 确定统计精度
    using tpns = std::chrono::time_point<Clock, dur>;   

### Usage
    有三种类型的Clock：system_clock, steady_clock, high_resolution_clock；目前统一使用steady_clock
    要提高精度可以将std::micro（微妙）改为 std::nano（纳秒）

    tpns time_point = Clock::now();  //获取时间
    
    dur time_span = std::chrono::duration_cast<dur>(time_point2-time_point1);  //获取时间差

    double time = time_span.count();  //double格式时间差

    // 建议循环的时候多统计几次，即使是steady_clock浮动幅度任然比较大


