#pragma once
#include <vector>
#include <sys/epoll.h>
#include "InputDevice.hpp"

class DeviceManager
{
	public:
		DeviceManager();
		~DeviceManager();

		void AddDevice(InputDevice* device);
		int Wait(struct epoll_event* events, int max_events, int timeout_ms);

	private:
		int epoll_fd = -1;
};
