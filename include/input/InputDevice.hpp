/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  InputDevice.hpp                                        ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:58:32 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/18 22:56:00 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include <string>
#include <libevdev/libevdev.h>

class InputDevice
{
	private:
		int					_fd = -1;
		struct libevdev*	_dev = nullptr;
		bool				_is_grabbed = false;
		
	public:
		InputDevice(const std::string& path, bool grab = false);
		~InputDevice();

		int		GetFd() const;
		bool	NextEvent(struct input_event& ev);
};
