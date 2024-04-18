#include "TaskListExecutor.h"

#include <thread>


void TaskListExecutor::execute_tasks(std::vector<Task> tasks, size_t num_threads)
{
	std::atomic<size_t> exec_index{ 0 };
	std::vector<std::thread> thds;
	thds.reserve(20);

	for (size_t i = 0; i < num_threads; i++)
	{
		thds.emplace_back(
			[&tasks, &exec_index]() {
				TaskListExecutor::thread_main(exec_index, tasks);
			}
			
		);
	}

	for (auto &it: thds)
	{
		it.join();
	}
}

void TaskListExecutor::thread_main(std::atomic<size_t>& exec_index, const std::vector<Task>& tasks)
{
	size_t index = get_index(exec_index);
	while (index < tasks.size())
	{
		tasks[index]();
		index = get_index(exec_index);
	}
}

size_t TaskListExecutor::get_index(std::atomic<size_t>& exec_index)
{
	return exec_index++;
}
