/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.cpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:54 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/30 15:54:01 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/


#include "engine/EventLoop.hpp"
#include "core/Config.hpp"
#include "input/DeviceFinder.hpp"

#include <cmath>
#include <cerrno>
#include <cstdint>
#include <iostream>
#include <stdexcept>

#include <poll.h>

volatile std::sig_atomic_t EventLoop::_stop_requested = 0;

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                           Constructor/Destructor                          │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

EventLoop::EventLoop()
{
	ResetTimers();
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                  Private                                  │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void EventLoop::ResetTimers()
{
	auto now = std::chrono::steady_clock::now();

	_last_time = now;
	_battery_last_time = now;
	_led_last_time = now;
}

bool EventLoop::TryConnect(bool strict)
{
	try
	{
		Ds4Paths paths = DeviceFinder::Find(Config::DS4_MAC, strict);

		auto controller = std::make_unique<Controller>(paths);

		_manager.AddDevice(controller->GetDs4Device());
		_manager.AddDevice(controller->GetTouchpadDevice());

		_controller = std::move(controller);
		_last_error.clear();
		return true;
	}
	catch (const std::exception& e)
	{
		if (_last_error != e.what())
		{
			_last_error = e.what();
			std::clog << "Waiting for DS4: " << _last_error << std::endl;
		}
		return false;
	}
}

void EventLoop::Disconnect()
{
	if (!_controller)
		return;

	_controller->ReleaseAll(_mouse, _keyboard);

	_manager.RemoveDevice(_controller->GetDs4Device());
	_manager.RemoveDevice(_controller->GetTouchpadDevice());

	_controller.reset();
}

void EventLoop::WaitForController()
{
	int fast_retries = 0;

	while (!_stop_requested)
	{
		if (TryConnect(fast_retries > 0))
			return;

		struct pollfd pfd = { _watcher.GetFd(), POLLIN, 0 };
		int timeout_ms = (fast_retries > 0) ? 250 : 5000;

		int ready = poll(&pfd, 1, timeout_ms);

		if (ready > 0)
		{
			_watcher.Drain();
			fast_retries = 12;
		}
		else if (fast_retries > 0)
		{
			fast_retries--;
		}
	}
}

void EventLoop::RunConnected()
{
	ResetTimers();

	while (!_stop_requested)
	{
		int timeout_ms = _controller->HasActiveMovement() ? 2 : 10;

		int num_ready = _manager.Wait(_returned_events, MAX_EPOLL_EVENTS, timeout_ms);
		if (num_ready < 0)
		{
			if (errno == EINTR) continue;
			throw std::runtime_error("epoll_wait failed");
		}

		bool lost = false;

		for (int i = 0; i < num_ready; i++)
		{
			auto* device = static_cast<InputDevice*>(_returned_events[i].data.ptr);
			uint32_t revents = _returned_events[i].events;

			if (revents & EPOLLIN)
				_controller->HandleDeviceEvent(device, _mouse, _keyboard);

			if ((revents & (EPOLLHUP | EPOLLERR)) || device->IsDisconnected())
				lost = true;
		}

		if (lost)
		{
			std::clog << "DS4 disconnected" << std::endl;
			Disconnect();
			return;
		}

		auto current_time = std::chrono::steady_clock::now();
		auto elapsed = current_time - _last_time;
		float dt = std::chrono::duration<float, std::milli>(elapsed).count();
		_last_time = current_time;

		if (dt > 100.0f)
		{
			dt = 100.0f;
			_battery_last_time = current_time;
			_led_last_time = current_time;
		}

		_controller->Update(dt);

		if (current_time - _battery_last_time >= Config::BATTERY_REFRESH_RATE)
		{
			_battery_last_time += Config::BATTERY_REFRESH_RATE;
			_controller->UpdateBattery();
		}

		if (current_time - _led_last_time >= Config::LED_REFRESH_RATE)
		{
			_led_last_time += Config::LED_REFRESH_RATE;
			_controller->UpdateLightBar();
		}

		_mouse.Move(_controller->GetMove());
		_mouse.Scroll(_controller->GetScroll());
	}
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                   Public                                  │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void EventLoop::RequestStop()
{
	_stop_requested = 1;
}

bool EventLoop::IsConnected() const
{
	return _controller != nullptr;
}

void EventLoop::Run()
{
	while (!_stop_requested)
	{
		if (_controller)
			RunConnected();
		else
			WaitForController();
	}

	Disconnect();
}
