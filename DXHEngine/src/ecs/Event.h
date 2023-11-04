#pragma once
#include <functional>
#include <vector>
template<typename ...T> class Event
{
	Event() : m_Functions() {};

	inline void operator += (const std::function<void(Args...)>& function) 
	{
		m_Functions.push_back(function);
	}
	inline void operator += (const Event& event)
	{
		m_Functions.insert(m_Functions.end(), event.m_Functions.begin(), event.m_Functions.end());
	}
	inline void operator -= (const std::function<void(Args...)>& function)
	{
		m_Functions.erase(std::remove(m_Functions.begin(), m_Functions.end(), function), m_Functions.end());
	}

	void operator()(Args... args)
	{
		for (auto& function : m_Functions)
		{
			function(args...);
		}
	}

	inline void Invoke(Args... args)
	{
		operator()(args...);
	}
	inline void Clear()
	{
		m_Functions.clear();
	}

private :
	std::vector<std::function<void(Args...)>> m_Functions;
};
