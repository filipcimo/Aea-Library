#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include "Pointer.hpp"
#include <thread>
#include <functional>



namespace aea
{
    class ThreadPool
    {
        public:
            ThreadPool() = default;
            explicit ThreadPool(const std::uint32_t& size);
            ThreadPool(const ThreadPool& obj) = delete;
            ThreadPool(ThreadPool&& obj) = delete;
            ~ThreadPool();

            ThreadPool& operator=(const ThreadPool& obj) = delete;
            ThreadPool& operator=(ThreadPool&& obj) = delete;

            bool running() const;
            std::uint32_t size() const;
            void reset();
            template<typename Function> void add(Function&& function);


        private:
            void execute(bool& run, std::function<void()>& function);
            std::uint32_t getFreeThread();


        private:
            bool isRunnning = false;

            aea::Pointer<std::thread> threads;
            aea::Pointer<bool> threadsToExecute;
            aea::Pointer<std::function<void()>> functionsToExecute;
    };


    ThreadPool::ThreadPool(const std::uint32_t& size)
    {
        if (size == 1)
        {
            threads = std::move(aea::Pointer<std::thread>(new std::thread));
            functionsToExecute = std::move(aea::Pointer<std::function<void()>>(new std::function<void()>));
            threadsToExecute = std::move(aea::Pointer<bool>(new bool));
        }

        else if (size > 1)
        {
            threads = std::move(aea::Pointer<std::thread>(new std::thread[size], size));
            functionsToExecute = std::move(aea::Pointer<std::function<void()>>(new std::function<void()>[size], size));
            threadsToExecute = std::move(aea::Pointer<bool>(new bool[size], size));
        }

        memset((void*) threadsToExecute.get(), 0, threadsToExecute.size());
        isRunnning = true;
    
        for (std::uint32_t i = 0; i < size; ++i)
        {
            threads.at(i) = std::move( std::thread(&ThreadPool::execute, this, std::ref(threadsToExecute.at(i)), std::ref(functionsToExecute.at(i))) );
        }
    }


    ThreadPool::~ThreadPool()
    {
        isRunnning = false;

        for (std::thread* singleThread = nullptr;
             aea::iterate_front(&singleThread, threads, threads.first()) != nullptr;)
        {
            singleThread->join();
        }
    }


    void ThreadPool::reset()
    {
        isRunnning = false;

        for (std::thread* singleThread = nullptr;
             aea::iterate_front(&singleThread, threads, threads.first()) != nullptr;)
        {
            singleThread->join();
        }

        threads.reset();
        threadsToExecute.reset();
        functionsToExecute.reset();
    }


    template<typename Function>
    void ThreadPool::add(Function&& function)
    {
        std::uint32_t freeThread = 0;
        while (freeThread == 0) { freeThread = getFreeThread(); }

        freeThread -= 1;
        functionsToExecute.at(freeThread) = std::move(std::function<void()>(function));
        threadsToExecute.at(freeThread) = true;
    }


    bool ThreadPool::running() const
    {
        if (threads.size() == 0) { return false; }
        
        for (std::thread* t = nullptr; aea::iterate_front(&t, threads, threads.first()) != nullptr;)
        {
            if (t->joinable() == false) { return false; }
        }

        return true;
    }


    std::uint32_t ThreadPool::size() const
    {
        return threads.size();
    }


    std::uint32_t ThreadPool::getFreeThread()
    {
        for (std::uint32_t i = 0; i < threadsToExecute.size(); ++i)
        {
            if (threadsToExecute.at(i) == false) { return (i + 1); }
        }

        return 0;
    }


    void ThreadPool::execute(bool& run, std::function<void()>& function)
    {
        while (isRunnning == true)
        {
            if (run == true)
            {
                function();
                run = false;
            }
        }
    }
}



#endif  // THREAD_POOL_HPP
