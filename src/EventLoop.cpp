/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  EventLoop.cpp                                          ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 23:00:54 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/17 23:16:02 by 0xK92JL4                 ▒▒    ▒▒        │*/
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
	: ds4(Config::DS4_DEVICE, false)
	, touchpad(Config::TOUCHPAD_DEVICE, true)
{
	manager.AddDevice(&ds4);
	manager.AddDevice(&touchpad);

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
			std::abs(axis_lx - 127) > Config::DEADZONE
			|| std::abs(axis_ly - 127) > Config::DEADZONE
		) ? 2 : 10;

		int num_ready =
			manager.Wait(returned_events, MAX_EPOLL_EVENTS, timeout_ms);

		if (num_ready < 0)
		{
			if (errno == EINTR)
				continue;

			break;
		}

		for (int i = 0; i < num_ready; i++)
		{
			auto* active_evdev =
				static_cast<struct libevdev*>(returned_events[i].data.ptr);

			struct input_event ev;

			while (libevdev_next_event( active_evdev, LIBEVDEV_READ_FLAG_NORMAL, &ev)
				== LIBEVDEV_READ_STATUS_SUCCESS)
			{
				if (active_evdev == ds4.GetEvdev())
				{
					if (ev.type == EV_ABS)
					{
						if (ev.code == ABS_X) axis_lx = ev.value;
						if (ev.code == ABS_Y) axis_ly = ev.value;
						if (ev.code == ABS_RX) axis_rx = ev.value;
						if (ev.code == ABS_RY) axis_ry = ev.value;
					}
					else if (ev.type == EV_KEY)
					{
						auto it = Config::ButtonMap.find(ev.code);

						if (it != Config::ButtonMap.end())
							mouse.SendButton(it->second, ev.value);
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
			mouse_stick.Process(
				axis_lx - 127,
				axis_ly - 127,
				Config::MOUSE_SENS_X,
				Config::MOUSE_SENS_Y,
				dt
			);

		Vec2 scroll =
			scroll_stick.Process(
				axis_rx - 127,
				axis_ry - 127,
				Config::SCROLL_SENS_X,
				Config::SCROLL_SENS_Y,
				dt
			);

		mouse.Move(move.x, move.y);
		mouse.Scroll(scroll.x, scroll.y);
	}
}
