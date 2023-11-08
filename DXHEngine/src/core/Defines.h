#pragma once

#if defined(_DEBUG) || defined(DEBUG)

#include <crtdbg.h>
#include <sstream>

// Debug output for std::string.
#define VS_DB_OUT_A( s )                                    \
{                                                           \
    std::stringstream ss;                                   \
    ss << s;                                                \
    OutputDebugStringA( ss.str().c_str() );                 \
}

// Debug output for std::wstring.
#define VS_DB_OUT_W( s )                                    \
{                                                           \
    std::wstringstream ss;                                  \
    ss << s;                                                \
    OutputDebugStringW( ss.str().c_str() );                 \
}

// Asserts if the HRESULT is not S_OK.
#define ASSERT_HRESULT(x)                                   \
{                                                           \
    HRESULT dhr = (x);                                      \
    if (FAILED(dhr))                                        \
    {                                                       \
        if (FACILITY_WINDOWS == HRESULT_FACILITY(dhr))      \
            dhr = HRESULT_CODE(dhr);                        \
                                                            \
        TCHAR* szErrMsg;                                    \
        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dhr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&szErrMsg, 0, NULL) != 0) \
        {                                                   \
            VS_DB_OUT_W(TEXT("%s") << szErrMsg);            \
            LocalFree(szErrMsg);                            \
        }                                                   \
        else VS_DB_OUT_W(L"Could not find a description for error " << dhr << L'\n'); \
        assert(false);                                      \
    }                                                       \
}

// Replace new to check for memory leaks
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

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
constexpr const int GRID_SIZE = 10;
