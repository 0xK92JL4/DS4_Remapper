/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.hpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:29 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/17 23:00:41 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "InputDevice.hpp"
#include "DeviceManager.hpp"
#include "StickProcessor.hpp"
#include "VirtualMouse.hpp"

#include <chrono>
#include <sys/epoll.h>

class EventLoop
{
	public:
		EventLoop();

		void Run();

	private:
		InputDevice ds4;
		InputDevice touchpad;

		DeviceManager manager;

		StickProcessor mouse_stick;
		StickProcessor scroll_stick;

		VirtualMouse mouse;

		int axis_lx = 127;
		int axis_ly = 127;
		int axis_rx = 127;
		int axis_ry = 127;

		static constexpr int MAX_EPOLL_EVENTS = 4;
		struct epoll_event returned_events[MAX_EPOLL_EVENTS];

		std::chrono::steady_clock::time_point last_time;
};
