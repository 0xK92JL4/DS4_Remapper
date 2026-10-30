/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  DeviceManager.cpp                                      ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:58:56 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/30 15:54:55 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "input/DeviceManager.hpp"

#include <unistd.h>
#include <stdexcept>

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                         Constructor/Destructor                            │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

DeviceManager::DeviceManager()
{
    _epoll_fd = epoll_create1(0);
    if (_epoll_fd < 0)
	{
        throw std::runtime_error("Failed to spin up backend epoll sub-instance");
    }
}

DeviceManager::~DeviceManager()
{
    if (_epoll_fd >= 0)
	{
        close(_epoll_fd);
    }
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                 Public                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void DeviceManager::AddDevice(InputDevice* device)
{
    if (!device) return;

    struct epoll_event ev_config{};
    ev_config.events = EPOLLIN;
    ev_config.data.ptr = device;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, device->GetFd(), &ev_config) < 0)
	{
        throw std::runtime_error("Failed configuration attachment inside epoll node registry.");
    }
}

void DeviceManager::RemoveDevice(InputDevice* device)
{
    if (!device) return;

    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, device->GetFd(), nullptr);
}

int DeviceManager::Wait(struct epoll_event* events, int max_events, int timeout_ms)
{
    return epoll_wait(_epoll_fd, events, max_events, timeout_ms);
}
