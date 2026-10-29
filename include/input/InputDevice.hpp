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
		bool				_syncing = false;
		bool				_disconnected = false;

	public:
		InputDevice(const std::string& path, bool grab = false);
		~InputDevice();

		InputDevice(const InputDevice&) = delete;
		InputDevice& operator=(const InputDevice&) = delete;

		int		GetFd() const;
		bool	IsDisconnected() const;
		bool	NextEvent(struct input_event& ev);
};
