/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  DeviceWatcher.cpp                                      ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/10/29 14:00:00 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/30 15:55:14 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/


#include "input/DeviceWatcher.hpp"

#include <sys/inotify.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <iostream>

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                           Constructor/Destructor                          │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

DeviceWatcher::DeviceWatcher()
{
	_fd = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
	if (_fd < 0)
	{
		std::clog << "inotify unavailable (" << std::strerror(errno)
			<< "), falling back to periodic scans" << std::endl;
		return;
	}

	if (inotify_add_watch(_fd, "/dev/input", IN_CREATE | IN_ATTRIB) < 0)
	{
		std::clog << "Cannot watch /dev/input (" << std::strerror(errno)
			<< "), falling back to periodic scans" << std::endl;
		close(_fd);
		_fd = -1;
	}
}

DeviceWatcher::~DeviceWatcher()
{
	if (_fd >= 0)
		close(_fd);
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                   Public                                  │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

int DeviceWatcher::GetFd() const { return _fd; }

void DeviceWatcher::Drain()
{
	if (_fd < 0)
		return;

	char buf[4096];
	while (read(_fd, buf, sizeof(buf)) > 0)
	{
	}
}
