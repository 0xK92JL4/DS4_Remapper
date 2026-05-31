/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  InputDevice.cpp                                        ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:59:03 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/31 02:27:14 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "input/InputDevice.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                         Constructor/Destructor                            │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

InputDevice::InputDevice(const std::string& path, bool grab)
	: _is_grabbed(grab)
{
    _fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
    if (_fd < 0)
	{
        throw std::runtime_error("Failed to open device path: " + path);
    }

    int rc = libevdev_new_from_fd(_fd, &_dev);
    if (rc < 0)
	{
        close(_fd);
        throw std::runtime_error("Failed to initialize libevdev context for: " + path);
    }

    if (_is_grabbed)
	{
        libevdev_grab(_dev, LIBEVDEV_GRAB);
    }
}

InputDevice::~InputDevice()
{
    if (_dev)
	{
        if (_is_grabbed)
		{
            libevdev_grab(_dev, LIBEVDEV_UNGRAB);
        }
        libevdev_free(_dev);
    }
    if (_fd >= 0)
	{
        close(_fd);
    }
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                 Public                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

int InputDevice::GetFd() const { return _fd; }

bool InputDevice::NextEvent(struct input_event& ev)
{
	return (
		libevdev_next_event(_dev, LIBEVDEV_READ_FLAG_NORMAL, &ev)
			== LIBEVDEV_READ_STATUS_SUCCESS
	);
}
