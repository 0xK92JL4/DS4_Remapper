/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  LightBar.cpp                                           ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/21 23:48:14 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/31 02:30:36 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "controller/LightBar.hpp"
#include "core/Config.hpp"

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
	Apply();
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

bool LightBar::WriteInt(int fd, uint8_t value)
{
	char buf[4];

	int len = snprintf(buf, sizeof(buf), "%u", value);
	if (len <= 0)
		return false;

	return write(fd, buf, len) == len;
}

bool LightBar::Apply()
{
	if (!_enabled) return true;

	uint8_t r = (_color >> 16) & 0xFF;
	uint8_t g = (_color >> 8) & 0xFF;
	uint8_t b = (_color) & 0xFF;

	auto scale = [this](uint8_t v) -> int {
		return (v * _brightness) / 100;
	};

	bool OK = WriteInt(_fd_r, scale(r))
		&& WriteInt(_fd_g, scale(g))
		&& WriteInt(_fd_b, scale(b));

	return OK;
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Public                                     │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

bool LightBar::Toggle(std::optional<bool> on)
{
	_enabled = (on.has_value()) ? *on : !_enabled;
	return WriteInt(_fd_global, _enabled);
}

bool LightBar::SetBrightness(uint8_t brightness)
{
	_brightness = (brightness <= 100) ? brightness : 100;
	return Apply();
}

bool LightBar::SetColor(uint32_t color)
{
	_color = color;
	return Apply();
}

bool LightBar::SetColor(uint8_t r, uint8_t g, uint8_t b)
{
	uint32_t color = (
		static_cast<uint32_t>(r) << 16)
		| (static_cast<uint32_t>(g) << 8)
		| (static_cast<uint32_t>(b)
	);

	return SetColor(color);
}

void LightBar::StartBlink(std::chrono::milliseconds interval, int cycles)
{
	_blinking = true;
	_blink_interval = interval;
	_blink_cycles = cycles;
	_blink_done = 0;
	_last_toggle = std::chrono::steady_clock::now();
}

void LightBar::StopBlink()
{
	_blinking = false;
	if (!_enabled) Toggle(true);
}

void LightBar::Update()
{
	if (!_blinking)
		return;

	auto now = std::chrono::steady_clock::now();

	if (now - _last_toggle >= _blink_interval)
	{
		Toggle();
		_last_toggle += _blink_interval;

		if (_blink_cycles > 0)
		{
			_blink_done++;
			if (_blink_done >= (_blink_cycles * 2))
			{
				StopBlink();
			}
		}
	}
}
