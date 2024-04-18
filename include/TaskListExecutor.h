#pragma once
#include <vector>
#include <functional>
#include <atomic>

using Task = std::function<void(void)>;

class TaskListExecutor
{
public:
	static void execute_tasks(std::vector<Task> tasks, size_t num_threads);
private:
	static void thread_main(std::atomic<size_t>& exec_index,const std::vector<Task>& tasks);
	static size_t get_index(std::atomic<size_t>& exec_index);
};

