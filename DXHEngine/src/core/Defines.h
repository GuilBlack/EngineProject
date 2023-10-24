#pragma once
#include <pch.h>

typedef DirectX::XMFLOAT3 Vector3;
typedef DirectX::XMFLOAT4 Quaternion;
typedef DirectX::XMMATRIX Matrix;

#if defined(_DEBUG) || defined(DEBUG)

#include <debugapi.h>

// Debug output for std::string.
#define VS_DB_OUT_A( s )								\
{														\
	std::stringstream ss;								\
	ss << s;											\
	OutputDebugStringA( ss.str().c_str() );	\
}

// Debug output for std::wstring.
#define VS_DB_OUT_W( s )								\
{														\
	std::wstringstream ss;								\
	ss << s;											\
	OutputDebugStringW( ss.str().c_str() );	\
}

// Asserts if the HRESULT is not S_OK.
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
#define VS_DB_OUT_A( s )
#define VS_DB_OUT_W( s )
#define ASSERT_HRESULT(x) x
#endif // DEBUG || _DEBUG

// Application instance handle
#define APP_PROC() GetModuleHandle(NULL)