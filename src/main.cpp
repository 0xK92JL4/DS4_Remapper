#include <iostream>
#include <chrono>
#include <cmath>
#include "Config.hpp"
#include "InputDevice.hpp"
#include "DeviceManager.hpp"
#include "VirtualMouse.hpp"

long long get_time_ms()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

int main()
{
    try
	{
        InputDevice ds4(Config::DS4_DEVICE, false);
        InputDevice touchpad(Config::TOUCHPAD_DEVICE, true);
        
        DeviceManager manager;
        manager.AddDevice(&ds4);
        manager.AddDevice(&touchpad);

        VirtualMouse mouse;

        int axis_lx = 127, axis_ly = 127, axis_rx = 127, axis_ry = 127;
        constexpr int MAX_EPOLL_EVENTS = 4;
        struct epoll_event returned_events[MAX_EPOLL_EVENTS];

        std::cout << "Engine running smoothly in structural C++..." << std::endl;

        while (true)
		{
            int timeout_ms = (std::abs(axis_lx - 127) > Config::DEADZONE || 
                              std::abs(axis_ly - 127) > Config::DEADZONE) ? 2 : 10;

            int num_ready = manager.Wait(returned_events, MAX_EPOLL_EVENTS, timeout_ms);
            if (num_ready < 0)
			{
                if (errno == EINTR) continue;
                break;
            }

            for (int i = 0; i < num_ready; i++)
			{
                auto* active_evdev = static_cast<struct libevdev*>(returned_events[i].data.ptr);
                struct input_event ev;

                while (libevdev_next_event(active_evdev, LIBEVDEV_READ_FLAG_NORMAL, &ev) == LIBEVDEV_READ_STATUS_SUCCESS)
				{
                    if (active_evdev == ds4.GetEvdev())
					{
                        if (ev.type == EV_ABS)
						{
                            if (ev.code == ABS_X)  axis_lx = ev.value;
                            if (ev.code == ABS_Y)  axis_ly = ev.value;
                            if (ev.code == ABS_RX) axis_rx = ev.value;
                            if (ev.code == ABS_RY) axis_ry = ev.value;
                        }
                        else if (ev.type == EV_KEY)
						{
                            auto it = Config::ButtonMap.find(ev.code);
                            if (it != Config::ButtonMap.end())
							{
                                mouse.SendButton(it->second, ev.value);
                            }
                        }
                    }
                    else if (active_evdev == touchpad.GetEvdev())
					{
                        // Ignore touchpad
                    }
                }
            }

            long long current_time = get_time_ms();
            mouse.Move(axis_lx - 127, axis_ly - 127);
            mouse.Scroll(axis_rx - 127, axis_ry - 127, current_time);
        }

    }
	catch (const std::exception& ex)
	{
        std::cerr << "Engine Panic Exception Critical: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
