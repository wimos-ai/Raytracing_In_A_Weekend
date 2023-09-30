#pragma once


#ifdef _WIN32 

    class semaphore
    {
    private:
        const void* m_win_hdl; //Defining as void* so I don't need to worry abt windows headers 
    public:
        semaphore();

        void release();

        void acquire();

        bool try_acquire();
    };

#else
    #include <mutex>
    #include <condition_variable>
    class semaphore
    {
        std::mutex m_mutex;
        std::condition_variable m_condition;
        unsigned long count_ = 0; // Initialized as locked.

    public:
        semaphore() = default;

        void release();

        void acquire();

        bool try_acquire();
    };
#endif // _WIN32 


