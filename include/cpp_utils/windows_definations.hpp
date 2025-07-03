#pragma once
#if defined( _WIN32 ) || defined( _WIN64 )
# include <windows.h>
#endif
namespace cpp_utils
{
#if defined( _WIN32 ) || defined( _WIN64 )
    namespace mouse
    {
        inline constexpr DWORD button_left{ FROM_LEFT_1ST_BUTTON_PRESSED };
        inline constexpr DWORD button_middle{ FROM_LEFT_2ND_BUTTON_PRESSED };
        inline constexpr DWORD button_right{ RIGHTMOST_BUTTON_PRESSED };
        inline constexpr DWORD click{ 0x0000 };
        inline constexpr DWORD click_double{ DOUBLE_CLICK };
        inline constexpr DWORD move{ MOUSE_MOVED };
        inline constexpr DWORD wheel_height{ MOUSE_HWHEELED };
        inline constexpr DWORD wheel{ MOUSE_WHEELED };
    }
    namespace keyboard
    {
        inline constexpr DWORD right_alt_press{ RIGHT_ALT_PRESSED };
        inline constexpr DWORD left_alt_press{ LEFT_ALT_PRESSED };
        inline constexpr DWORD right_ctrl_press{ RIGHT_CTRL_PRESSED };
        inline constexpr DWORD left_ctrl_press{ LEFT_CTRL_PRESSED };
        inline constexpr DWORD shift_press{ SHIFT_PRESSED };
        inline constexpr DWORD num_lock_on{ NUMLOCK_ON };
        inline constexpr DWORD scroll_lock_on{ SCROLLLOCK_ON };
        inline constexpr DWORD caps_lock_on{ CAPSLOCK_ON };
        inline constexpr DWORD enhanced_key{ ENHANCED_KEY };
    }
    namespace console_handle_flag
    {
        inline constexpr DWORD std_input{ STD_INPUT_HANDLE };
        inline constexpr DWORD std_output{ STD_OUTPUT_HANDLE };
        inline constexpr DWORD std_error{ STD_ERROR_HANDLE };
    }
    namespace console_text
    {
        inline constexpr WORD foreground_red{ FOREGROUND_RED };
        inline constexpr WORD foreground_green{ FOREGROUND_GREEN };
        inline constexpr WORD foreground_blue{ FOREGROUND_BLUE };
        inline constexpr WORD foreground_white{ FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
        inline constexpr WORD foreground_intensity{ FOREGROUND_INTENSITY };
        inline constexpr WORD background_red{ BACKGROUND_RED };
        inline constexpr WORD background_green{ BACKGROUND_GREEN };
        inline constexpr WORD background_blue{ BACKGROUND_BLUE };
        inline constexpr WORD background_white{ BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE };
        inline constexpr WORD background_intensity{ BACKGROUND_INTENSITY };
        inline constexpr WORD lvb_leading_byte{ COMMON_LVB_LEADING_BYTE };
        inline constexpr WORD lvb_trailing_byte{ COMMON_LVB_TRAILING_BYTE };
        inline constexpr WORD lvb_grid_horizontal{ COMMON_LVB_GRID_HORIZONTAL };
        inline constexpr WORD lvb_grid_lvertical{ COMMON_LVB_GRID_LVERTICAL };
        inline constexpr WORD lvb_grid_rvertical{ COMMON_LVB_GRID_RVERTICAL };
        inline constexpr WORD lvb_reverse_video{ COMMON_LVB_REVERSE_VIDEO };
        inline constexpr WORD lvb_underscore{ COMMON_LVB_UNDERSCORE };
        inline constexpr WORD lvb_sbcsdbcs{ COMMON_LVB_SBCSDBCS };
    }
    namespace window_state
    {
        inline constexpr UINT hide{ SW_HIDE };
        inline constexpr UINT show{ SW_SHOW };
        inline constexpr UINT show_without_activating{ SW_SHOWNA };
        inline constexpr UINT show_default{ SW_SHOWDEFAULT };
        inline constexpr UINT show_normal{ SW_SHOWNORMAL };
        inline constexpr UINT show_normal_without_activating{ SW_SHOWNOACTIVATE };
        inline constexpr UINT minimize{ SW_SHOWMINIMIZED };
        inline constexpr UINT minimize_and_activate_next_window_with_z_order{ SW_MINIMIZE };
        inline constexpr UINT minimize_without_activating{ SW_SHOWMINNOACTIVE };
        inline constexpr UINT minimize_force{ SW_FORCEMINIMIZE };
        inline constexpr UINT maximize{ SW_SHOWMAXIMIZED };
        inline constexpr UINT restore{ SW_RESTORE };
    }
    namespace service
    {
        inline constexpr DWORD auto_start{ SERVICE_AUTO_START };
        inline constexpr DWORD boot_start{ SERVICE_BOOT_START };
        inline constexpr DWORD demand_start{ SERVICE_DEMAND_START };
        inline constexpr DWORD system_start{ SERVICE_SYSTEM_START };
        inline constexpr DWORD disabled_start{ SERVICE_DISABLED };
    }
    namespace registry
    {
        inline const auto hkey_classes_root{ HKEY_CLASSES_ROOT };
        inline const auto hkey_current_user{ HKEY_CURRENT_USER };
        inline const auto hkey_current_config{ HKEY_CURRENT_CONFIG };
        inline const auto hkey_local_machine{ HKEY_LOCAL_MACHINE };
        inline const auto hkey_users{ HKEY_USERS };
        inline constexpr DWORD binary_type{ REG_BINARY };
        inline constexpr DWORD dword_type{ REG_DWORD };
        inline constexpr DWORD dword_big_endian_type{ REG_DWORD_BIG_ENDIAN };
        inline constexpr DWORD dword_little_endian_type{ REG_DWORD_LITTLE_ENDIAN };
        inline constexpr DWORD qword_type{ REG_QWORD };
        inline constexpr DWORD qword_little_endian{ REG_QWORD_LITTLE_ENDIAN };
        inline constexpr DWORD expandable_string_type{ REG_EXPAND_SZ };
        inline constexpr DWORD multi_string_type{ REG_MULTI_SZ };
        inline constexpr DWORD string_type{ REG_SZ };
        inline constexpr DWORD link_type{ REG_LINK };
        inline constexpr DWORD none_type{ REG_NONE };
    }
#else
# error "must be compiled on the windows os"
#endif
}