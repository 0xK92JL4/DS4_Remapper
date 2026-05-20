/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  Controller.hpp                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/19 20:20:08 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/20 23:31:09 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "InputDevice.hpp"
#include "VirtualMouse.hpp"
#include "structs.hpp"

#include <linux/input.h>

class Controller
{
	private:
		InputDevice _ds4;
		InputDevice _touchpad;

		int _axis_lx = 127;
		int _axis_ly = 127;
		int _axis_rx = 127;
		int _axis_ry = 127;

	public:
		Controller();
		~Controller() = default;

		Controller(const Controller&) = delete;
		Controller& operator=(const Controller&) = delete;

		InputDevice* GetDs4Device();
		InputDevice* GetTouchpadDevice();

		void	HandleDeviceEvent(InputDevice* device, VirtualMouse& virtual_mouse);

		Vec2	LeftStickPos() const;
		Vec2	RightStickPos() const;

		bool	HasActiveMovement() const;
};
