/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.hpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:29 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/18 23:01:16 by 0xK92JL4                 ▒▒    ▒▒        │*/
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
	private:
		InputDevice		_ds4;
		InputDevice		_touchpad;

		DeviceManager	_manager;

		StickProcessor	_mouse_stick;
		StickProcessor	_scroll_stick;

		VirtualMouse	_mouse;

		int				_axis_lx = 127;
		int				_axis_ly = 127;
		int				_axis_rx = 127;
		int				_axis_ry = 127;

		static constexpr int	MAX_EPOLL_EVENTS = 4;
		struct epoll_event		_returned_events[MAX_EPOLL_EVENTS];

		std::chrono::steady_clock::time_point last_time;

	public:
		EventLoop();

		void Run();
};
