#include <iostream>
#include <ctime>
#include <mutex>

#include "utils/utils.hpp"

static std::mutex	log_mtx;

static std::string	timestamp(void) noexcept
{
	std::time_t	now;
	char		buf[32];

	now = std::time(nullptr);
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
	return (buf);
}

void				log(const std::string& msg, LogLevel level) noexcept
{
	std::lock_guard<std::mutex>	lock(log_mtx);
	const char	*labels[] = {"", "ERROR", "WARN", "INFO", "DEBUG"};
	std::string	clean;

	if (level > current_level)
		return;
	clean = msg;
	if (!clean.empty() && clean.back() == '\n')
		clean.pop_back();

	std::cerr << "[" << timestamp() << "] [" << labels[level] << "] " << clean << std::endl;
}
