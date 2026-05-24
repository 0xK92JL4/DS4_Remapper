/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  LightBar.cpp                                           ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/21 23:48:14 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/24 02:35:35 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "Controller/LightBar.hpp"
#include "Config.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <algorithm>
#include <string>

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                          Constructor/Destructor                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

LightBar::LightBar(uint32_t color)
{
	_fd_r = open(Config::LED_R, O_WRONLY);
	_fd_g = open(Config::LED_G, O_WRONLY);
	_fd_b = open(Config::LED_B, O_WRONLY);
	_fd_global = open(Config::LED_GLOBAL, O_WRONLY);

	if (_fd_r < 0 || _fd_g < 0 || _fd_b < 0 || _fd_global < 0)
		throw std::runtime_error("LightBar: failed to open sysfs LED nodes");

	_color = color;
	apply();
}

LightBar::~LightBar()
{
    if (_fd_r >= 0) close(_fd_r);
    if (_fd_g >= 0) close(_fd_g);
    if (_fd_b >= 0) close(_fd_b);
    if (_fd_global >= 0) close(_fd_global);
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Private                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

bool LightBar::writeInt(int fd, uint8_t value)
{
	char buf[8];

	int len = snprintf(buf, sizeof(buf), "%u", value);
	if (len <= 0 || len >= (int)sizeof(buf))
		return false;

	return write(fd, buf, len) == len;
}

bool LightBar::apply()
{
	uint8_t r = (_color >> 16) & 0xFF;
	uint8_t g = (_color >> 8) & 0xFF;
	uint8_t b = (_color) & 0xFF;

	auto scale = [this](uint8_t v) -> int {
		return (v * _brightness) / 100;
	};

	bool OK = writeInt(_fd_r, scale(r))
		&& writeInt(_fd_g, scale(g))
		&& writeInt(_fd_b, scale(b));

	return OK;
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Public                                     │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

bool LightBar::toggle(std::optional<bool> on)
{
	_enabled = (on.has_value()) ? *on : !_enabled;
	return writeInt(_fd_global, _enabled);
}

bool LightBar::setBrightness(uint8_t brightness)
{
	_brightness = (brightness <= 100) ? brightness : 100;
	return apply();
}

bool LightBar::setColor(uint32_t color)
{
	_color = color;
	return apply();
}

bool LightBar::setColor(uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t color = (
		static_cast<uint32_t>(r) << 16)
		| (static_cast<uint32_t>(g) << 8)
		| (static_cast<uint32_t>(b)
	);

	return setColor(color);
}

