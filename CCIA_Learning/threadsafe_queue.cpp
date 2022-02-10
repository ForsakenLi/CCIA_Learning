//
//  threadsafe_queue.cpp
//  CCIA_Learning - Chapter 4.1
//
//  Created by lmc on 2022/1/31.
//

#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class threadsafe_queue {
private:
    mutable std::mutex mu_; // mu_必须声明为mutable(无视const限制)否则在常成员函数和拷贝构造函数中无法lock住
    std::queue<T> data_queue_;
    std::condition_variable data_cond_;
    
public:
    threadsafe_queue(){}
    threadsafe_queue(threadsafe_queue const& other) {
        std::lock_guard<std::mutex> lk(other.mu_);
        data_queue_ = other.data_queue_;
    }
    threadsafe_queue& operator=(const threadsafe_queue&) = delete;
    
    void push(T new_value) {
        std::lock_guard<std::mutex> lk(mu_);
        data_queue_.push(new_value);
        data_cond_.notify_one();
    }
    
    void wait_and_pop(T& value){
        std::lock_guard<std::mutex> lk(mu_);
        data_cond_.wait(lk, [this]{return !data_queue_.empty();});  // this指针在lambda内语义也是隐含的
        // 上面这行等价于while(![this]{return !data_queue_.empty();}) {wait(lk);}
        // 即在queue非空时才wait，注意有两个"!"
        value = data_queue_.front();
        data_queue_.pop();
    }
    
    std::shared_ptr<T> wait_and_pop() {
        std::lock_guard<std::mutex> lk(mu_);
        data_cond_.wait(lk, [this]{return !data_queue_.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue_.front()));
        data_queue_.pop();
        return res;
    }
    
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mu_);
        if (data_queue_.empty())
            return false;
        value = data_queue_.front();
        data_queue_.pop();
        return true;
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lk(mu_);
        return data_queue_.empty();
    }
};
