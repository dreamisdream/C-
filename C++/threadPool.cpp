#include <atomic>
#include <vector>
#include <thread>
#include <queue>
#include <iostream>
// #pragma region 线程池
// class threadPool{
// private:
//     std::atomic_bool done;
//     thread_safe_queue<std::function<void()> > work_queue;
//     std::vector<std::thread> thread;
//     join_threads joiner;
    
//     void worker_thread(){
//         while(!done){
//             std::function<void() > task;
//             if(work_queue.try_pop(task)){
//                 task();
//             }else
//             {  
//                 std::this_thread::yield();
//             }
            
//         }
//     }
// public:
//     threadPool()
//         :done(false)
//         ,joiner(threads){
//             // 初始化线程池
//             unsigned const thread_count = std::thread::hardware_concurrency();
//             try{
//                 for(unsigned i = 0;i<thread_count;++i){
//                     thread.push_back(std::thread(&threadPool::work_thread,this));
//                 }
//             }catch(...){
//                 done = true;
//                 throw;
//             }
//         }
//     ~threadPool(){
//         done =true;
//     }
//     template <class functionType>
//     void submit(functionType f){
//         work_queue.push(std::function<void() >(f));
//     }
// }
// #pragma endregion
