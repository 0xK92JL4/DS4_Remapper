/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  DeviceManager.hpp                                      ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:58:17 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/18 22:54:11 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include <vector>
#include <sys/epoll.h>
#include "InputDevice.hpp"

class DeviceManager
{
	private:
		int	_epoll_fd = -1;

	public:
		DeviceManager();
		~DeviceManager();

		void	AddDevice(InputDevice* device);
		int		Wait(struct epoll_event* events, int max_events, int timeout_ms);
};
