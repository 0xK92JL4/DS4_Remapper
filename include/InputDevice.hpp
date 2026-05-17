/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  InputDevice.hpp                                        ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:58:32 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/17 00:58:34 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include <string>
#include <libevdev/libevdev.h>

class InputDevice
{
	public:
		InputDevice(const std::string& path, bool grab = false);
		~InputDevice();

		int GetFd() const { return fd; }
		struct libevdev* GetEvdev() { return dev; }

	private:
		int fd = -1;
		struct libevdev* dev = nullptr;
		bool is_grabbed = false;
};
