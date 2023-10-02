#pragma once


#ifdef _WIN32 

    class Semaphore
    {
    public:
        Semaphore();

        void release();

        void acquire();

        bool try_acquire();

    private:
        const void* m_win_hdl; //Defining as void* so I don't need to worry abt contamination from windows headers 
    };

#else
    #include <mutex>
    #include <condition_variable>
    class Semaphore
    {
    public:
        Semaphore() = default;

        void release();

        void acquire();

        bool try_acquire();
    private:
        std::mutex m_mutex;
        std::condition_variable m_condition;
        unsigned long count_ = 0; // Initialized as locked.
    };
#endif // _WIN32 


