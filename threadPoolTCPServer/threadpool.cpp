#include <thread>
#include <iostream>
#include <chrono>
#include <utility> 
#include <memory> 
#include "threadpool.hpp"



ThreadPool::ThreadPool(std::size_t thread_count)
{   
    if(!thread_count)
    {
        throw std::invalid_argument("thread count error: must be non-zero"); 
    }
    for(auto i = 0; i < thread_count; ++i)
    {
        _pool.push_back(std::thread([this]()
        {
            std::cout << "thread: " << std::this_thread::get_id() << " begins... "<< std::endl; 
            using namespace std::chrono_literals; 
            std::this_thread::sleep_for(1s); 
        })); 
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock guard(_queue_lock); 
        _queue.push(work_item_ptr{nullptr});
        _condition.notify_one(); 
    }
    for(auto& t: _pool)
    {
        t.join(); 
    }
}

void ThreadPool::start_work(work_item w_item)
{
    auto item = std::make_unique<work_item>(std::move(w_item)); 
    {
        std::unique_lock guard(_queue_lock); 
        _queue.push(std::move(item));
        _check = true;
    }
    _condition.notify_one(); 

}

