#pragma once


#include <functional>
#include <queue>
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>

#include "Semaphore.h"

using Task = std::function<void(void)>;

/*
* Basic thread pool task.
* Submit tasks
* Tasks are guarenteed to be completed before object destruction
*/
class ThreadPool
{
public:
	ThreadPool(size_t num_thds = std::thread::hardware_concurrency());
	~ThreadPool();
	bool submit_task(Task t); //True on success, false on failure
private:
	void close_pool();
	void thread_main();
private:
	std::mutex m_tasks_mutex;
	std::queue<Task> m_tasks;
	std::vector<std::thread> m_worker_thds;
	semaphore m_task_sem;
	std::atomic_bool m_running;
	std::atomic_bool m_accepting;
};

