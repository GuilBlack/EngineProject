#include "DXHEngine.h"

DXH::DXHEngine* DXH::DXHEngine::s_App = nullptr;

DXH::DXHEngine::DXHEngine()
{
	s_App = this;
}

void DXH::DXHEngine::Run()
{
	VS_DB_OUT_W(L"Welcome to DXHEngine!\n");
	while (m_IsRunning) 
	{
		break;
	}
}
