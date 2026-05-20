/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.hpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:29 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/20 23:48:35 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "Controller.hpp"
#include "DeviceManager.hpp"
#include "StickProcessor.hpp"
#include "VirtualMouse.hpp"

#include <chrono>
#include <sys/epoll.h>

class EventLoop
{
	private:
		Controller      _controller;
		DeviceManager   _manager;
		VirtualMouse    _mouse;

		StickProcessor  _mouse_stick;
		StickProcessor  _scroll_stick;

		static constexpr int MAX_EPOLL_EVENTS = 4;
		struct epoll_event   _returned_events[MAX_EPOLL_EVENTS];

		std::chrono::steady_clock::time_point _last_time;

	public:
		EventLoop();
		void Run();
};
