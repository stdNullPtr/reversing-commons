#pragma once
#include <string>

namespace commons::logger
{
#ifndef NDEBUG
    template <typename... Args>
    void LOG(const std::wstring& fmt, Args&&... args)
    {
        wprintf(fmt.c_str(), std::forward<Args>(args)...);
    }

    template <typename... Args>
    void LOG(const std::string& fmt, Args&&... args)
    {
        printf(fmt.c_str(), std::forward<Args>(args)...);
    }
#else
    template <typename... Args>
    void LOG(const std::wstring& fmt, Args&&... args)
    {
    }
#endif
}
