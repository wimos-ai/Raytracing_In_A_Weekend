#pragma once


#include <functional>
#include <deque>
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>
#include <semaphore>
#include <limits>

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
	void submit_task(Task t); //True on success, false on failure
	void clear_tasks();
private:
	void close_pool();
	void thread_main();
private:
	std::mutex m_tasks_mutex;
	std::deque<Task> m_tasks;
	std::vector<std::thread> m_worker_thds;
	std::counting_semaphore<std::numeric_limits<int>::max()> m_task_sem;
	std::atomic_bool m_running;
	std::atomic_bool m_accepting;
};
