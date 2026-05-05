//stolen from skeeto on Reddit https://www.reddit.com/user/skeeto/

#include "cwd.h"

#if defined(__unix__) || defined(__APPLE__) || defined(__HAIKU__)
#include <errno.h>
#include <unistd.h>

enum cwd_result
cwd(char *buf, size_t len)
{
    if (!getcwd(buf, len)) {
        return errno == ERANGE ? CWD_ERANGE : CWD_FAILURE;
    }
    return CWD_SUCCESS;
}

#elif defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

enum cwd_result
cwd(char *buf, size_t len)
{
    DWORD n;
    WCHAR *wbuf;
    enum cwd_result r;

    if (!len || len > (DWORD)-1 || len > (size_t)-1/2) {
        return CWD_FAILURE;
    }

    wbuf = HeapAlloc(GetProcessHeap(), 0, len*2);
    if (!wbuf) {
        buf[0] = 0;
        return CWD_FAILURE;
    }

    n = GetCurrentDirectoryW(len, wbuf);
    if (!n) {
        buf[0] = 0;
        r = CWD_FAILURE;
    } else if (n >= len) {
        buf[0] = 0;
        r = CWD_ERANGE;
    } else {
        WideCharToMultiByte(CP_UTF8, 0, wbuf, -1, buf, len, 0, 0);
        r = CWD_SUCCESS;
    }

    HeapFree(GetProcessHeap(), 0, wbuf);
    return r;
}

#else
#error cwd() not implemented
#endif
