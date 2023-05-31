#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define LOG_I(fmt,...)  spdlog::info("[{}] [{}] [{}] "#fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOG_D(fmt,...)  spdlog::debug("[{}] [{}] [{}] "#fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
