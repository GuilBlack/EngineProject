#pragma once
#include <functional>
#include <vector>
template<typename ...T> class Event
{
	Event() {};

	inline void operator += (const std::function<void(Args...)>& function) 
	{
		m_functions.push_back(function);
	}
	inline void operator += (const Event& event)
	{
		m_functions.insert(m_functions.end(), event.m_functions.begin(), event.m_functions.end());
	}
	inline void operator -= (const std::function<void(Args...)>& function)
	{
		m_functions.erase(std::remove(m_functions.begin(), m_functions.end(), function), m_functions.end());
	}

	void operator()(Args... args)
	{
		for (auto& function : m_functions)
		{
			function(args...);
		}
	}

	inline void invoke(Args... args)
	{
		operator()(args...);
	}
	inline void clear()
	{
		m_functions.clear();
	}

private :
	std::vector<std::function<void(Args...)>> m_functions;
};
