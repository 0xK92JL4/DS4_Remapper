/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  Controller.cpp                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/20 21:21:58 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/24 03:07:04 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "Controller.hpp"
#include "Config.hpp"
#include <cmath>

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                          Constructor/Destructor                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

Controller::Controller()
	: _lightbar(Config::COLOR)
    , _ds4(Config::DS4_DEVICE, false)
    , _touchpad(Config::TOUCHPAD_DEVICE, true) {}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                 Public                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

InputDevice* Controller::GetDs4Device()      { return &_ds4; }
InputDevice* Controller::GetTouchpadDevice() { return &_touchpad; }

void Controller::HandleDeviceEvent(InputDevice* device, VirtualMouse& virtual_mouse)
{
	if (!device) return;

	struct input_event ev;
	while (device->NextEvent(ev))
	{
		if (device == &_ds4)
		{
			if (ev.type == EV_ABS)
			{
				if (ev.code == ABS_X)  _axis_lx = ev.value;
				if (ev.code == ABS_Y)  _axis_ly = ev.value;
				if (ev.code == ABS_RX) _axis_rx = ev.value;
				if (ev.code == ABS_RY) _axis_ry = ev.value;
			}
			else if (ev.type == EV_KEY)
			{
				auto it = Config::ButtonMap.find(ev.code);
				if (it != Config::ButtonMap.end())
				{
					virtual_mouse.SendButton(it->second, ev.value);
				}
			}
		}
	}
}

Vec2 Controller::LeftStickPos()  const { return {_axis_lx, _axis_ly}; }
Vec2 Controller::RightStickPos() const { return {_axis_rx, _axis_ry}; }

bool Controller::HasActiveMovement() const
{
    bool L_Stick = (std::abs(_axis_lx - Config::HALF_AXIS) > Config::DEADZONE
					|| std::abs(_axis_ly - Config::HALF_AXIS) > Config::DEADZONE);

    bool R_Stick = (std::abs(_axis_rx - Config::HALF_AXIS) > Config::DEADZONE
					|| std::abs(_axis_ry - Config::HALF_AXIS) > Config::DEADZONE);

    return L_Stick || R_Stick;
}
