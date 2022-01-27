//
//  threadsafe_stack.cpp
//  CCIA_Learning
//
//  Created by lmc on 2022/1/27.
//

#include "threadsafe_stack.hpp"

template<typename T>
threadsafe_stack<T>::threadsafe_stack(): data_(std::stack<T>()) {}

template<typename T>
threadsafe_stack<T>::threadsafe_stack(const threadsafe_stack& other) {
    std::lock_guard<std::mutex> lock(other.mu_);
    data_ = other.data_;
}

template<typename T>
bool threadsafe_stack<T>::empty() const {
    std::lock_guard<std::mutex> lock(mu_);
    return data_.empty();
}

template<typename T>
void threadsafe_stack<T>::push(T new_value) {
    std::lock_guard<std::mutex> lock(mu_);
    data_.push(new_value);
}

// 本质就是将top和pop绑定在一起，避免在并发环境下两个原子的top和pop方法产生top()返回同一个值而pop掉两个值的情况发生
// 和之前6.824 Lab2中遇到的问题类似: 在锁粒度太小时会产生的问题
template<typename T>
std::shared_ptr<T> threadsafe_stack<T>::pop() {
    std::lock_guard<std::mutex> lock(mu_);
    if (data_.empty())
        throw empty_stack();
    std::shared_ptr<T> const res(std::make_shared<T>(data_.top()));
    data_.pop();
    return res;
}

template<typename T>
void threadsafe_stack<T>::pop(T &value)
{
    std::lock_guard<std::mutex> lock(mu_);
    if(data_.empty())
        throw empty_stack();
    value = data_.top();
    data_.pop();
}

const char *empty_stack::what() const throw() { 
    return "empty stack";
}
