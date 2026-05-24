/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  LightBar.hpp                                           ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/21 22:59:38 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/25 01:22:16 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include <optional>
#include <cstdint>
#include <chrono>

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

		bool	_blinking = false;
		int		_blink_cycles = -1;
		int		_blink_done = 0;

		std::chrono::milliseconds				_blink_interval{500};
		std::chrono::steady_clock::time_point	_last_toggle;

		bool WriteInt(int fd, uint8_t value);
		bool Apply();

	public:
		LightBar(uint32_t color);
		~LightBar();

		bool SetColor(uint32_t color);
		bool SetColor(uint8_t r, uint8_t g, uint8_t b);
		bool SetBrightness(uint8_t brightness);

		bool Toggle(std::optional<bool> on = std::nullopt);

		void StartBlink(std::chrono::milliseconds interval, int cycles);
		void StopBlink();
		void Update();
};

