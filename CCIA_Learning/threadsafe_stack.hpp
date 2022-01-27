//
//  threadsafe_stack.hpp
//  CCIA_Learning - Chapter 3.2
//  线程安全的栈
//  Created by lmc on 2022/1/27.
//

#ifndef threadsafe_stack_hpp
#define threadsafe_stack_hpp

#include <exception>
#include <memory>
#include <mutex>
#include <stack>

struct empty_stack : std::exception
{
    const char* what() const throw();
};

template<typename T>
class threadsafe_stack
{
public:
    threadsafe_stack();
    threadsafe_stack(const threadsafe_stack&);  // 拷贝构造
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;  // 不允许赋值操作
    
    bool empty() const;
    void push(T new_value);
    std::shared_ptr<T> pop();
    void pop(T& value);

private:
    std::stack<T> data_;
    
};

#endif /* threadsafe_stack_hpp */
