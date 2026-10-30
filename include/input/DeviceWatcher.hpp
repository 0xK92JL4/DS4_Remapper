/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  DeviceWatcher.hpp                                      ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/10/29 14:00:00 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/30 15:55:06 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/


#pragma once

class DeviceWatcher
{
	private:
		int _fd = -1;

	public:
		DeviceWatcher();
		~DeviceWatcher();

		DeviceWatcher(const DeviceWatcher&) = delete;
		DeviceWatcher& operator=(const DeviceWatcher&) = delete;

		int  GetFd() const;

		void Drain();
};
