#pragma once

#include <chrono>
#include <iostream>

using time_point = std::chrono::time_point<std::chrono::system_clock>;
using duration = std::chrono::system_clock::duration;

class benchmark_result { 
public:
    benchmark_result(time_point start_time, time_point end_time)
    {
        ellapsed_time = end_time - start_time;
    }
   
    long long get_ms() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(ellapsed_time).count();
    }

    long long get_sec() {
        return std::chrono::duration_cast<std::chrono::seconds>(ellapsed_time).count();
    }

    void log(const char* message, std::ostream& out) { 
        out << message << "MS elapsed: " << get_ms() << "\nSec: " << get_sec() << "\n";
    }

private:
    duration ellapsed_time;
};

class benchmark {
public:
    void start() {
        start_time = std::chrono::system_clock::now();
    }

    benchmark_result stop() {
        time_point end_time = std::chrono::system_clock::now();
        return benchmark_result(start_time, end_time);
    }

private:
    time_point start_time;
};
