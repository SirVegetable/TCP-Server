#include <thread> 
#include <queue>
#include <mutex> 


class ThreadPool{
    public:
        ThreadPool(std::size_t thread_count = std::thread::hardware_concurrency()){}
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete; 
        ThreadPool& operator=(ThreadPool&& )= delete; 
        ~ThreadPool(){}

    public: 
        using work_item = std::function<void(void)> ;

        void start_work(work_item wItem); 
        
    private:
        using work_item_ptr = std::unique_ptr<work_item>; 
        using work_queue = std::queue<work_item_ptr>; 

        work_queue _queue;
        std::mutex _queue_lock; 
        std::condition_variable _condition; 
        bool _check; 



}; 