/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  Controller.cpp                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/20 21:21:58 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/30 15:53:18 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "controller/Controller.hpp"
#include "engine/ProcessExecutor.hpp"
#include "engine/BindingExecutor.hpp"
#include "core/Config.hpp"
#include <cmath>
#include <iostream>
#include <exception>

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                          Constructor/Destructor                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

Controller::Controller(const Ds4Paths& paths)
	: _ds4(paths.gamepad_event, false)
	, _touchpad(paths.touchpad_event, true)
	, _mouse_stick(Config::MOUSE_SENS_X, Config::MOUSE_SENS_Y)
	, _scroll_stick(Config::SCROLL_SENS_X, Config::SCROLL_SENS_Y)
{
	try
    {
        _lightbar.emplace(paths, Config::COLOR);
    }
	catch (const std::exception& e)
	{
		std::clog << "LightBar disabled: " << e.what() << std::endl;
	}

	try
    {
        _battery.emplace(paths);
    }
	catch (const std::exception& e)
	{
		std::clog << "Battery disabled: " << e.what() << std::endl;
	}
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Private                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void Controller::HandleAbsEvent(const input_event& ev)
{
	switch (ev.code)
	{
		case ABS_X:   _axis_lx = ev.value;  break;
		case ABS_Y:   _axis_ly = ev.value;  break;
		case ABS_RX:  _axis_rx = ev.value;  break;
		case ABS_RY:  _axis_ry = ev.value;  break;
	}
}

void Controller::HandleKeyEvent(const input_event& ev, VirtualMouse& mouse,
		VirtualKeyboard& keyboard)
{
	auto it = Config::InputMap.find(ev.code);
	if (it == Config::InputMap.end()) return;

	if (ev.value == Input::PRESS)
		_pressed.insert(ev.code);
	else if (ev.value == Input::RELEASE)
		_pressed.erase(ev.code);

	ExecuteAction(it->second, ev.value, mouse, keyboard);
}

void Controller::ExecuteAction(const Action& action, int value,
		VirtualMouse& mouse, VirtualKeyboard& keyboard)
{
	switch (action.type)
	{
		case ActionType::MouseButton:
			if (value != Input::REPEAT)
				mouse.SendButton(action.code, value);
			break;

		case ActionType::KeyboardKey:
			keyboard.SendKey(action.code, value);
			break;

		case ActionType::Binding:
		{
			if (value == Input::PRESS)
				BindingExecutor::Press(action, mouse, keyboard);
			else if (value == Input::RELEASE)
				BindingExecutor::Release(action, mouse, keyboard);
			break;
		}

		case ActionType::Command:
		{
			if (value == Input::PRESS)
				ProcessExecutor::Execute(action.cmd_args);
			break;
		}
	}
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                 Public                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

InputDevice* Controller::GetDs4Device()      { return &_ds4; }
InputDevice* Controller::GetTouchpadDevice() { return &_touchpad; }

void Controller::HandleDeviceEvent(InputDevice* device, VirtualMouse& mouse,
		VirtualKeyboard& keyboard)
{
	if (!device)
		return;

	struct input_event ev;

	while (device->NextEvent(ev))
	{
		if (device != &_ds4)
			continue;

		switch (ev.type)
		{
			case EV_ABS:
				HandleAbsEvent(ev);
				break;

			case EV_KEY:
				HandleKeyEvent(ev, mouse, keyboard);
				break;
		}
	}
}

void Controller::Update(float dt)
{
	const Vec2 left {_axis_lx, _axis_ly};
	const Vec2 right{_axis_rx, _axis_ry};

	_move   = _mouse_stick.Process(left, dt);
	_scroll = _scroll_stick.Process(right, dt);
}

void Controller::UpdateLightBar() { if (_lightbar) _lightbar->Update(); }
void Controller::UpdateBattery()  { if (_battery)  _battery->Update(); }

bool Controller::HasBattery() const { return _battery.has_value(); }

int Controller::GetBatteryCapacity() const
{
	return _battery ? _battery->GetCapacity() : -1;
}

BatteryStatus Controller::GetBatteryStatus() const
{
	return _battery ? _battery->GetStatus() : BatteryStatus::Unknown;
}

void Controller::ReleaseAll(VirtualMouse& mouse, VirtualKeyboard& keyboard)
{
	for (int code : _pressed)
	{
		auto it = Config::InputMap.find(code);
		if (it != Config::InputMap.end())
			ExecuteAction(it->second, Input::RELEASE, mouse, keyboard);
	}
	_pressed.clear();
}

Vec2 Controller::GetMove()  const  { return _move; }
Vec2 Controller::GetScroll() const { return _scroll; }

bool Controller::HasActiveMovement() const
{
    bool L_Stick = (std::abs(_axis_lx - Config::HALF_AXIS) > Config::DEADZONE
					|| std::abs(_axis_ly - Config::HALF_AXIS) > Config::DEADZONE);

    bool R_Stick = (std::abs(_axis_rx - Config::HALF_AXIS) > Config::DEADZONE
					|| std::abs(_axis_ry - Config::HALF_AXIS) > Config::DEADZONE);

    return L_Stick || R_Stick;
}
