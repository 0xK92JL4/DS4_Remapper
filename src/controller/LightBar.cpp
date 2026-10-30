/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  LightBar.cpp                                           ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/21 23:48:14 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/29 11:56:23 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "controller/LightBar.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <stdexcept>
#include <algorithm>
#include <string>

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                          Constructor/Destructor                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

LightBar::LightBar(const Ds4Paths& paths, uint32_t color)
{
	_fd_r = open(paths.led_r.c_str(), O_WRONLY);
	_fd_g = open(paths.led_g.c_str(), O_WRONLY);
	_fd_b = open(paths.led_b.c_str(), O_WRONLY);
	_fd_global = open(paths.led_global.c_str(), O_WRONLY);

	if (_fd_r < 0 || _fd_g < 0 || _fd_b < 0 || _fd_global < 0)
	{
		if (_fd_r >= 0) close(_fd_r);
		if (_fd_g >= 0) close(_fd_g);
		if (_fd_b >= 0) close(_fd_b);
		if (_fd_global >= 0) close(_fd_global);
		throw std::runtime_error("LightBar: failed to open sysfs LED nodes");
	}

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

	if (!WriteInt(_fd_global, _enabled))
		return false;

	return _enabled ? Apply() : true;
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
		if (now - _last_toggle >= _blink_interval)
			_last_toggle = now;

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
