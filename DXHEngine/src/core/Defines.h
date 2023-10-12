#pragma once
#include <Windows.h>
#include <pch.h>

#if defined(_DEBUG) || defined(DEBUG)

#include <debugapi.h>

#define VS_DB_OUT_A( s )								\
{														\
	std::stringstream ss;								\
	ss << s;											\
	OutputDebugStringA( ss.str().c_str() );	\
}

#define VS_DB_OUT_W( s )								\
{														\
	std::wstringstream ss;								\
	ss << s;											\
	OutputDebugStringW( ss.str().c_str() );	\
}

#define ASSERT_HRESULT(x)							\
{													\
	HRESULT dhr = (x);								\
	if (FAILED(dhr))								\
	{												\
		_com_error err(dhr);						\
		VS_DB_OUT_W(err.ErrorMessage() << L'\n');	\
		assert(false);								\
	}												\
}

#else
#define VS_DB_OUT( s )
#endif // DEBUG || _DEBUG