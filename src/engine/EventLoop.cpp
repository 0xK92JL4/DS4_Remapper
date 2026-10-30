/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.cpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:54 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/29 11:55:22 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "engine/EventLoop.hpp"
#include "core/Config.hpp"
#include "input/DeviceFinder.hpp"

#include <cmath>
#include <cerrno>
#include <cstdint>
#include <stdexcept>

volatile std::sig_atomic_t EventLoop::_stop_requested = 0;

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                         Constructor/Destructor                            │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

EventLoop::EventLoop()
	: _controller(DeviceFinder::Find(Config::DS4_MAC))
{
	_manager.AddDevice(_controller.GetDs4Device());
	_manager.AddDevice(_controller.GetTouchpadDevice());

	_last_time = std::chrono::steady_clock::now();
	_battery_last_time = std::chrono::steady_clock::now();
	_led_last_time = std::chrono::steady_clock::now();
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                 Public                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void EventLoop::RequestStop()
{
	_stop_requested = 1;
}

void EventLoop::Run()
{
	while (!_stop_requested)
	{
		int timeout_ms = _controller.HasActiveMovement() ? 2 : 10;

		int num_ready = _manager.Wait(_returned_events, MAX_EPOLL_EVENTS, timeout_ms);
		if (num_ready < 0)
		{
			if (errno == EINTR) continue;
			throw std::runtime_error("epoll_wait failed");
		}

		for (int i = 0; i < num_ready; i++)
		{
			auto* device = static_cast<InputDevice*>(_returned_events[i].data.ptr);
			uint32_t revents = _returned_events[i].events;

			if (revents & EPOLLIN)
				_controller.HandleDeviceEvent(device, _mouse, _keyboard);

			if ((revents & (EPOLLHUP | EPOLLERR)) || device->IsDisconnected())
				throw std::runtime_error("Input device disconnected");
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

		_controller.Update(dt);

		if (current_time - _battery_last_time >= Config::BATTERY_REFRESH_RATE)
		{
			_battery_last_time += Config::BATTERY_REFRESH_RATE;
			_controller.UpdateBattery();
		}

		if (current_time - _led_last_time >= Config::LED_REFRESH_RATE)
		{
			_led_last_time += Config::LED_REFRESH_RATE;
			_controller.UpdateLightBar();
		}

		_mouse.Move(_controller.GetMove());
		_mouse.Scroll(_controller.GetScroll());

	}
}
