#define WINVER       0x0601
#define _WIN32_WINNT 0x0601
#define NOCOMM
#define NOSOUND
#define NORPC
#include <windows.h>
#include <wininet.h>
namespace scltk
{
    namespace details_
    {
        auto clear_wininet_proxy() noexcept
        {
            INTERNET_PER_CONN_OPTION options[ 5 ];
            options[ 0 ].dwOption       = INTERNET_PER_CONN_FLAGS_UI;
            options[ 0 ].Value.dwValue  = 0;
            options[ 1 ].dwOption       = INTERNET_PER_CONN_PROXY_SERVER;
            options[ 1 ].Value.pszValue = nullptr;
            options[ 2 ].dwOption       = INTERNET_PER_CONN_PROXY_BYPASS;
            options[ 2 ].Value.pszValue = nullptr;
            options[ 3 ].dwOption       = INTERNET_PER_CONN_AUTOCONFIG_URL;
            options[ 3 ].Value.pszValue = nullptr;
            options[ 4 ].dwOption       = INTERNET_PER_CONN_AUTODISCOVERY_FLAGS;
            options[ 4 ].Value.dwValue  = 0;
            INTERNET_PER_CONN_OPTION_LIST list{
              .dwSize{ sizeof( list ) }, .pszConnection{ nullptr }, .dwOptionCount{ 5 }, .dwOptionError{ 0 }, .pOptions{ options } };
            InternetSetOptionW( nullptr, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, sizeof( list ) );
            InternetSetOptionW( nullptr, INTERNET_OPTION_SETTINGS_CHANGED, nullptr, 0 );
            InternetSetOptionW( nullptr, INTERNET_OPTION_REFRESH, nullptr, 0 );
        }
    }
}