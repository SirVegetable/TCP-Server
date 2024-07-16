#include <thread> 
#include <queue>
#include <mutex> 
#include <memory> 

#pragma once

class ThreadPool{
    public:
        explicit ThreadPool(std::size_t thread_count =  std::thread::hardware_concurrency()); 
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete; 
        ThreadPool& operator=(ThreadPool&& )= delete; 
        ~ThreadPool(); 

    public: 
        using work_item = std::function<void(void)> ;

        void start_work(work_item w_item); 
        void stop(); 
    private:
        using work_item_ptr = std::unique_ptr<work_item>; 
        using work_queue = std::queue<work_item_ptr>; 
        using Pool = std::vector<std::thread>; 
        work_queue _queue;
        std::mutex _queue_lock; 
        std::condition_variable _condition; 
        bool _check = false; 
        Pool _pool;
        bool stopped = false; 


}; 