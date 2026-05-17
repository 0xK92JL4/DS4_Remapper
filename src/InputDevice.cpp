/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  InputDevice.cpp                                        ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:59:03 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/17 00:59:05 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "InputDevice.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

InputDevice::InputDevice(const std::string& path, bool grab) : is_grabbed(grab)
{
    fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
    if (fd < 0)
	{
        throw std::runtime_error("Failed to open device path: " + path);
    }

    int rc = libevdev_new_from_fd(fd, &dev);
    if (rc < 0)
	{
        close(fd);
        throw std::runtime_error("Failed to initialize libevdev context for: " + path);
    }

    if (is_grabbed)
	{
        libevdev_grab(dev, LIBEVDEV_GRAB);
    }
}

InputDevice::~InputDevice()
{
    if (dev)
	{
        if (is_grabbed)
		{
            libevdev_grab(dev, LIBEVDEV_UNGRAB);
        }
        libevdev_free(dev);
    }
    if (fd >= 0)
	{
        close(fd);
    }
}
