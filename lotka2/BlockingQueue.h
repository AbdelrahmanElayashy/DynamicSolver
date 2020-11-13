#pragma once

#include <mutex>
#include <queue>
#include <atomic>
#include <chrono>
#include <utility>
#include <stdexcept>
#include <type_traits>
#include <condition_variable>
#include "MyLibrary.h"


template<typename T>
class BlockingQueue
{
public:
	template<typename Q = T>
	typename std::enable_if<std::is_copy_constructible<Q>::value, void>::type
		push(const T& item)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_queue.push(item);
		}
		m_ready.notify_one();
	}

	template<typename Q = T>
	typename std::enable_if<std::is_move_constructible<Q>::value, void>::type
		push(T&& item)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_queue.emplace(std::forward<T>(item));
		}
		m_ready.notify_one();
	}

	template<typename Q = T>
	typename std::enable_if<std::is_copy_constructible<Q>::value, bool>::type
		try_push(const T& item)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex, std::try_to_lock);
			if (!lock)
				return false;
			m_queue.push(item);
		}
		m_ready.notify_one();
		return true;
	}

	template<typename Q = T>
	typename std::enable_if<std::is_move_constructible<Q>::value, bool>::type
		try_push(T&& item)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex, std::try_to_lock);
			if (!lock)
				return false;
			m_queue.emplace(std::forward<T>(item));
		}
		m_ready.notify_one();
		return true;
	}

	template<typename Q = T>
	typename std::enable_if<
		std::is_copy_assignable<Q>::value &&
		!std::is_move_assignable<Q>::value, bool>::type
		pop(T& item)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		while (m_queue.empty() && !m_done)
			m_ready.wait(lock);
		if (m_queue.empty())
			return false;
		item = m_queue.front();
		m_queue.pop();
		return true;
	}

	template<typename Q = T>
	typename std::enable_if<std::is_move_assignable<Q>::value, bool>::type
		pop(T& item)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		while (m_queue.empty() && !m_done)
			m_ready.wait(lock);
		if (m_queue.empty())
			return false;
		item = std::move(m_queue.front());
		m_queue.pop();
		return true;
	}

	template<typename Q = T>
	typename std::enable_if<
		std::is_copy_assignable<Q>::value &&
		!std::is_move_assignable<Q>::value, bool>::type
		try_pop(T& item)
	{
		std::unique_lock<std::mutex> lock(m_mutex, std::try_to_lock);
		if (!lock || m_queue.empty())
			return false;
		item = m_queue.front();
		m_queue.pop();
		return true;
	}

	template<typename Q = T>
	typename std::enable_if<std::is_move_assignable<Q>::value, bool>::type
		try_pop(T& item)
	{
		std::unique_lock<std::mutex> lock(m_mutex, std::try_to_lock);
		if (!lock || m_queue.empty())
			return false;
		item = std::move(m_queue.front());
		m_queue.pop();
		return true;
	}

	void done() noexcept
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_done = true;
		}
		m_ready.notify_all();
	}

	bool empty() const noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.empty();
	}

	unsigned int size() const noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.size();
	}

private:
	std::queue<T> m_queue;
	mutable std::mutex m_mutex;
	std::condition_variable m_ready;
	bool m_done = false;
};


