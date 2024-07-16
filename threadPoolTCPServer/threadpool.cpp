#include <thread>
#include <iostream>
#include <chrono>
#include <utility> 
#include <memory> 
#include "threadpool.hpp"



ThreadPool::ThreadPool(std::size_t thread_count )
{   
    if(!thread_count)
    {
        throw std::invalid_argument("thread count error: must be non-zero"); 
    }
    for(auto i = 0; i < thread_count; ++i)
    {
        _pool.push_back(std::thread([this]()
        {
            while(true)
            {
                work_item_ptr work{nullptr}; 
                {
                    std::unique_lock guard(_queue_lock); 
                    _condition.wait(guard,[this](){ return _check;});
                    _check = false;
                    work = std::move(_queue.front()); 
                    _queue.pop();
                    
                }
                if(!work)
                {
                    stop(); 
                    break; 
                }

                (*work)(); 
            }
            

        })); 
    }
}

ThreadPool::~ThreadPool()
{
   stop(); 
}

void ThreadPool::start_work(work_item w_item)
{
    auto item = std::make_unique<work_item>(std::move(w_item)); 
    {
        std::unique_lock<std::mutex> guard(_queue_lock); 
        _queue.push(std::move(item));
        _check = true;
    }
    _condition.notify_one(); 

}
void ThreadPool::ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> guard(_queue_lock);
        if(stopped) return; 
        _queue.push(work_item_ptr{nullptr}); 
        _check = true;
        stopped = true; 
        _condition.notify_one(); 
    }
    for(auto& i: _pool)
    {
        i.join(); 
    }
}

