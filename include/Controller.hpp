/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  Controller.hpp                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/19 20:20:08 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/27 22:33:00 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "Controller/LightBar.hpp"
#include "Controller/Battery.hpp"
#include "StickProcessor.hpp"
#include "InputDevice.hpp"
#include "VirtualMouse.hpp"
#include "structs.hpp"

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

		void	HandleDeviceEvent(InputDevice* device, VirtualMouse& virtual_mouse);
		void	Update(float dt);

		void    UpdateLightBar();
		void    UpdateBattery();

		Vec2	GetMove() const;
		Vec2	GetScroll() const;

		bool	HasActiveMovement() const;
};
