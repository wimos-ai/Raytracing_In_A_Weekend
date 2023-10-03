#include "Semaphore.h"

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include <winbase.h>
#include<synchapi.h>
#include <limits>
#include <cstdio>

Semaphore::Semaphore(): m_win_hdl(CreateSemaphoreA(NULL,0,std::numeric_limits<LONG>::max(), NULL))
{
}

void Semaphore::release() {
	if (ReleaseSemaphore((HANDLE)m_win_hdl, 1, NULL) == 0)
	{
		char err_msg[255];
		sprintf_s(err_msg, "Error in Semaphore::release. ReleaseSemaphore failed with %lx.", GetLastError());
		perror(err_msg);
		exit(-1);
	}
}

void Semaphore::acquire() {
	auto ret_val = WaitForSingleObject((HANDLE)m_win_hdl, INFINITE);
	if (ret_val != WAIT_OBJECT_0)
	{
		char err_msg[255];
		sprintf_s(err_msg, "Error in Semaphore::acquire. WaitForSingleObject failed with %lx.", ret_val);
		perror(err_msg);
		exit(-1);
	}
}

bool Semaphore::try_acquire() {
	auto ret_val = WaitForSingleObject((HANDLE)m_win_hdl, 0);
	if (ret_val == WAIT_OBJECT_0)
	{
		return true;
	}
	else if (ret_val == WAIT_TIMEOUT)
	{
		return false;
	}
	else {
		char err_msg[255];
		sprintf_s(err_msg, "Error in Semaphore::try_acquire. WaitForSingleObject failed with %lx.", ret_val);
		perror(err_msg);
		exit(-1);
		return false;
	}
}


#else

void Semaphore::release() {
	std::lock_guard<decltype(m_mutex)> lock(m_mutex);
	++count_;
	m_condition.notify_one();
}

void Semaphore::acquire() {
	std::unique_lock<decltype(m_mutex)> lock(m_mutex);
	while (!count_) // Handle spurious wake-ups.
		m_condition.wait(lock);
	--count_;
}

bool Semaphore::try_acquire() {
	std::lock_guard<decltype(m_mutex)> lock(m_mutex);
	if (count_) {
		--count_;
		return true;
	}
	return false;
}


#endif // _WIN32







