/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.hpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:29 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/29 11:54:30 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "controller/Controller.hpp"
#include "input/DeviceManager.hpp"
#include "output/VirtualMouse.hpp"
#include "output/VirtualKeyboard.hpp"

#include <chrono>
#include <csignal>
#include <sys/epoll.h>

class EventLoop
{
	private:
		static volatile std::sig_atomic_t	_stop_requested;

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

		static void RequestStop();

		void Run();
};
