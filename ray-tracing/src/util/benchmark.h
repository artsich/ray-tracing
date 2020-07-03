#pragma once

#include <chrono>
#include <iostream>

using time_point = std::chrono::time_point<std::chrono::system_clock>;

class benchmark_result { 
public:
    benchmark_result(time_point start_time, time_point end_time) 
    : start_time(start_time)
    , end_time(end_time)
    {
    }
   
    long long get_ms() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    }

    long long get_sec() {
        return std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    }

    void log(std::ostream& out) { 
        std::cerr << "\nDone.\n" << "MS elapsed: " << get_ms() << "\nSec: " << get_sec() << "\n";
    }

private:
    time_point start_time;
    time_point end_time;
};

class benchmark {
public:
    void start() {
        start_time = std::chrono::system_clock::now();
    }

    benchmark_result stop() {
        end_time = std::chrono::system_clock::now();
        return benchmark_result(start_time, end_time);
    }

private:
    time_point start_time;
    time_point end_time;
};
