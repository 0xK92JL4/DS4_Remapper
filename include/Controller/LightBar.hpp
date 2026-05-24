/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  LightBar.hpp                                           ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/21 22:59:38 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/24 02:11:41 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include <cstdint>
#include <optional>

class LightBar
{
	private:
		int _fd_r = -1;
		int _fd_g = -1;
		int _fd_b = -1;
		int _fd_global = -1;

		uint32_t	_color = 0x0000FF;
		uint8_t		_brightness = 100;
		bool		_enabled = true;

		bool writeInt(int fd, uint8_t value);
		bool apply();

	public:
		LightBar(uint32_t color);
		~LightBar();

		bool setColor(uint32_t color);
		bool setColor(uint8_t r, uint8_t g, uint8_t b);
		bool setBrightness(uint8_t brightness);

		bool toggle(std::optional<bool> on = std::nullopt);
};

