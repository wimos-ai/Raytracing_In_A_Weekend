#include "ThreadPool.hpp"
#include <cassert>

void ThreadPool::close_pool()
{
	if (this->m_worker_thds.size() == 0)
	{
		return;
	}
	//Refuse new tasks
	m_accepting = false;

	//Wait on all tasks
	while (m_tasks.size() > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	}

	m_running = false;

	//release semaphore to wake workers
	for (size_t idx = 0; idx < m_worker_thds.size(); idx++)
	{
		m_task_sem.release();
	}
}

void ThreadPool::thread_main()
{
	while (true) {
		//Wait for a task to become available
		m_task_sem.acquire();

		//If the pool completed while waiting, terminate
		if (!m_running)
		{
			return;
		}

		//Lock the tasks mutex in case multiple tasks become available at once
		m_tasks_mutex.lock();

		assert(!m_tasks.empty()); //Debug

		//Claim the top task
		Task t = m_tasks.front();
		m_tasks.pop_front();

		//Allow other threads to claim tasks
		m_tasks_mutex.unlock();

		//Execute task
		t();
	}
}

ThreadPool::ThreadPool(size_t num_thds) : m_running(true), m_accepting(true), m_task_sem(0)
{
	m_worker_thds.reserve(num_thds);

	for (size_t x = 0; x < num_thds; x++) {
		m_worker_thds.emplace_back(
			[this]() {
				this->thread_main();
			}
		);
	}
}

ThreadPool::~ThreadPool() {
	close_pool();

	//Join the threads
	for (auto& thd : m_worker_thds) {
		if (thd.joinable())
		{
			thd.join();
		}
	}
}

void ThreadPool::submit_task(Task t)
{
	std::lock_guard<std::mutex> lck(m_tasks_mutex);
	m_tasks.push_back(t);
	m_task_sem.release();
}

void ThreadPool::clear_tasks()
{
	
	std::lock_guard<std::mutex> lck(m_tasks_mutex);
	// Swap for empty tasks so we do not need to mess with the semaphore
	for (auto& task: m_tasks)
	{
		Task emptyTask = []() {};
		std::swap(task, emptyTask);
	}

}