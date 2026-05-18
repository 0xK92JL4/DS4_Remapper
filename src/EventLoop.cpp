/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.cpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:54 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/18 23:27:51 by 0xK92JL4                 ▒▒    ▒▒        │*/
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
	: _ds4(Config::DS4_DEVICE, false)
	, _touchpad(Config::TOUCHPAD_DEVICE, true)
{
	_manager.AddDevice(&_ds4);
	_manager.AddDevice(&_touchpad);

	last_time = std::chrono::steady_clock::now();
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                 Public                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void EventLoop::Run()
{
	while (true)
	{
		int timeout_ms = (
			std::abs(_axis_lx - 127) > Config::DEADZONE
			|| std::abs(_axis_ly - 127) > Config::DEADZONE
		) ? 2 : 10;

		int num_ready =
			_manager.Wait(_returned_events, MAX_EPOLL_EVENTS, timeout_ms);

		if (num_ready < 0)
		{
			if (errno == EINTR)
				continue;

			break;
		}

		for (int i = 0; i < num_ready; i++)
		{
			auto* device =
				static_cast<InputDevice*>(_returned_events[i].data.ptr);

			struct input_event ev;

			while (device->NextEvent(ev))
			{
				if (device == &_ds4)
				{
					if (ev.type == EV_ABS)
					{
						if (ev.code == ABS_X) _axis_lx = ev.value;
						if (ev.code == ABS_Y) _axis_ly = ev.value;
						if (ev.code == ABS_RX) _axis_rx = ev.value;
						if (ev.code == ABS_RY) _axis_ry = ev.value;
					}
					else if (ev.type == EV_KEY)
					{
						auto it = Config::ButtonMap.find(ev.code);

						if (it != Config::ButtonMap.end())
							_mouse.SendButton(it->second, ev.value);
					}
				}
			}
		}

		auto current_time = std::chrono::steady_clock::now();

		std::chrono::duration<float, std::milli>
			elapsed = current_time - last_time;

		float dt = elapsed.count();

		last_time = current_time;

		if (dt > 100.0f)
			dt = 100.0f;

		Vec2 move =
			_mouse_stick.Process(
				_axis_lx - Config::HALF_AXIS_RANGE,
				_axis_ly - Config::HALF_AXIS_RANGE,
				Config::MOUSE_SENS_X,
				Config::MOUSE_SENS_Y,
				dt
			);

		Vec2 scroll =
			_scroll_stick.Process(
				_axis_rx - Config::HALF_AXIS_RANGE,
				_axis_ry - Config::HALF_AXIS_RANGE,
				Config::SCROLL_SENS_X,
				Config::SCROLL_SENS_Y,
				dt
			);

		_mouse.Move(move.x, move.y);
		_mouse.Scroll(scroll.x, scroll.y);
	}
}
