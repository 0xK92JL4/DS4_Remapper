/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.cpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:54 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/20 21:40:05 by 0xK92JL4                 ▒▒    ▒▒        │*/
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

	last_time = std::chrono::steady_clock::now();
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
		std::chrono::duration<float, std::milli> elapsed = current_time - last_time;
		float dt = elapsed.count();
		last_time = current_time;

		if (dt > 100.0f) dt = 100.0f;

		Vec2 move = _mouse_stick.Process(
			_controller.GetLeftStickX() - Config::HALF_AXIS_RANGE,
			_controller.GetLeftStickY() - Config::HALF_AXIS_RANGE,
			Config::MOUSE_SENS_X,
			Config::MOUSE_SENS_Y,
			dt
		);

		Vec2 scroll = _scroll_stick.Process(
			_controller.GetRightStickX() - Config::HALF_AXIS_RANGE,
			_controller.GetRightStickY() - Config::HALF_AXIS_RANGE,
			Config::SCROLL_SENS_X,
			Config::SCROLL_SENS_Y,
			dt
		);

		_mouse.Move(move.x, move.y);
		_mouse.Scroll(scroll.x, scroll.y);
	}
}
