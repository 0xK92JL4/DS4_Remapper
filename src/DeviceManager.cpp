/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  DeviceManager.cpp                                      ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:58:56 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/17 14:47:04 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "DeviceManager.hpp"

#include <unistd.h>
#include <stdexcept>

DeviceManager::DeviceManager()
{
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
	{
        throw std::runtime_error("Failed to spin up backend epoll sub-instance");
    }
}

DeviceManager::~DeviceManager()
{
    if (epoll_fd >= 0)
	{
        close(epoll_fd);
    }
}

void DeviceManager::AddDevice(InputDevice* device)
{
    if (!device) return;

    struct epoll_event ev_config{};
    ev_config.events = EPOLLIN;
    ev_config.data.ptr = device->GetEvdev();

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, device->GetFd(), &ev_config) < 0)
	{
        throw std::runtime_error("Failed configuration attachment inside epoll node registry.");
    }
}

int DeviceManager::Wait(struct epoll_event* events, int max_events, int timeout_ms)
{
    return epoll_wait(epoll_fd, events, max_events, timeout_ms);
}
