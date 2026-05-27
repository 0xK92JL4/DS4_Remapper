/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.cpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:54 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/27 22:34:29 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "EventLoop.hpp"
#include "Config.hpp"

#include <cmath>
#include <cerrno>

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                         Constructor/Destructor                            │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

EventLoop::EventLoop()
{
	_manager.AddDevice(_controller.GetDs4Device());
	_manager.AddDevice(_controller.GetTouchpadDevice());

	_last_time = std::chrono::steady_clock::now();
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                 Public                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void EventLoop::Run()
{
	while (true)
	{
		int timeout_ms = _controller.HasActiveMovement() ? 2 : 10;

		int num_ready = _manager.Wait(_returned_events, MAX_EPOLL_EVENTS, timeout_ms);
		if (num_ready < 0)
		{
			if (errno == EINTR) continue;
			break;
		}

		for (int i = 0; i < num_ready; i++)
		{
			auto* device = static_cast<InputDevice*>(_returned_events[i].data.ptr);
			_controller.HandleDeviceEvent(device, _mouse);
		}

		auto current_time = std::chrono::steady_clock::now();
		auto elapsed = current_time - _last_time;
		float dt = std::chrono::duration<float, std::milli>(elapsed).count();
		_last_time = current_time;

		if (dt > 100.0f) dt = 100.0f;

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
