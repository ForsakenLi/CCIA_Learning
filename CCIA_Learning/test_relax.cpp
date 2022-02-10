//
//  test_relax.cpp
//  CCIA_Learning
//
//  Created by lmc on 2022/2/10.
//

#include <atomic>
#include <thread>
#include <iostream>

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true,std::memory_order_relaxed);  // 1
    y.store(true,std::memory_order_relaxed);  // 2
}
void read_y_then_x()
{
    while(!y.load(std::memory_order_relaxed));  // 3
    if(x.load(std::memory_order_relaxed))  // 4
        ++z;
}
int main()
{
    for (int i = 0; i < 1000; i++) {
        x=false;
        y=false;
        z=0;
        std::thread a(write_x_then_y);
        std::thread b(read_y_then_x);
        a.join();
        b.join();
        std::cout << z << std::endl;  // on x86 always 1
    }
    return 0;
}
