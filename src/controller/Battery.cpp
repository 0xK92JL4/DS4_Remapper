/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  Battery.cpp                                            ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/25 14:18:32 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/31 02:30:32 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "controller/Battery.hpp"
#include "core/Config.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                          Constructor/Destructor                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

Battery::Battery()
{
	_fd_capacity = open(Config::BATTERY_CAPACITY, O_RDONLY);
	_fd_status = open(Config::BATTERY_STATUS, O_RDONLY);

	if (_fd_capacity < 0 || _fd_status < 0)
		throw std::runtime_error("Battery: failed to open sysfs files");
}

Battery::~Battery()
{
	if (_fd_capacity >= 0) close(_fd_capacity);
	if (_fd_status >= 0) close(_fd_status);
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Private                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

std::string Battery::Read(int fd)
{
	char buf[64];

	ssize_t n = pread(fd, buf, sizeof(buf) - 1, 0);
	if (n <= 0)
		return "";

	buf[n] = '\0';

	std::string out(buf);

	if (!out.empty() && out.back() == '\n')
		out.pop_back();

	return out;
}

int Battery::ParseCapacity(const std::string& s)
{
	if (s.empty())
		return -1;

	return std::atoi(s.c_str());
}

BatteryStatus Battery::ParseStatus(const std::string& s)
{
	if (s == "Charging"    )  return BatteryStatus::Charging;
	if (s == "Discharging" )  return BatteryStatus::Discharging;
	if (s == "Full"        )  return BatteryStatus::Full;
	if (s == "Not charging")  return BatteryStatus::NotCharging;

	return BatteryStatus::Unknown;
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Public                                     │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void Battery::Update()
{
    std::string cap_str = Read(_fd_capacity);
    std::string status_str = Read(_fd_status);

    _capacity = ParseCapacity(cap_str);
    _status   = ParseStatus(status_str);
}

int				Battery::GetCapacity() const { return _capacity; }
BatteryStatus	Battery::GetStatus() const   { return _status;   }
