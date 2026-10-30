/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.hpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:29 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/30 15:50:27 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "controller/Controller.hpp"
#include "input/DeviceManager.hpp"
#include "output/VirtualMouse.hpp"
#include "output/VirtualKeyboard.hpp"

#include "input/DeviceWatcher.hpp"

#include <chrono>
#include <csignal>
#include <memory>
#include <string>
#include <sys/epoll.h>

class EventLoop
{
	private:
		static volatile std::sig_atomic_t _stop_requested;

		std::unique_ptr<Controller> _controller;
		DeviceManager   _manager;
		DeviceWatcher   _watcher;
		VirtualKeyboard _keyboard;
		VirtualMouse    _mouse;

		static constexpr int    MAX_EPOLL_EVENTS = 4;
		struct epoll_event      _returned_events[MAX_EPOLL_EVENTS];

		std::chrono::steady_clock::time_point _last_time;
		std::chrono::steady_clock::time_point _battery_last_time;
		std::chrono::steady_clock::time_point _led_last_time;

		std::string _last_error;

		void ResetTimers();
		bool TryConnect(bool strict);
		void Disconnect();
		void WaitForController();
		void RunConnected();

	public:
		EventLoop();

		static void RequestStop();

		bool IsConnected() const;
		void Run();
};
