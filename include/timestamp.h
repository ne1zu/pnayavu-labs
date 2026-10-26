#pragma once
#include <ctime>
#include <string>

inline std::string currentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm local = {};
#ifdef _WIN32
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local);
    return buffer;
}
