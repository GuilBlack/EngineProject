#pragma once
#include "pch.h"

namespace DXH 
{
	class DXHEngine 
	{
	public:
		DXHEngine();
		virtual ~DXHEngine() {}
		void Run();

		[[nodiscard]] static DXHEngine* GetInstance() noexcept { return s_App; }

	private:
		static DXHEngine* s_App;
		bool m_IsRunning = true;
	};

	DXHEngine* CreateDXHEngine();
}

