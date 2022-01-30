//
//  Accumulate.cpp
//  CCIA_Learning - Chapter 2.4
//  多线程求和
//  Created by lmc on 2022/1/26.
//
#include <vector>
#include <thread>
#include <numeric>
#include <iterator>
#include <iostream>

template<typename Iterator, typename T>
struct accumulate_block {
    void operator()(Iterator first, Iterator last, T& result) {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
    unsigned long const len = std::distance(first, last);
    if (len == 0) {
        return init;
    }
    unsigned long const min_num_acc = 25;
    unsigned long const max_threads = (len + min_num_acc - 1) / min_num_acc;
    // hardware_concurrency 这个thread的静态成员函数可以返回硬件支持的线程数量，但是也可能返回0
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = len / num_threads; // each threads calculate
    
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);
    
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); i++) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);  // 将迭代器block_end向前移动block_size位
        // std::thread 如果是一般函数直接写函数指针，仿函数需加上括号
        threads[i] = std::thread(accumulate_block<Iterator, T>(),
                                 block_start, block_end, std::ref(results[i]));
        // 使用std::ref是因为thread构造函数对参数是直接拷贝，而如果希望传递一个引用时就需要它
        // 将其包装为一个reference_wrapper类型，可以隐式转换为左值引用类型
        block_start = block_end;
    }
    // 最后还有尾部一块没有计算, 由本线程完成
    accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    
    return std::accumulate(results.begin(), results.end(), init);
}

//int main() {
//    std::vector<int> v(100, 1);
//    parallel_accumulate(v.begin(), v.end(), 0);
//    return 0;
//}
