#define WINVER       0x0601
#define _WIN32_WINNT 0x0601
#define NOCOMM
#define NOSOUND
#define NORPC
#include <windows.h>
#include <winhttp.h>
namespace scltk
{
    namespace details_
    {
        auto clear_winhttp_proxy() noexcept
        {
            WINHTTP_PROXY_INFO proxy_info{
              .dwAccessType{ WINHTTP_ACCESS_TYPE_NO_PROXY }, .lpszProxy{ nullptr }, .lpszProxyBypass{ nullptr } };
            WinHttpSetDefaultProxyConfiguration( &proxy_info );
        }
    }
}