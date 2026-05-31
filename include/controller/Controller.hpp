/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  Controller.hpp                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/19 20:20:08 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/31 02:26:04 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "controller/LightBar.hpp"
#include "controller/Battery.hpp"
#include "engine/StickProcessor.hpp"
#include "input/InputDevice.hpp"
#include "output/VirtualKeyboard.hpp"
#include "output/VirtualMouse.hpp"
#include "core/structs.hpp"

#include <linux/input.h>

class Controller
{
	private:
		LightBar	_lightbar;
		Battery		_battery;
		InputDevice	_ds4;
		InputDevice	_touchpad;

		StickProcessor  _mouse_stick;
		StickProcessor  _scroll_stick;

		int _axis_lx = 127;
		int _axis_ly = 127;
		int _axis_rx = 127;
		int _axis_ry = 127;

		Vec2 _move   = {0,0};
		Vec2 _scroll = {0,0};

	public:
		Controller();
		~Controller() = default;

		Controller(const Controller&) = delete;
		Controller& operator=(const Controller&) = delete;

		InputDevice* GetDs4Device();
		InputDevice* GetTouchpadDevice();

		void	HandleDeviceEvent(InputDevice*, VirtualMouse&, VirtualKeyboard&);
		void	Update(float dt);

		void    UpdateLightBar();
		void    UpdateBattery();

		Vec2	GetMove() const;
		Vec2	GetScroll() const;

		bool	HasActiveMovement() const;
};
