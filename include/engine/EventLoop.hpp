/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.hpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:29 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/31 02:26:52 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "controller/Controller.hpp"
#include "input/DeviceManager.hpp"
#include "output/VirtualMouse.hpp"

#include <chrono>
#include <sys/epoll.h>

class EventLoop
{
	private:
		Controller		_controller;
		DeviceManager	_manager;
		VirtualKeyboard	_keyboard;
		VirtualMouse	_mouse;

		static constexpr int	MAX_EPOLL_EVENTS = 4;
		struct epoll_event		_returned_events[MAX_EPOLL_EVENTS];

		std::chrono::steady_clock::time_point _last_time;
		std::chrono::steady_clock::time_point _battery_last_time;
		std::chrono::steady_clock::time_point _led_last_time;

	public:
		EventLoop();
		void Run();
};
