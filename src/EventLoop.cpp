/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.cpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:54 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/24 03:15:57 by 0xK92JL4                 ▒▒    ▒▒        │*/
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
	: _mouse_stick(Config::MOUSE_SENS_X, Config::MOUSE_SENS_Y)
	, _scroll_stick(Config::SCROLL_SENS_X, Config::SCROLL_SENS_Y)

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

		Vec2 move = _mouse_stick.Process(_controller.LeftStickPos(), dt);
		Vec2 scroll = _scroll_stick.Process(_controller.RightStickPos(), dt);

		_mouse.Move(move.x, move.y);
		_mouse.Scroll(scroll.x, scroll.y);

		//_controller.update();
	}
}
