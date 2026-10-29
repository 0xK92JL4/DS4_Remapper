/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  InputDevice.cpp                                        ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:59:03 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/29 11:59:10 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "input/InputDevice.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <cerrno>

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

    if (_is_grabbed && libevdev_grab(_dev, LIBEVDEV_GRAB) < 0)
	{
        libevdev_free(_dev);
        close(_fd);
        throw std::runtime_error("Failed to grab device: " + path);
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

bool InputDevice::IsDisconnected() const { return _disconnected; }

bool InputDevice::NextEvent(struct input_event& ev)
{
	if (!_dev || _disconnected)
		return false;

	unsigned int flag = _syncing
		? LIBEVDEV_READ_FLAG_SYNC
		: LIBEVDEV_READ_FLAG_NORMAL;

	int rc = libevdev_next_event(_dev, flag, &ev);

	if (rc == LIBEVDEV_READ_STATUS_SUCCESS)
		return true;

	if (rc == LIBEVDEV_READ_STATUS_SYNC)
	{
		_syncing = true;
		return true;
	}

	if (rc == -ENODEV)
		_disconnected = true;

	_syncing = false;
	return false;
}
