#pragma once
#include <pch.h>

#if defined(_DEBUG) || defined(DEBUG)

#include <debugapi.h>

// Debug output for std::string.
#define VS_DB_OUT_A( s )								\
{														\
	std::stringstream ss;								\
	ss << s;											\
	OutputDebugStringA( ss.str().c_str() );				\
}

// Debug output for std::wstring.
#define VS_DB_OUT_W( s )								\
{														\
	std::wstringstream ss;								\
	ss << s;											\
	OutputDebugStringW( ss.str().c_str() );				\
}

// Asserts if the HRESULT is not S_OK.
#define ASSERT_HRESULT(x)								\
{														\
	HRESULT dhr = (x);									\
	if (FAILED(dhr))									\
	{													\
		_com_error err(dhr);							\
		VS_DB_OUT_W(err.ErrorMessage() << L'\n');		\
		assert(false);									\
	}													\
}

#else
#define VS_DB_OUT_A( s )
#define VS_DB_OUT_W( s )
#define ASSERT_HRESULT(x) x
#endif // DEBUG || _DEBUG

// Application instance handle
#define APP_PROC() GetModuleHandle(NULL)

// Release COM object and set pointer to nullptr.
#define RELEASE_PTR(x) { if (x) { x->Release(); x = nullptr; } }
// Delete pointer and set pointer to nullptr.
#define DELETE_PTR(x) { if (x) { delete x; x = nullptr; } }

// Maximum number of game objects.
constexpr const size_t MAX_GO_COUNT = 1000;
